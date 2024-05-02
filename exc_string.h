#ifndef EXC_STRING
#define EXC_STRING

#include <stdbool.h>
#include <stddef.h>

#include "exc_container.h"

typedef struct {
    char *begin;
    int   len;
} EXC_String_View;

typedef struct {
    char *str;
    int   size;
    int   capacity;
} EXC_String_Builder;

#define EXC_SV_PRINTF_FMT "%.*s"
#define EXC_SV_PRINTF_ARGS(sv) (sv).len, (sv).begin

#define exc_sv_end(sv) ((sv).begin + (sv).len)
#define exc_sv_last(sv) (exc_sv_end(sv) - 1)
#define exc_sv_size(sv) ((((sv).begin) + (sv).len) - (sv).begin)

EXC_String_View exc_sv_from_cstr(char *str, int len);
EXC_String_View exc_sv_trim_space(EXC_String_View sv);
EXC_String_View exc_sv_trim_space_left(EXC_String_View sv);
EXC_String_View exc_sv_trim_space_right(EXC_String_View sv);
int             exc_sv_index_of(EXC_String_View sv, char c);
int             exc_sv_parse_llong(EXC_String_View sv, long long *n, int base);
int             exc_sv_parse_ullong(EXC_String_View sv, unsigned long long *n, int base);
void            exc_sv_copy(void *dst, int dst_cap, EXC_String_View sv, bool nullterm);
EXC_String_View exc_sv_copy_to_paged_buffer(EXC_Paged_Buffer *b, EXC_String_View sv);
void            exc_sb_free(EXC_String_Builder *b);
void            exc_sb_clear(EXC_String_Builder *b);
EXC_String_View exc_sb_append_cstr(EXC_String_Builder *b, char *s);
EXC_String_View exc_sb_append_string_view(EXC_String_Builder *b, EXC_String_View s);
void            exc_sb_copy(EXC_String_Builder *b, char *dst, bool nullterm);
EXC_String_View exc_sb_as_string_view(EXC_String_Builder *b);

#endif // EXC_STRING

#ifdef EXC_STRING_IMPLEMENTATION

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

EXC_String_View exc_sv_from_cstr(char *begin, int len)
{
    if (len < 1) {
        len = strlen(begin);
    }
    return (EXC_String_View) { begin, len };
}

EXC_String_View exc_sv_trim_space(EXC_String_View sv)
{
    return exc_sv_trim_space_right(exc_sv_trim_space_left(sv));
}

EXC_String_View exc_sv_trim_space_left(EXC_String_View sv)
{
    EXC_String_View rval = { sv.begin, sv.len };
    while (rval.len > 0 && isspace(*rval.begin)) {
        ++rval.begin;
        --rval.len;
    }
    return rval;
}

EXC_String_View exc_sv_trim_space_right(EXC_String_View sv)
{
    EXC_String_View rval = { sv.begin, sv.len };
    while (rval.len > 0 && isspace(*exc_sv_last(rval))) {
        --rval.len;
    }
    return rval;
}

int exc_sv_index_of(EXC_String_View sv, char c)
{
    for (int i = 0; i < sv.len; i++) {
        if (sv.begin[i] == c) {
            return i;
        }
    }
    return -1;
}

int exc_sv_parse_llong(EXC_String_View sv, long long *n, int base)
{
    *n = 0;

    sv = exc_sv_trim_space(sv);
    char buff[32];

    if ((unsigned long)exc_sv_size(sv) > (sizeof(buff) - 1)) {
        return 1;
    }

    exc_sv_copy(buff, sizeof(buff), sv, true);
    errno = 0;
    long long res = strtoll(buff, NULL, base);

    if (res == LLONG_MAX || res == LLONG_MIN || errno != 0) {
        return 1;
    }

    *n = res;
    return 0;
}

int exc_sv_parse_ullong(EXC_String_View sv, unsigned long long *n, int base)
{
    *n = 0;

    sv = exc_sv_trim_space(sv);
    char buff[32];

    if ((unsigned long)exc_sv_size(sv) > (sizeof(buff) - 1)) {
        return 1;
    }

    exc_sv_copy(buff, sizeof(buff), sv, true);
    errno = 0;
    unsigned long long res = strtoull(buff, NULL, base);

    if (res == ULLONG_MAX || errno != 0)  {
        return 1;
    }

    *n = res;
    return 0;
}

void exc_sv_copy(void *dst, int dst_cap, EXC_String_View sv, bool nullterm)
{
    int cap = nullterm ? dst_cap - 1 : dst_cap;
    int n = sv.len > cap ? cap : sv.len;
    memcpy(dst, sv.begin, n);
    if (nullterm) {
        ((char*)(dst))[n] = '\0';
    }
}

EXC_String_View exc_sv_copy_to_paged_buffer(EXC_Paged_Buffer *b, EXC_String_View sv)
{
    char *tmp = exc_pb_get(b, sv.len);
    if (tmp) {
        exc_sv_copy(tmp, sv.len, sv, false);
        return (EXC_String_View) {tmp, sv.len};
    }
    return (EXC_String_View) {NULL, 0};
}

void exc_sb_free(EXC_String_Builder *b)
{
    if (b->str != NULL) free(b->str);
    b->str = NULL;
    b->size = 0;
    b->capacity = 0;
}

void exc_sb_clear(EXC_String_Builder *b)
{
    if (b->size > 0) b->str[0] = '\0';
    b->size = 0;
}

EXC_String_View exc_sb_append_cstr(EXC_String_Builder *b, char *s)
{
    return exc_sb_append_string_view(b, exc_sv_from_cstr(s, 0));
}

EXC_String_View exc_sb_append_string_view(EXC_String_Builder *b, EXC_String_View s)
{
    int req_cap = b->size + s.len + 1;
    if (req_cap > b->capacity) {
        void *p = realloc(b->str, req_cap);
        if (!p) exit(ENOMEM);
        b->str = (char*)p;
        b->capacity = req_cap;
    }
    memcpy(b->str + b->size, s.begin, s.len);
    EXC_String_View sv = {b->str + b->size, s.len};
    b->size += s.len;
    b->str[b->size] = '\0';
    return sv;
}

void exc_sb_copy(EXC_String_Builder *b, char *dst, bool nullterm)
{
    memcpy(dst, b->str, b->size);
    if (nullterm) dst[b->size] = '\0';
}

EXC_String_View exc_sb_as_string_view(EXC_String_Builder *b)
{
    return (EXC_String_View) { b->str, b->size };
}

#endif //EXC_STRING_IMPLEMENTATION
