#ifndef GUIX_LOADRES_H_
#define GUIX_LOADRES_H_

#ifdef __cplusplus
extern "C" {
#endif

struct GX_THEME_STRUCT;
struct GX_STRING_STRUCT;

int guix_binres_load(const char* filename, int theme_id,
	struct GX_THEME_STRUCT** theme, struct GX_STRING_STRUCT*** language);

#ifdef __cplusplus
}
#endif
#endif /* GUIX_LOADRES_H_ */