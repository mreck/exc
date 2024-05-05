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
