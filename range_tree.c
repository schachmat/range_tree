// See LICENSE file for copyright and license details.

#include <assert.h>
#include <stdlib.h>
#include "range_tree.h"

// clears a range_tree node recursively and frees all memory
void range_tree_clear(range_node* tree)
{
	if(! tree) return;
	range_tree_clear(tree->left);
	range_tree_clear(tree->right);
	free(tree);
	tree = NULL;	//TODO ???
}

// creates a range_tree node
range_node* range_tree_create(void* val_min, void* val_max)
{
	range_node* root = malloc(sizeof(range_node));
	assert(root);
	root->min = val_min;
	root->max = val_max;
	root->left = NULL;
	root->right = NULL;
	return root;
}

// checks if a range_tree contains a given value
int range_tree_contains(const range_node* tree, const void* val, val_cmp comp)
{
	if(!tree) return 0;
	assert(tree && val);
	if(comp(tree->min, val) > 0) {			// val kleiner als aktueller tree-knoten
		if(tree->left) return range_tree_contains(tree->left, val, comp);
		else return 0;
	} else if(comp(tree->max, val) < 0) {		// val größer als aktueller tree-knoten
		if(tree->right) return range_tree_contains(tree->right, val, comp);
		else return 0;
	}
	return 1;
}
