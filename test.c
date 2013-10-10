// See LICENSE file for copyright and license details.

#include "range_tree.h"


// Compares two values
int cmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

// Increments a value by the minimal possible amount
void inc(const void* val)
{
	*(int*)val = *(int*)val + 1;
}

int main(int argc, char* argv[])
{
	int a = 5;
	int b = 7;
	range_node* node = range_tree_create(&a, &b);
	range_tree_clear(node);
	return 0;
}
