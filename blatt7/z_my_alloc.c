#include <stdlib.h>
#include <stdbool.h>
#include "my_alloc.h"
#include "my_system.h"
    
typedef struct header{
    size_t size;    
    struct header *next;
    struct header *succ; // zeigt auf die Fläche direkt rechts.
} header;


// Wurzelelement 
header *root;
// Aktuelles Element des Rings gemäß circular first fit
header *node;

typedef long align;


void init_my_alloc() {
    void *block = get_block_from_system();
    if (!block)
        exit(1);
    root = (header *) block;
    node = root + 1;
    root->size = 0;
    root->next = node;
    root->succ = node;
    node->size = BLOCKSIZE - 2 * sizeof(header);
    node->next = root;
    node->succ = root;
}
bool is_allocated(header* current) {
    /* Das Wurzelelement ist immer frei */
    if (current == root) 
        return false;
    /* Wenn der Zeiger vorwaerts zeigt, dann ist es frei */
    if (current->next > current) 
        return false;
    /* Der Zeiger zeigt jetzt rueckwaerts, denn
     *       ptr->next == ptr ist nur beim Wurzelelement moeglich */
    /* Wenn nicht auf Wurzel verwiesen wird, ist ptr belegt */
    if (current->next != root) 
        return true;
    /* Wenn ptr->next auf die Wurzel verweist, gibt es zwei
     * Faelle:
     *  - Es ist belegt und ptr liegt unmittelbar hinter
     *    dem Wurzelelement
     *  - Es ist frei und ptr zeigt auf die freie Speicherflaeche
     *    mit der hoechsten Adresse
     *                                   */
    /* Wenn root->next jenseits von ptr zeigt, dann ist ptr belegt */
    if (root->next > current) 
        return true;
    /* Wenn die Wurzel das einzige freie Element ist,
    *       dann ist ptr ebenfalls belegt */
    return root->next == root;
}
void append_new(header *old, header *new){
    
    //Aufsteigend sorieren fehlt noch

    new->next = old->next;
    old->next = new;
    new->succ = old->succ;
    old->succ = new;

}
void trim_block(header *old, header *new, int size){
    new->next = old;
    new->succ = old->succ;
    if (old->succ->next == old)
        old->succ->next = new;
    old->succ = new;
    new->size = size;
    old->size -= size + sizeof(header);
}
void remove_from_free_ring(header *prev_free, header *current){
    header *prev;
    prev_free->next = current->next;
    for(header *p; p < current; p = current->succ){
        prev = p;
    }
    current->next = prev;
}
/*
void insert_after_use(header *n){
    n->next = n->prev->next;
    n->prev->next = n;
    //if (n->next->prev == n->next->next)
    //    n->next->next = n;
    n->next->prev = n;
}
*/
void* my_alloc(size_t size) {
    if (size <= 0)
        exit(1);

    void *result=0;
    // Richtet angeforderte Größe auf durch 8 teilbare Gößen aus
    // Sollte eigentlich laut Aufgabenstellung nicht notwendig sein
    if (size % sizeof(align))
        size += sizeof(align) - size % sizeof(align);
    
    header *prev = node;
    header *current = node->next;

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
        void *block = get_block_from_system();
        if (!block)
            exit(1);
        header *new_header = (header *) block;
        new_header->size = BLOCKSIZE - sizeof(header);
        append_new(current, new_header);
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
        node = current;                             // circular first fit
        remove_from_free_ring(prev,current);        // Block aus dem Ring freier Flächen entfernen

        if (current == node)
            node = root;
        result = (void *) (current + 1);
    }
    else {
        node = current;
        header *new_header=(header *)((char *) current + current->size - size);
        trim_block(current,new_header,size);
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
