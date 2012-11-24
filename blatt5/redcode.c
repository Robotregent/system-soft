#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PROG_SIZE 256
#define MIN_PGRM_DIST 300
#define MEM_SIZE 8000

typedef enum { DAT, MOV, ADD, SUB, JMP, JMZ, JMG, DJN, CMP, invalid_opcode=-1} opcode;
typedef enum { immediate, direct, indirect, decrement, invalid_address_mode=-1} address_mode;

typedef struct {
    opcode opc;
    address_mode a_add_mod;
    int a_value;
    address_mode b_add_mod;
    int b_value;
} instruction;

int Inc(int i){
    if (i<0)
        i+=MEM_SIZE;
    return i % MEM_SIZE;
}
/**
 * Annahme: Eine Zeile == Eine Instuktion und ein blank als Trennzeichen 
 * for Schleife: Erstes Byte mit fgetc einlesen. Anhand dieses Opcodes die Struktur des restlichen Ausdrucks bestimmen 
 * */
int read_instructions(FILE* file, instruction player[], int pc){
    int i, opc;
    for(i=pc; i < (PROG_SIZE + pc) && ((opc=fgetc(file))>47) && opc < 58 ; i=Inc(i+1)) {
        opc -= 48;
        switch (opc){
        case 0:
            printf("DAT\n");
            player[i].opc = 0;
            fscanf(file," %d\n", &player[i].b_value);
            break;
        case 4:
            printf("JMP\n");
            fscanf(file," %d %d\n", &player[i].a_add_mod, &player[i].a_value);
            break;
        default:
            printf("REST\n");
            fscanf(file," %d %d %d %d\n", &player[i].a_add_mod, &player[i].a_value, &player[i].b_add_mod, &player[i].b_value);
            break;

        }
    }
    return i - pc;
}
/**
 * Die Programme sollen zu Begin mindestens 300 Instruktionen weit ausseinander liegen
 **/
void init_memory(int *pc_one, int *pc_two, FILE* file1, FILE* file2, instruction *memory){
    int end_player_one, end_player_two;

    srand(time(0));
    *pc_one = rand() % MEM_SIZE;
    
    end_player_one = (*pc_one + read_instructions(file1, memory, *pc_one)) % MEM_SIZE;
    printf("PC Player One: Start=%d Instructions=%d \n",*pc_one, end_player_one - *pc_one);

    do{
        *pc_two = rand() % MEM_SIZE;
    } while ((end_player_one + MIN_PGRM_DIST + PROG_SIZE) > *pc_two );

    end_player_two = (*pc_two + read_instructions(file2, memory, *pc_two)) % MEM_SIZE;
    printf("PC Player Two: Start=%d Instructions=%d \n",*pc_two, end_player_two - *pc_two);
}

int main(int argc, char** argv){
    if (argc < 3){
        printf("To few args\n");
        return -1;
    }
    FILE* file1 = fopen(argv[1],"r");
    FILE* file2 = fopen(argv[2],"r");
    if (file1==NULL || file2==NULL){
        printf("Can't open File\n");
        return -1;
    }
    int pc_one, pc_two;
    instruction memory[MEM_SIZE];

    init_memory(&pc_one, &pc_two, file1, file2, memory);

    fclose(file1);
    fclose(file2);
    return 0;
}
