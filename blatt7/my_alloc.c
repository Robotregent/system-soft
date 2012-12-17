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
    
typedef enum { is_free, is_alloc } alloc_flag;    
typedef struct header{
    size_t size;    
    struct header *next;
    struct header *succ; // zeigt auf die Fläche direkt rechts.
    alloc_flag flag;
} header;


// Wurzelelement 
header *root;
// Aktuelles Element des Rings gemäß circular first fit
header *node;
header *min_address;

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
    D printf("Root:\t %p\n",root);
    D printf("Node:\t %p\n",node);
    D printf("Min:\t %p\n",min_address);
}
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

    if (min_free->flag == is_alloc){
        D printf("\nKein min_free %p ",min_free);
       exit(1);
    }
        
    D printf("\nmin_free %p ",min_free);
    header *max_free = min_free;
    while(max_free->next != min_free){
        max_free = max_free->next;
    }
    D printf("max_free %p \n",max_free);
    header *prev = min_address->succ;
    while(prev->succ != min_address){
        prev = prev->succ;
    }
    // for (header *p = prev; p != min_address; p = p->succ){
    //     prev = p;
    // }
    D printf("prev %p\n",prev);

    // Ring der freien Elemente
    new->next = min_free;
    max_free->next = new;

    // Allgemeiner Ring
    prev->succ = new;
    new->succ = min_address;
    
    //min_address->next = new; //?

    min_address = new;

    new->flag = is_free;

    return max_free;
}
void trim_block(header *old, header *new, int size){
    new->next = old;
    new->succ = old->succ;
    if (old->succ->next == old && old->succ != root  && old->succ->flag == is_alloc)
        old->succ->next = new;
    old->succ = new;
    new->size = size;
    old->size -= size + sizeof(header);
    new->flag = is_alloc;
    //printf("old->size: %d size %d succ %p\n", old->size, size,new->succ);
}
void remove_from_free_ring(header *prev_free, header *current){
    header *prev;
 //   if (current->next==prev_free)
 //       exit (1);
    prev_free->next = current->next;
    if (prev_free <  current){
        prev = prev_free;
        while (prev->succ < current){
            prev = prev->succ;
        }
        // for(header *p=prev_free->succ; p < current; p = p->succ){
        //     prev = p;
        // }
    }
    else {
        prev = current->succ;
        while (prev->succ != current){
            prev = prev->succ;
        }
        // for(header *p=prev; p != current; p = p->succ){
        //     prev = p;
        // }
    }
    
    current->next = prev;
    current->flag = is_alloc;
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
    D printf("Size: %d\n",size);
    void *result=0;
    // Richtet angeforderte Größe auf durch 8 teilbare Gößen aus
    // Sollte eigentlich laut Aufgabenstellung nicht notwendig sein
    if (size % sizeof(align))
        size += sizeof(align) - size % sizeof(align);
    
    header *prev = node;
    header *current = node->next;

    //D printf("node: %p\n",node);
    // Suche ausreichend große freie Fläche
    do {
        //D printf("Genug Platz? %p\n",current);
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
//        prev = root;                               // Einzige Möglichkeit. Update: Stimmt nicht!
        current = new_header;
        D printf("Neuer Block %p\n",current);    
    }
    //D printf("Zu nutzen %p\n",current);

    if (current->size < size +2 * sizeof(header)){
        D printf("Prev %p. Ganzer Block weg ",prev);

        if (current == node)                  // if (current->next == node)
            node = root;
        else
            node = current->next;                   // circular first fit
        remove_from_free_ring(prev,current);        // Block aus dem Ring freier Flächen entfernen
        result = (void *) (current + 1);
        D printf(" %p\n",current);
    }
    else {
        D printf("Block %p verkleinern ",current);
        node = current;               // ->next ist test:
        header *new_header=(header *)((char *) current + current->size - size);
        trim_block(current,new_header,size);
        result = (void *) (new_header + 1);
        D printf("%p\n",new_header);
    }
    D printf("\n");
    return result;
}

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
    if (current->flag == is_free){
        D printf("Not Allocated\n\n");
        exit(1);
    }
    header *prev = current->next;
    while (prev->flag == is_alloc){
        prev = prev->next;
    }
    //D  printf("prev %p\n",prev);
    D printf("prev %p flag %d\n",prev,prev->flag);
    current->next = prev->next;
    prev->next = current;
    current->flag = is_free;

    join_free(current, current->next); 
    join_free(prev, current); 

    //node = current;
    D printf("\n");
}

void print_ring(){
    header *p;
    char c;
    if (node->flag == is_alloc)
        c='U';
    else
        c='F';
    printf("\n%p %c n:%p ->",node,c,node->next);
    for (p=node->succ; p!=node; p=p->succ){
        if (p->flag == is_alloc)
            c='U';
        else
            c='F';
        printf("%p %c n:%p ->",p,c,p->next);
    }
    if (p->flag == is_alloc)
        c='U';
    else
        c='F';
    printf("%p %c n:%p \n",p,c,p->next);
}
