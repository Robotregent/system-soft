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

int _mod(int i){
    if (i<0)
        i+=MEM_SIZE;
    return i % MEM_SIZE;
}


address_mode setValue(int pc, instruction memory[], address_mode mode, int address_value, int value){
    address_mode result = mode;
    switch (mode) {
    case immediate:
        memory[pc].b_value = value;
        break;
    case direct:
        memory[_mod(pc + address_value)].b_value = value;
        break;
    case indirect:
        memory[_mod(pc + address_value + memory[_mod(pc + address_value)].b_value)].b_value = value;
        break;
    case decrement:
        if (memory[pc].b_add_mod == decrement){
            memory[_mod(pc + address_value)].b_value = _mod(memory[_mod(pc + address_value)].b_value - 1);
            memory[_mod(pc + address_value + memory[_mod(pc + address_value)].b_value)].b_value = value;
        }
        else {
            memory[_mod(pc + address_value)].a_value = _mod(memory[_mod(pc + address_value)].a_value - 1);
            memory[_mod(pc + address_value + memory[_mod(pc + address_value)].a_value)].a_value = value;
        }
        break;
    default:
        result = invalid_address_mode;
        break;
    }
    return result;
}
int getValue(int pc, instruction memory[], address_mode mode, int value){
    int result;
    switch (mode) {
    case immediate:
        result = value;
        break;
    case direct:
        result = memory[_mod(pc + value)].b_value;
        break;
    case indirect:
        result = memory[_mod(pc + value + memory[_mod(pc + value)].b_value)].b_value;
        break;
    case decrement:
        if (memory[pc].b_add_mod == decrement){
           memory[_mod(pc + value)].b_value = _mod(memory[_mod(pc + value)].b_value - 1);
           result = memory[_mod(pc + value + memory[_mod(pc + value)].b_value)].b_value; 
        }
        else{
           memory[_mod(pc + value)].a_value = _mod(memory[_mod(pc + value)].a_value - 1);
           result = memory[_mod(pc + value + memory[_mod(pc + value)].a_value)].a_value;
        }       
        break;
    default:
        result = (int) invalid_address_mode;
        break;
    }
    return result;
}

void dat(int pc, instruction memory[]){
    setValue(pc, memory, immediate, 0, _mod(memory[pc].b_value));
    printf("DAT #%d an Stelle: %d\n",memory[pc].b_value, pc);
}

void mov(int pc, instruction memory[]){
    int from , to;
    from = getValue(pc, memory, memory[pc].a_add_mod,memory[pc].a_value);
    to = getValue(pc, memory, memory[pc].b_add_mod,memory[pc].b_value);

    memory[to] = memory[from];

    printf("MOV von %d nach %d\n",from, to);
}

void add(int pc, instruction memory[]){
    int a, b, c;
    a = getValue(pc, memory, memory[pc].a_add_mod,memory[pc].a_value);
    b = getValue(pc, memory, memory[pc].b_add_mod,memory[pc].b_value);
    c = _mod(a + b);
    setValue(pc, memory, memory[pc].b_add_mod, memory[pc].b_value, c);

    printf("ADD %d + %d = %d\n",a, b, c);
}

void sub(int pc, instruction memory[]){
    int a, b, c;
    a = getValue(pc, memory, memory[pc].a_add_mod,memory[pc].a_value);
    b = getValue(pc, memory, memory[pc].b_add_mod,memory[pc].b_value);
    c = _mod(a - b);
    setValue(pc, memory, memory[pc].b_add_mod, memory[pc].b_value, c);

    printf("SUB %d - %d = %d\n",a, b, c);
}
void jmp(int *pc, instruction memory[]){
    *pc = _mod(getValue(*pc, memory, memory[*pc].a_add_mod,memory[*pc].a_value));
    printf("JMP to %d \n",*pc);
}
void jmz(int *pc, instruction memory[]){
    int check_value;
    if (memory[*pc].a_add_mod!=immediate){
        check_value = getValue(*pc, memory, memory[*pc].b_add_mod,memory[*pc].b_value);
        if (check_value == 0){
            *pc = _mod(getValue(*pc, memory, memory[*pc].a_add_mod,memory[*pc].a_value));
            printf("JMZ to %d \n",*pc);
        }
        else{
            printf("No JMZ. Value not zero.\n");
        }

    }else{
        printf("Invalid address_mode while trying JMZ\n");
    }
}
void jmg(int *pc, instruction memory[]){
    int check_value;
    if (memory[*pc].a_add_mod!=immediate){
        check_value = getValue(*pc, memory, memory[*pc].b_add_mod,memory[*pc].b_value);
        if (check_value > 0){
            *pc = _mod(getValue(*pc, memory, memory[*pc].a_add_mod,memory[*pc].a_value));
            printf("JMG to %d \n",*pc);
        }
        else{
            printf("No JMG. Value zero.\n");
        }

    }else{
        printf("Invalid address_mode while trying JMG\n");
    }

}
void djn(int *pc, instruction memory[]){
    int check_value;
    if (memory[*pc].a_add_mod!=immediate){
        check_value = getValue(*pc, memory, memory[*pc].b_add_mod,memory[*pc].b_value);
        check_value = _mod(check_value -1);
        setValue(*pc, memory, memory[*pc].b_add_mod, memory[*pc].b_value, check_value );
        if (check_value != 0){
            *pc = _mod(getValue(*pc, memory, memory[*pc].a_add_mod,memory[*pc].a_value));
            printf("DJN to %d \n",*pc);
        }
        else{
            printf("No DJN. Value zero.\n");
        }

    }else{
        printf("Invalid address_mode while trying DJN\n");
    }

}

void cmp(int *pc, instruction memory[]){
    int a, b, c;
    a = getValue(*pc, memory, memory[*pc].a_add_mod,memory[*pc].a_value);
    b = getValue(*pc, memory, memory[*pc].b_add_mod,memory[*pc].b_value);
    if (a == b)
        c = 2;
    else
        c = 1;
    *pc= _mod(*pc + c); 
    printf("CMP, Inc: %d\n",c );
}

opcode evaluate_instuktion(int pc, instruction memory[]){
    opcode result = memory[pc].opc;
    switch (result){
        case DAT:
            //dat(pc,memory);
            // verloren!!!
            break;
        case MOV:
            mov(pc,memory);
            break;
        case ADD:
            add(pc,memory);
            break;
        case SUB:
            sub(pc,memory);
            break;
        case JMP:
            jmp(&pc,memory);
            break;
        case JMZ:
            jmz(&pc,memory);
            break;
        case JMG:
            jmg(&pc,memory);
            break;
        case DJN:
            djn(&pc,memory);
            break;
        case CMP:
            cmp(&pc,memory);
            break;
        default:
            result  = invalid_opcode;
            break;
    }
    return result;
}


/**
 * Annahme: Eine Zeile == Eine Instruktion und ein blank als Trennzeichen 
 * for Schleife: Erstes Byte mit fgetc einlesen. Anhand dieses Opcodes die Struktur des restlichen Ausdrucks bestimmen 
 * */
int read_instructions(FILE* file, instruction player[], int pc){
    int i, opc;
    for(i=pc; i < (PROG_SIZE + pc) && ((opc=fgetc(file))>47) && opc < 58 ; i=_mod(i+1)) {
        opc -= 48;
        switch (opc){
        case 0:
            player[i].opc = 0;
            fscanf(file," %d\n", &player[i].b_value);
            break;
        case 4:
            fscanf(file," %d %d\n", &player[i].a_add_mod, &player[i].a_value);
            break;
        default:
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
