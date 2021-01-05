#ifndef GUIX_RESMGR_H_
#define GUIX_RESMGR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *res_alloc(size_t size);
void res_free(void* ptr);
void res_free_all(void);

#ifdef __cplusplus
}
#endif
#endif /* GUIX_RESMGR_H_ */