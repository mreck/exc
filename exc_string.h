#ifndef EXC_STRING
#define EXC_STRING

#include <stddef.h>

typedef struct {
    char *begin;
    int   len;
} EXC_String_View;

#define EXC_SV_PRINTF_FMT "%.*s"
#define EXC_SV_PRINTF_ARGS(sv) (sv).len, (sv).begin

#define exc_sv_end(sv) ((sv).begin + (sv).len)
#define exc_sv_last(sv) (exc_sv_end(sv) - 1)

EXC_String_View exc_sv_from_cstr(char *str, int len);

EXC_String_View exc_sv_trim_space(EXC_String_View sv);
EXC_String_View exc_sv_trim_space_left(EXC_String_View sv);
EXC_String_View exc_sv_trim_space_right(EXC_String_View sv);

int exc_sv_index_of(EXC_String_View sv, char c);

void exc_sv_copy(void *dst, int dst_cap, EXC_String_View sv);

#endif // EXC_STRING

#ifdef EXC_STRING_IMPLEMENTATION

#include <ctype.h>
#include <string.h>

EXC_String_View exc_sv_from_cstr(char *begin, int len)
{
    if (len == 0) {
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

void exc_sv_copy(void *dst, int dst_cap, EXC_String_View sv)
{
    int n = sv.len > dst_cap ? dst_cap : sv.len;
    memcpy(dst, sv.begin, n);
}

#endif //EXC_STRING_IMPLEMENTATION
