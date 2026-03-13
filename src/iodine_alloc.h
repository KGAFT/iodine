#ifndef IODINE_ALLOC_H
#define IODINE_ALLOC_H

#ifndef iodmalloc
#include <stdlib.h>
#define iodmalloc(size) malloc(size)
#define iodrealloc(ptr, size) realloc(ptr, size)
#define iodfree(ptr) free(ptr)
#endif

#endif
