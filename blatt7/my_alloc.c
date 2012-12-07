#include <stdlib.h>
#include <stdbool.h>
#include "my_alloc.h"
#include "my_system.h"

struct header{
    unsigned int size;
    struct header *next;
};
typedef struct header header;

// Wurzelelement 
header *root;
// Aktuelles Element des Rings gemäß circular first fit
header *node;

typedef long align;

/*
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
*/

void init_my_alloc() {
    root.next = &root;
    node = &root;
}

void* my_alloc(size_t size) {
    if (size <= 0)
        exit(1);

    // Richtet angeforderte Größe auf durch 8 teilbare Gößen aus
    // Sollte eigentlich laut Aufgabenstellung nicht notwendig sein
    if (size % sizeof(align))
        size += sizeof(align) - size % sizeof(align);

    header *prev = node; 
    header *current = prev->next;

    // Suche ausreichend große freie Fläche
    do {
        if (current->size >= size)
            break;
        prev = current;
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

    current->size -= size + sizeof(header);
    return (void *)(newnode + 1);
}

void my_free(void* ptr) {
}
