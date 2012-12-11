#include <stdlib.h>
#include <stdbool.h>
#include "my_alloc.h"
#include "my_system.h"
    
typedef struct header{
    size_t size;    
    struct header *next;
    struct header *prev;
} header;


// Wurzelelement 
header root;
// Aktuelles Element des Rings gemäß circular first fit
header *node;

typedef long align;


void init_my_alloc() {
    root.next = &root;
    root.prev = &root;
    root.size = 0;
    node = &root;
}

void append_new(header *old, header *new){
    new->next = old->next;
    old->next = new;
    new->prev = old;
    new->next->prev = new;
}
void remove_used(header *n){
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->next = n->prev;
}
void insert_after_use(header *n){
    n->next = n->prev->next;
    n->prev->next = n;
    //if (n->next->prev == n->next->next)
    //    n->next->next = n;
    n->next->prev = n;
}

void* my_alloc(size_t size) {
    if (size <= 0)
        exit(1);

    void *result=0;
    // Richtet angeforderte Größe auf durch 8 teilbare Gößen aus
    // Sollte eigentlich laut Aufgabenstellung nicht notwendig sein
    if (size % sizeof(align))
        size += sizeof(align) - size % sizeof(align);

    header *current = node;

    // Suche ausreichend große freie Fläche
    do {
        if (current->size >= size)
            break;
        current = current->next;
    } while (current != node);

    // Situation:
    // Entweder Block gefunden, dann current->size >= size
    // oder eben nicht ;)

    if (current->size < size){
        // Kein ausreichend großer Block da.
        // Neuen Block anforder
        // Header erstellen
        // in den Ring freier Flächen einsortieren
        // current auf den neuen Header setzen
        void *block = get_block_from_system();
        if (!block)
            exit(1);
        header *new_header = (header *) block;
        new_header->size = BLOCKSIZE - sizeof(header);
        append_new(node, new_header);
        current = new_header;
    }

    // Situation:
    // current zeigt jetzt aufjedenfall auf ausreichend großen freien Block

    // Entweder ist der Block genau gleich groß, dann kann er nach Korrektur der Zeiger
    // direkt zurück gegeben werden.
    //
    // oder:
    //
    // Der Block ist zu groß, dann muss er von hinten verkleinert werden
    // -> Neuer Header new_H erstellen. 
    // -> header *new_header = (header *)((char *) current + current->size - size;
    // -> current->size -= size + sizeof(header);
    // -> return new_header + 1;

    //if (current->size == size)
     
    if (current->size < size + 2 * sizeof(header)){
        node = current->next;     // circular first fit
        remove_used(current);     // Block aus dem Ring freier Flächen entfernen

        if (current == node)
            node = &root;
        result = (void *) (current + 1);
    }
    else {
        header *new_header=(header *)((char *) current + current->size - size);
        result = (void *) (new_header + 1);
    }
    /*
    node = current;

    header *newnode = (header *)((char *)current + current->size - size);
    
    newnode->size = size;
    newnode->next = current;
    
    header *next = successor(current);

    current->size -= size + sizeof(header);
    return (void *)(newnode + 1);
    */

    return result;
}

void my_free(void* ptr) {
}
