#include <../src/iodine_alloc.h>
#define STB_DS_IMPLEMENTATION
#define STBDS_REALLOC(context, ptr, size) iodrealloc(ptr, size)
#define STBDS_FREE(context, ptr) iodfree(ptr)
#include <../src/stb_ds.h>
