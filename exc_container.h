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

#ifdef EXC_CONTAINER_IMPLEMENTATION

#include <errno.h>
#include <unistd.h>
#include <string.h>

long exc_get_page_size()
{
    return sysconf(_SC_PAGE_SIZE);
}

char *exc_pb_get(EXC_Paged_Buffer *pbuf, long size)
{
    if (pbuf->pages == NULL) {
        long sz = exc_get_page_size();
        if (sz < 1) {
            return NULL;
        }
        memset(pbuf, 0, sizeof(*pbuf));
        pbuf->page_size = sz;

        size_t pages_size = sizeof(*pbuf->pages) * EXC_PAGED_BUFFER_INCREMENTS;
        pbuf->pages = (EXC_Paged_Buffer_Page*)malloc(pages_size);
        if (pbuf->pages == NULL) {
            exit(ENOMEM);
        }
        memset(pbuf->pages, 0, pages_size);
        pbuf->pages_count = EXC_PAGED_BUFFER_INCREMENTS;
    }

    if (size > pbuf->page_size) {
        return NULL;
    }

    EXC_Paged_Buffer_Page *page;

    for (int i = 0; i < pbuf->pages_count; i++) {
        page = pbuf->pages + i;

        if (size <= pbuf->page_size - page->used) {
            if (page->data == NULL) {
                page->data = (char*)malloc(pbuf->page_size);

                if (page->data == NULL) {
                    exit(ENOMEM);
                }
            }

            char *p = page->data + page->used;
            page->used += size;
            return p;
        }
    }

    long new_pages_count = pbuf->pages_count + EXC_PAGED_BUFFER_INCREMENTS;
    size_t pages_size = sizeof(*pbuf->pages) * new_pages_count;
    pbuf->pages = (EXC_Paged_Buffer_Page*)realloc(pbuf->pages, pages_size);
    if (pbuf->pages == NULL) {
        exit(ENOMEM);
    }
    memset(pbuf->pages + pbuf->pages_count, 0, sizeof(*pbuf->pages) * EXC_PAGED_BUFFER_INCREMENTS);
    pbuf->pages_count = new_pages_count;

    return exc_pb_get(pbuf, size);
}

void exc_pb_free(EXC_Paged_Buffer *pbuf)
{
    if (pbuf->pages != NULL) {
        for (int i = 0; i < pbuf->pages_count; i++) {
            if (pbuf->pages[i].data != NULL) {
                free(pbuf->pages[i].data);
                pbuf->pages[i].data = NULL;
            }
        }

        free(pbuf->pages);
    }

    memset(pbuf, 0, sizeof(*pbuf));
}

#endif // EXC_CONTAINER_IMPLEMENTATION
