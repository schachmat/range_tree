// See LICENSE file for copyright and license details.

#include <assert.h>
#include <stdlib.h>
#include "range_tree.h"

// BEWARE: in this file the range_node type is used as list elements.


// check a list of value-ranges and merge overlapping elements
range_node* range_list_merge_ranges(range_node* first, val_cmp comp, val_inc inc)
{
	assert(first);
	range_node* curr = first;
	range_node* last = first; // we return this later
	while(curr != NULL) {
		assert(comp(curr->min, curr->max) <= 0);	// in jedem knoten muss min<=max sein
		void* new_curr_max = curr->max;
		range_node* tmp = curr->right;
		while(tmp != NULL) { // for each element, which can be joined into curr.
			assert(comp(curr->min, tmp->min) <= 0);	// jeder knoten muss min<=min des nächsten knotens sein
			void* a = new_curr_max;
			inc(a);
			if(comp(a, tmp->min) < 0) break;	// no more element to join into curr ==> cancel
			if(comp(new_curr_max, tmp->max) < 0)
				new_curr_max = tmp->max;
			tmp->left = NULL;
			tmp = tmp->right;
			curr->right->right = NULL; // not really necessary
			free(curr->right);
			curr->right = tmp;
		}
		// update values for curr and (if existent) tmp
		curr->max = new_curr_max;
		if(tmp) tmp->left = curr;
		last = curr;
		// move curr pointer forward
		curr = curr->right;
	}
	return last;
}

// creates a tree out of a sorted list
range_node* range_tree_build_from_list(range_node* first, range_node* last)
{
	// stop recursion
	if(!first || !last) return NULL;
	
	range_node* root = last;
	range_node* tmp = first;
	
	// first find our root
	while(root != tmp && root->left != tmp) {
		root = root->left;
		tmp  = tmp->right;
	}
	
	// now detach left and right list
	if(root->left)  root->left->right = NULL;
	if(root->right) root->right->left = NULL;

	// recurse
	root->left  = range_tree_build_from_list(first, root->left);
	root->right = range_tree_build_from_list(root->right, last);

	return root;
}

// merges two lists into one for natural mergesort
static range_node* range_list_natural_merge(range_node* Afirst, range_node* Bfirst, val_cmp comp)
{
	if(!Afirst || !Bfirst) return (Afirst ? Afirst : Bfirst);
	
	range_node* cursrc;
	range_node* curdst;
	range_node* dst;
	range_node* lastcurdst;

	int cmp = comp(Afirst->min, Bfirst->min);
	dst    = (cmp<=0 ? Afirst : Bfirst);
	cursrc = (cmp<=0 ? Bfirst : Afirst);
	curdst = dst->right;
	lastcurdst = dst;

	while(curdst != NULL && cursrc != NULL) {
		if(comp(curdst->min, cursrc->min) <= 0) {
			lastcurdst = curdst;
			curdst = curdst->right;
		} else {
			curdst->left->right	= cursrc;
			cursrc->left 		= curdst->left;
			curdst->left		= cursrc;
			cursrc				= cursrc->right;
			curdst->left->right	= curdst;
		}
	}
	if(cursrc != NULL) {
		lastcurdst->right = cursrc;
		cursrc->left = lastcurdst;
	}

	return dst;
}

// uses natural mergesort to sort the list
range_node* range_list_natural_mergesort(range_node* first, val_cmp comp)
{
	if(!first) return NULL;

	range_node* tmp = first->right;
	range_node* last = first;

	while(tmp != NULL) {
		if(comp(last->min, tmp->min) <= 0) {
			last = tmp;
			tmp = tmp->right;
		} else break;
	}

	if(tmp != NULL) {
		tmp->left = NULL;
		last->right = NULL;
		return range_list_natural_merge(first, range_list_natural_mergesort(tmp, comp), comp);
	} else
		return first;
}

