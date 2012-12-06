#include <stdlib.h>
#include <stdbool.h>
#include "my_alloc.h"
#include "my_system.h"

struct header{
    unsigned int size;
    struct header *next;
};
typedef struct header header;

header root = {0,0};

bool is_allocated(header *memarea){
    if (memarea == &root)
        return false;
    if (memarea->next > memarea)
        return false;
    if (memarea->next != &root)
        return true;
    if (root.next > memarea)
        return true;
    return root.next == &root;
}

header *successor(header *memarea){
    return (header *) ((char *)(memarea+1)+memarea->size);
}

void init_my_alloc() {
    root.next = &root;
}

void* my_alloc(size_t size) {
    return get_block_from_system();
}

void my_free(void* ptr) {
}
