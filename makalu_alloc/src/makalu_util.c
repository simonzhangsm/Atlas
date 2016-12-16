/*
 * Source code is partially derived from Boehm-Demers-Weiser Garbage 
 * Collector (BDWGC) version 7.2 (license is attached)
 *
 * File:
 *   misc.c
 */

#include "makalu_internal.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h> 


MAK_INLINE int MAK_write(FILE *f, const char *buf, size_t len)
{
    int res = fwrite(buf, 1, len, f);
    fflush(f);
    return res;
}


MAK_INNER void MAK_abort(const char *msg)
{
    if (MAK_write(stderr, (void *)msg, strlen(msg)) >= 0)
        (void)MAK_write(stderr, (void *)("\n"), 1);

    (void) abort();
}


void MAK_err_printf(const char *format, ...)
{
    va_list args;
    char buf[BUFSZ+1];

    va_start(args, format);
    buf[BUFSZ] = 0x15;

    (void) vsnprintf(buf, BUFSZ, format, args);
    va_end(args);
    if (buf[BUFSZ] != 0x15) ABORT("MAK_printf clobbered stack");
    if (MAK_write(stderr, buf, strlen(buf)) < 0)
      ABORT("write to stderr failed");
    
}

/* A version of printf that is unlikely to call malloc, and is thus safer */
/* to call from the collector in case malloc has been bound to MAK_malloc. */
/* Floating point arguments and formats should be avoided, since fp       */
/* conversion is more likely to allocate.                                 */
/* Assumes that no more than BUFSZ-1 characters are written at once.      */
void MAK_printf(const char *format, ...)
{
    va_list args;
    char buf[BUFSZ+1];

    va_start(args, format);
    buf[BUFSZ] = 0x15;
    (void) vsnprintf(buf, BUFSZ, format, args);
    va_end(args);
    if (buf[BUFSZ] != 0x15) ABORT("MAK_printf clobbered stack");
    if (MAK_write(stdout, buf, strlen(buf)) < 0)
      ABORT("write to stdout failed");
}


MAK_INNER void MAK_warn_proc(char* msg, MAK_word arg){
    MAK_err_printf(msg, arg);
}

