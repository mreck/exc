#ifndef EXC_TEST
#define EXC_TEST

#define __exc_expect_eq(fmt, a, b)                 \
        do {                                       \
            if ((a) != (b)) {                      \
                fprintf(stderr,                    \
                        "ERROR: %s:%d:\n"          \
                        "\texpected: %" #fmt "\n"  \
                        "\tactual:   %" #fmt "\n", \
                        __FILE__,                  \
                        __LINE__,                  \
                        (a),                       \
                        (b));                      \
            }                                      \
        } while (0)

#define exc_expect_eq_ptr(a, b) __exc_expect_eq(p, a, b)

#define exc_expect_eq_char(a, b) __exc_expect_eq(hhd, a, b)
#define exc_expect_eq_uchar(a, b) __exc_expect_eq(hhu, a, b)

#define exc_expect_eq_short(a, b) __exc_expect_eq(hd, a, b)
#define exc_expect_eq_ushort(a, b) __exc_expect_eq(hu, a, b)

#define exc_expect_eq_int(a, b) __exc_expect_eq(d, a, b)
#define exc_expect_eq_uint(a, b) __exc_expect_eq(u, a, b)

#define exc_expect_eq_long(a, b) __exc_expect_eq(l, a, b)
#define exc_expect_eq_ulong(a, b) __exc_expect_eq(lu, a, b)

#define exc_expect_eq_llong(a, b) __exc_expect_eq(ll, a, b)
#define exc_expect_eq_ullong(a, b) __exc_expect_eq(llu, a, b)

#define exc_expect_eq_float(a, b) __exc_expect_eq(f, a, b)
#define exc_expect_eq_double(a, b) __exc_expect_eq(f, a, b)

#endif // EXC_TEST
