#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "my_alloc.h"
#include "my_system.h"
#ifdef DEBUG
 #define D if(1) 
#else
 #define D if(0) 
#endif

/*
    Realisierung nach dem Circular-First-Fit Prinzip. 
    Orientiert an dem Beispiel aus der Vorlesung.  
    Verwaltungseinheit hat neben dem next Zeiger einen Zeiger auf das benachtbarte Element.
    Dieser Mechanismus wurde eingebaut, um einen Ring realisieren zu können auch wenn die 
    Blöcke von get_block_from_system nicht nebeneinander liegen.
    Folge: Großer Header und lange Suchzeiten. Buddy Verfahren wäre besser gewesen.

    Kann mit Debug-Ausgaben gebaut werden.
*/
    
typedef enum { is_free, is_alloc } alloc_flag;    
typedef struct header{
    size_t size;    
    struct header *next;
    struct header *succ; // zeigt auf die Fläche direkt rechts.
    alloc_flag flag;     // header muss 16 Byte groß sein, daher können die 4 Byte padding auch als Flag genutzt werden
} header;



header *root;           // Wurzelelement 
header *node;           // Aktuelles Element des Rings gemäß circular first fit
header *min_address;    // Ring aufteigend sortieren

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
    root->flag = is_free;
    node->size = BLOCKSIZE - 2 * sizeof(header);
    node->next = root;
    node->succ = root;
    node->flag = is_free; 
    min_address = root;
    printf("Root:\t %p\n",root);
    D printf("Node:\t %p\n",node);
    D printf("Min:\t %p\n",min_address);
}

// Fügt neuen Block in den Ring ein
header* append_new(header *new){
    if (new > min_address){
        D printf("Nicht absteigend!\n");
        exit (1);
    }
    //Aufsteigend sorieren 
    header *min_free = min_address;
    while (min_free->flag == is_alloc){
        min_free = min_free->succ;
    }
    header *max_free = min_free;
    while(max_free->next != min_free){
        max_free = max_free->next;
    }
    header *prev = min_address;
    while(prev->succ != min_address){
        prev = prev->succ;
    }

    // Allgemeiner Ring
    if (min_address->flag == is_alloc)
        min_address->next = new;
    prev->succ = new;
    new->succ = min_address;

    // Ring der freien Elemente
    new->next = min_free;
    max_free->next = new;

    min_address = new;

    new->flag = is_free;

    return max_free;
}

// Verkleinert freie Speicherfläche
void trim_block(header *old, header *new, int size){
    D printf("\nOld->size: %d size %d\n", old->size, size);
    new->next = old;
    new->succ = old->succ;
    if (old->succ->next == old && old->succ != root  && old->succ->flag == is_alloc)
        old->succ->next = new;
    old->succ = new;
    new->size = size;
    old->size -= size + sizeof(header);
    new->flag = is_alloc;    
}

// Entfernt Speicherfläche aus dem Ring freier Elemente
void remove_from_free_ring(header *prev_free, header *current){
    header *prev;
    prev_free->next = current->next;
    if (prev_free <  current){
        prev = prev_free;
        while (prev->succ < current){
            prev = prev->succ;
        }
    }
    else {
        prev = current->succ;
        while (prev->succ != current){
            prev = prev->succ;
        }
    }    
    current->next = prev;
    current->flag = is_alloc;
}
void* my_alloc(size_t size) {
    if (size <= 0)
        exit(1);
    D printf("Size: %d\n",size);
    void *result=0;
    // Richtet angeforderte Größe auf durch 8 teilbare Gößen aus
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
        D printf("Kein passender Block mehr. ");
        void *block = get_block_from_system();
        if (!block)
            exit(1);
        header *new_header = (header *) block;
        new_header->size = BLOCKSIZE - sizeof(header);
        prev = append_new(new_header);
        current = new_header;
        D printf("Neuer Block %p\n",current);    
    }

    if (current->size < size + 2 * sizeof(header)){
        D printf("Ganzer Block ");
        if (current == node)                        
            node = root;
        else
            node = current->next;                   // circular first fit
        remove_from_free_ring(prev,current);        // Block aus dem Ring freier Flächen entfernen
        result = (void *) (current + 1);
        D printf(" %p weg.\n",current);
    }
    else {
        D printf("Block %p verkleinern. Neu: ",current);
        node = current;              
        header *new_header=(header *)((char *) current + current->size - size);
        trim_block(current,new_header,size);
        result = (void *) (new_header + 1);
        D printf("%p\n",new_header);
    }
    D printf("\n");
    return result;
}
// Vereinigt benachtbarte Speicherflächen
int join_free(header* prev, header* current){
    header *n = (header *)((char*) prev + sizeof(header) + prev->size);
    // Nicht nebeneinander oder das root Element
    if (n!=current || prev == root )
        return 0;
    D printf("Join %p and %p\n",prev,current);
    prev->size += current->size + sizeof(header);
    prev->succ = current->succ;
    prev->next = current->next;
    if (current->succ->next == current)
        current->succ->next = prev;
    if (node == current)
        node = prev;
    
    return 1;
}
void my_free(void* current_ptr) {
    D printf("Free: %p, ptr %p\n",current_ptr-sizeof(header), current_ptr);
    header *current = (header *)((char *) current_ptr - sizeof(header));
    header *prev = current->next;
    while (prev->flag == is_alloc){
        prev = prev->next;
    }
    current->next = prev->next;
    prev->next = current;
    current->flag = is_free;

    join_free(current, current->next); 
    join_free(prev, current); 

    D printf("\n");
}