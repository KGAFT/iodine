#include <iodine_alloc.h>
#define STB_DS_IMPLEMENTATION
#define STBDS_REALLOC(context, ptr, size) iodrealloc(ptr, size)
#define STBDS_FREE(context, ptr) iodfree(ptr)
#include <stb_ds.h>
