#include <stdlib.h>
#include <stdio.h>

#include "guix_resmgr.h"
#include "list.h"


struct res_node {
	struct list_head node;
	void (*destory)(struct res_node* res);
	uint8_t data[];
};

static LIST_HEAD(res_root);

static void free_node(struct res_node* res)
{
	if (res)
		free(res);
}

void res_free(void* ptr)
{
	struct res_node* res;
	if (!ptr)
		return;

	res = container_of(ptr, struct res_node, data);
	list_del(&res->node);
	res->destory(res);
}

void *res_alloc(size_t size)
{
	struct res_node* res;
	res = calloc(1, sizeof(struct res_node) + size);
	if (!res) {
		printf("No more memory\n");
		return NULL;
	}

	res->destory = free_node;
	list_add(&res->node, &res_root);
	return res->data;
}

void res_free_all(void)
{
	struct list_head *pos, *next;
	struct res_node *curr;

	list_for_each_safe(pos, next, &res_root) {
		curr = container_of(pos, struct res_node, node);
		list_del(pos);
		curr->destory(curr);
	}
}