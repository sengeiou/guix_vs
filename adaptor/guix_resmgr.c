#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "guix_resmgr.h"
#include "resource_base.h"

static LIST_HEAD(res_root);
static void free_node(struct resource_node* res)
{
	if (res)
		free(res);
}

void *res_alloc(size_t size)
{
	void* ptr = resource_alloc(&res_root, malloc, 
		free, size);
	if (!ptr) {
		printf("No more memory\n");
		return NULL;
	}
	memset(ptr, 0, size);
	return ptr;
}

void res_free(void* ptr)
{
	struct resource_node* res;
	if (!ptr)
		return;

	res = container_of(ptr, struct resource_node, data);
	resource_release(res);
}

void res_free_all(void)
{
	resources_release(&res_root);
}