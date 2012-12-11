#include <stdlib.h>
#include <stdbool.h>
#include "my_alloc.h"
#include "my_system.h"
    
typedef struct header{
    size_t size;    
    struct header *next;
    struct header *child; // zeigt auf sich selbst, wenn Block belegt. Sonst 0 oder auf abgetrennte Flächen in diesem Block.
} header;


// Wurzelelement 
header root;
// Aktuelles Element des Rings gemäß circular first fit
header *node;

typedef long align;


void init_my_alloc() {
    root.next = &root;
    root.child = 0;
    root.size = 0;
    node = &root;
}

void append_new(header *old, header *new){
    new->next = old->next;
    old->next = new;
}
void remove_from_free_ring(header *prev, header *current){
    prev->next = current->next;
    while(prev->child!=0 && prev->child!=
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

    //if (current->size == size)
    if (current->size < size + 2 * sizeof(header)){
        node = current->next;     // circular first fit
        remove_used(current);     // Block aus dem Ring freier Flächen entfernen

        if (current == node)
            node = &root;
        result = (void *) (current + 1);
    }
    else {
        node = current;
        header *new_header=(header *)((char *) current + current->size - size);
        new_header->size = size;
        new_header->prev = current;
        new_header->next = current;
        current->size -= size + sizeof(header);
        result = (void *) (new_header + 1);
    }

    return result;
}

int join(header* prev, header* current){
    header *n = (header *)((char*) prev + sizeof(header) + prev->size);
    if (n!=current || prev->size == 0)
        return 0;

}

void my_free(void* ptr) {
}
