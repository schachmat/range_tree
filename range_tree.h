// See LICENSE file for copyright and license details.

// Compares two values
typedef int (*val_cmp)(const void* a, const void* b);

// Increments a value by the minimal possible amount
typedef void (*val_inc)(const void* val);

typedef struct range_node {
	struct range_node*	left;
	struct range_node*	right;
	void*		min;
	void*		max;
} range_node;

range_node*	range_list_merge_ranges(range_node*, val_cmp, val_inc);
range_node*	range_list_natural_mergesort(range_node*, val_cmp);
range_node*	range_tree_build_from_list(range_node*, range_node*);
void		range_tree_clear(range_node*);
int			range_tree_contains(const range_node*, const void*, val_cmp);
range_node*	range_tree_create(void*, void*);
