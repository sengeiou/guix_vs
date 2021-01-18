#include "resource_base.h"


void* resource_alloc(struct list_head *root,
	resource_allocate_t allocate,
	reource_release_t release,
	size_t size)
{
	struct resource_node *res = allocate(size + sizeof(struct resource_node));
	if (!res)
		return NULL;
	res->base.release = release;
	list_add(&res->base.node, root);
	return res->data;
}

void resource_release(struct resource_node *res)
{
	list_del(&res->base.node);
	res->base.release(res);
}

void resources_release(struct list_head *root)
{
	struct list_head* pos, * next;
	list_for_each_safe(pos, next, root) {
		struct resource_node* curr = container_of(pos,
			struct resource_node, base.node);
		resource_release(curr);
	}
}