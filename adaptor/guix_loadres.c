#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>

#include "guix_resmgr.h"
#include "gx_api.h"

#define FILE_SIZE_LIMIT (10 * 1024 * 1024ul) /* 10MB */

static int file_get_size(const char* file, size_t *size)
{
	struct stat buf;
	int ret;

	if (!file)
		return -EINVAL;

	if (!size)
		return -EINVAL;

	ret = stat(file, &buf);
	if (ret < 0) {
		printf("%s is not exist\n", file);
		return ret;
	}

	*size = buf.st_size;
	return 0;
}

static void *guix_map_binres(const char* file)
{
	FILE* fp;
	char* addr;
	size_t size;

	if (!file)
		return NULL;

	if (file_get_size(file, &size) < 0)
		return NULL;

	if (size > FILE_SIZE_LIMIT) {
		printf("File(%s) is too large\n", file);
		return NULL;
	}

	addr = res_alloc(size);
	if (!addr) {
		printf("No more memory\n");
		return NULL;
	}

	fp = fopen(file, "rb");
	if (!fp) {
		printf("Open file(%s) failed\n", file);
		goto _free;
	}

	if (fread(addr, 1, size, fp) != size) {
		printf("Read file(%s) failed\n", file);
		goto _close;
	}

	fclose(fp);
	return addr;
_close:
	fclose(fp);
_free:
	free(addr);
	return NULL;
}

int guix_binres_load(const char *filename, int theme_id,
	struct GX_THEME_STRUCT** theme, struct GX_STRING_STRUCT*** language)
{
	void* root;
	UINT ret;

	root = guix_map_binres(filename);
	if (!root)
		return GX_FAILURE;

	/* Load gui resource */
	ret = gx_binres_language_table_load_ext(root, language);
	if (ret != GX_SUCCESS) {
		printf("%s load gui language resource failed\n", __func__);
		goto out;
	}

	ret = gx_binres_theme_load(root, theme_id, theme);
	if (ret != GX_SUCCESS) {
		printf("%s load gui theme resource failed\n", __func__);
		goto out;
	}

out:
	return ret;
}