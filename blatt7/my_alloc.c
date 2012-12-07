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
header *node;

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
    node = &root;
}

void* my_alloc(size_t size) {
    if (size <= 0)
        exit(1);
    //Align check
    
    header *prev = node; 
    header *current = prev->next;

    do {
        if (current->size >= size)
            break;
        prev = current;
        current = current->next;
    } while (current != node);

    if (current->size < size)
        return 0;

    if (current->size < size + 2 * sizeof(header)){
        node = current->next;
        prev->next = current->next;

        for (header *p = prev; p<current; p=successor(p)){
            prev = p;
        }
        current->next = prev;

        if (current == node)
            node = &root;
        return (void *) (current+1);
    }
    node = current;

    header *newnode = (header *)((char *)current + current->size - size);
    
    newnode->size = size;
    newnode->next = current;
    
    header *next = successor(current);

    // Irgendwas mit dynmem

    current->size -= size + sizeof(header);
    return (void *)(newnode + 1);
}

void my_free(void* ptr) {
}
