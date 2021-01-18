#ifndef RESOURCE_BASE_H_
#define RESOURCE_BASE_H_

#include <stdint.h>
#include <stddef.h>
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void  (*reource_release_t)(struct resource_node *);
typedef void* (*resource_allocate_t)(size_t);

struct resource_base {
	struct list_head node;
	reource_release_t release;
};

struct resource_node {
	struct resource_base base;
	uint8_t data[];
};

void *resource_alloc(struct list_head *root,
					 resource_allocate_t allocate,
					 reource_release_t release,
					 size_t size);
void resource_release(struct resource_node* res);
void resources_release(struct list_head *root);

#ifdef __cplusplus
}
#endif
#endif /* RESOURCE_BASE_H_ */