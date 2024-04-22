#ifndef EXC_STRING
#define EXC_STRING

#include <stddef.h>

typedef struct {
    char *begin;
    int   len;
} StringView;

#define EXC_SV_PRINTF_FMT "%.*s"
#define EXC_SV_PRINTF_ARGS(sv) (sv).len, (sv).begin

#define exc_sv_end(sv) ((sv).begin + (sv).len)
#define exc_sv_last(sv) (exc_sv_end(sv) - 1)

StringView exc_sv_from_cstr(char *str, int len);

StringView exc_sv_trim_space(StringView sv);
StringView exc_sv_trim_space_left(StringView sv);
StringView exc_sv_trim_space_right(StringView sv);

int exc_sv_index_of(StringView sv, char c);

void exc_sv_copy(void *dst, int dst_cap, StringView sv);

#endif // EXC_STRING

#ifdef EXC_STRING_IMPLEMENTATION

#include <ctype.h>
#include <string.h>

StringView exc_sv_from_cstr(char *begin, int len)
{
    if (len == 0) {
        len = strlen(begin);
    }
    return (StringView) { begin, len };
}

StringView exc_sv_trim_space(StringView sv)
{
    return exc_sv_trim_space_right(exc_sv_trim_space_left(sv));
}

StringView exc_sv_trim_space_left(StringView sv)
{
    StringView rval = { sv.begin, sv.len };
    while (rval.len > 0 && isspace(*rval.begin)) {
        ++rval.begin;
        --rval.len;
    }
    return rval;
}

StringView exc_sv_trim_space_right(StringView sv)
{
    StringView rval = { sv.begin, sv.len };
    while (rval.len > 0 && isspace(*exc_sv_last(rval))) {
        --rval.len;
    }
    return rval;
}

int exc_sv_index_of(StringView sv, char c)
{
    for (int i = 0; i < sv.len; i++) {
        if (sv.begin[i] == c) {
            return i;
        }
    }
    return -1;
}

void exc_sv_copy(void *dst, int dst_cap, StringView sv)
{
    int n = sv.len > dst_cap ? dst_cap : sv.len;
    memcpy(dst, sv.begin, n);
}

#endif //EXC_STRING_IMPLEMENTATION
