#ifndef EXC_CONTAINER
#define EXC_CONTAINER

#ifndef EXC_ARRAY_LIST_GROWTH_FACTOR
#define EXC_ARRAY_LIST_GROWTH_FACTOR 1.4f
#endif

#ifndef EXC_ARRAY_LIST_INITIAL_CAPACITY
#define EXC_ARRAY_LIST_INITIAL_CAPACITY 4
#endif

// typedef struct {
//     char *data;
//     int size;
//     int capacity;
// } Char_Array_List_Example;

#define exc_array_list_resize(a, n)                               \
    do {                                                          \
        void *p = realloc((a)->data, sizeof(*((a)->data)) * (n)); \
        if (!p) exit(ENOMEM);                                     \
        (a)->data = p;                                            \
        (a)->capacity = (n);                                      \
    } while (0)

#define exc_array_list_push(a, v)                                   \
    do {                                                            \
        if ((a)->size >= (a)->capacity) {                           \
            int cap = (a)->capacity * EXC_ARRAY_LIST_GROWTH_FACTOR; \
            if (cap == 0) cap = EXC_ARRAY_LIST_INITIAL_CAPACITY;     \
            exc_array_list_resize((a), cap);                        \
        }                                                           \
        (a)->data[(a)->size++] = (v);                               \
    } while (0)

#define exc_array_list_pop(a) ((a)->data[--(a)->size])

#ifndef EXC_PAGED_BUFFER_INCREMENTS
#define EXC_PAGED_BUFFER_INCREMENTS 4
#endif

typedef struct {
    char *data;
    long used;
} EXC_Paged_Buffer_Page;

typedef struct {
    long page_size;
    int pages_count;
    EXC_Paged_Buffer_Page *pages;
} EXC_Paged_Buffer;

long exc_get_page_size();

char *exc_pb_get(EXC_Paged_Buffer *pbuf, long size);
void  exc_pb_free(EXC_Paged_Buffer *pbuf);

#endif // EXC_CONTAINER
