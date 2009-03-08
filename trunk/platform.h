/* all platform-specific includes and defines go in this file */
#ifndef PLATFORM_H
#define PLATFORM_H

/* configurable options */
#define UNIX_HOST                           /* select your host type */
#undef  FLYINGFOX_HOST
#undef  SURVEYOR_HOST

#ifndef SURVEYOR_HOST
#define HEAP_SIZE 16384                     /* space for the heap and the stack */
#endif
#define LARGE_INT_POWER_OF_TEN 1000000000   /* the largest power of ten which fits in an int on this architecture */
#define ARCH_ALIGN_WORDSIZE sizeof(int)     /* memory alignment boundary on this architecture */

#define GLOBAL_TABLE_SIZE 397               /* global variable table */
#define STRING_TABLE_SIZE 97                /* shared string table size */
#define PARAMETER_MAX 16                    /* maximum number of parameters to a function */
#define LINEBUFFER_MAX 256                  /* maximum number of characters on a line */
#define LOCAL_TABLE_SIZE 11                 /* size of local variable table (can expand) */
#define STRUCT_TABLE_SIZE 11                /* size of struct/union member table (can expand) */

#define INTERACTIVE_PROMPT_START "starting picoc\n"
#define INTERACTIVE_PROMPT_STATEMENT "c> "
#define INTERACTIVE_PROMPT_LINE " > "

/* host platform includes */
#ifdef UNIX_HOST
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <setjmp.h>
#ifndef NO_FP
#include <math.h>
#endif

#else
# ifdef FLYINGFOX_HOST

# else
#  ifdef SURVEYOR_HOST
#   define NO_FP
#   define NO_CTYPE
#   include <cdefBF537.h>
#   include "../string.h"
#   include "../print.h"
#   include "../srv.h"
#   include "../setjmp.h"
#   include "../stdarg.h"
#   include "../colors.h"
#   include "../neural.h"
#   include "../i2c.h"
#   define assert(x)
#  endif
# endif
#endif

#endif /* PLATFORM_H */

