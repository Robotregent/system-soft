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


int get_instuction_ptr_a(int pc, instruction memory[]){
    address_mode mode =  memory[pc].a_add_mod;
    int result, value = memory[pc].a_value;

    switch (mode) {
        case immediate:
            printf("Addressing of immediate is undefined\n");
            result = (int) invalid_address_mode;
            break;
        case direct:
            result = _mod(pc + value);
            break;
        case indirect:
            result = _mod(pc + value + memory[_mod(pc + value)].a_value);
            break;
        case decrement:
            memory[_mod(pc + value)].a_value = _mod(memory[_mod(pc + value)].a_value - 1);
            result = _mod(memory[_mod(pc + value + memory[_mod(pc + value)].a_value)].a_value);
            break;
        default:
            result = (int) invalid_address_mode;
            break;
        }
    return result;
}

int get_instuction_ptr_b(int pc, instruction memory[]){
    address_mode mode =  memory[pc].b_add_mod;
    int result, value = memory[pc].b_value;
    switch (mode) {
        case immediate:
            printf("Addressing of immediate is undefined\n");
            result = (int) invalid_address_mode;
            break;
        case direct:
            result = _mod(pc + value);
            break;
        case indirect:
            result = _mod(pc + value + memory[_mod(pc + value)].b_value);
            break;
        case decrement:
            memory[_mod(pc + value)].b_value = _mod(memory[_mod(pc + value)].b_value - 1);
            result = _mod(memory[_mod(pc + value + memory[_mod(pc + value)].b_value)].b_value);
            break;
        default:
            result = (int) invalid_address_mode;
            break;
        }
    return result;
}

//Setzt a_value
address_mode set_a_value(int pc, instruction memory[], int value){
    if (memory[pc].a_add_mod!=immediate){
        int at = get_instuction_ptr_a(pc,memory);
        memory[at].a_value = _mod(value);
    }
    else{
        memory[pc].a_value = _mod(value);
    }
    return memory[pc].a_add_mod;
}
//Setzte  b_value
address_mode set_b_value(int pc, instruction memory[], int value){
    if (memory[pc].b_add_mod!=immediate){
        int at = get_instuction_ptr_b(pc,memory);
        memory[at].b_value = _mod(value);
    }
    else{
        memory[pc].b_value = _mod(value);
    }
    return memory[pc].b_add_mod;
}

// Hole a_value
int get_a_value(int pc, instruction memory[]){
    int result;
    if (memory[pc].a_add_mod!=immediate){
        int at = get_instuction_ptr_a(pc,memory);
        result = memory[at].a_value;
    }
    else{
        result = memory[pc].a_value;;
    }
    return _mod(result);
}
// Hole b_value
int get_b_value(int pc, instruction memory[]){
    int result;
    if (memory[pc].b_add_mod!=immediate){
        int at = get_instuction_ptr_b(pc,memory);
        result = memory[at].b_value;
    }
    else{
        result = memory[pc].b_value;;
    }
    return _mod(result);
}


void dat(int pc, instruction memory[]){
    set_b_value(pc, memory, 0);
    printf("DAT #%d an Stelle: %d\n",memory[pc].b_value, pc);
}

void mov(int pc, instruction memory[]){
    int from , to;
    to = get_instuction_ptr_b(pc,memory);
    if(memory[pc].a_add_mod==immediate){
    	memory[to].opc = DAT;
    	memory[to].a_add_mod = invalid_address_mode;
    	memory[to].a_value = 0;
    	memory[to].b_add_mod = immediate;
    	memory[to].b_value = _mod(memory[pc].a_value);
    	printf("MOV %d nach %d\n",_mod(memory[pc].a_value), to);
    }
    else{
    	from = get_instuction_ptr_a(pc,memory);
    	memory[to]=memory[from];
    	printf("MOV von %d nach %d\n",from, to);
    }
}

void add(int pc, instruction memory[]){
    int a, b, c;
    a = get_a_value(pc, memory);
    b = get_b_value(pc, memory);
    c = _mod(a + b);
    set_b_value(pc, memory, c);

    printf("ADD %d + %d = %d\n",a, b, c);
}

void sub(int pc, instruction memory[]){
    int a, b, c;
    a = get_a_value(pc, memory);
    b = get_b_value(pc, memory);
    c = _mod(a - b);
    set_b_value(pc, memory, c);

    printf("SUB %d - %d = %d\n",a, b, c);
}
void jmp(int *pc, instruction memory[]){
    *pc = get_instuction_ptr_a(*pc,memory);
    printf("JMP to %d \n",*pc);
}
void jmz(int *pc, instruction memory[]){
    int check_value;
    if (memory[*pc].a_add_mod!=immediate){
        check_value = get_b_value(*pc, memory);
        if (check_value == 0){
            *pc = get_instuction_ptr_a(*pc,memory);
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
        check_value = get_b_value(*pc, memory);
        if (check_value > 0){
            *pc = get_instuction_ptr_a(*pc,memory);
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
        check_value = get_b_value(*pc, memory);
        check_value = _mod(check_value - 1);
        set_b_value(*pc, memory, check_value );
        if (check_value != 0){
            *pc = get_instuction_ptr_a(*pc,memory);
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
    a = get_a_value(*pc, memory);
    b = get_b_value(*pc, memory);
    if (a == b)
        c = 2;
    else
        c = 1;
    *pc= _mod(*pc + c);
    printf("CMP, Inc: %d\n",c );
}

opcode evaluate_instuktion(int *pc, instruction memory[]){
    opcode result = memory[*pc].opc;
    switch (result){
        case DAT:
            // verloren!!!
            dat(*pc,memory);
            break;
        case MOV:
            mov(*pc,memory);
            break;
        case ADD:
            add(*pc,memory);
            break;
        case SUB:
            sub(*pc,memory);
            break;
        case JMP:
            jmp(pc,memory);
            break;
        case JMZ:
            jmz(pc,memory);
            break;
        case JMG:
            jmg(pc,memory);
            break;
        case DJN:
            djn(pc,memory);
            break;
        case CMP:
            cmp(pc,memory);
            break;
        default:
            result  = invalid_opcode;
            break;
    }
    if (result>0&&result<4)
        *pc = _mod(*pc + 1);
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
            fscanf(file," %d\n", &player[i].b_value);
            break;
        case 4:
            fscanf(file," %d %d\n", &player[i].a_add_mod, &player[i].a_value);
            break;
        default:
            fscanf(file," %d %d %d %d\n", &player[i].a_add_mod, &player[i].a_value, &player[i].b_add_mod, &player[i].b_value);
            break;

        }
        player[i].opc = opc;
    }
    return i - pc;
}

void ignore_DAT(int* pc, instruction memory[]){
    while(memory[*pc].opc == DAT)
        *pc = *pc + 1;
}
/**
 * Die Programme sollen zu Begin mindestens 300 Instruktionen weit ausseinander liegen
 **/
void init_memory(int *pc_one, int *pc_two, FILE* file1, FILE* file2, instruction *memory){
    int end_player_one, end_player_two, i;
    instruction empty = {DAT, invalid_address_mode, 0, immediate, 0};

    for (i=0; i < MEM_SIZE; i++){
        memory[i]=empty;
    }

    srand(time(0));
    *pc_one = rand() % MEM_SIZE;

    end_player_one = (*pc_one + read_instructions(file1, memory, *pc_one)) % MEM_SIZE;
    printf("PC Player One: Start=%d Instructions=%d \n",*pc_one, end_player_one - *pc_one);

    do{
        *pc_two = rand() % MEM_SIZE;
    } while ((end_player_one + MIN_PGRM_DIST + PROG_SIZE) > *pc_two );

    end_player_two = (*pc_two + read_instructions(file2, memory, *pc_two)) % MEM_SIZE;
    printf("PC Player Two: Start=%d Instructions=%d \n",*pc_two, end_player_two - *pc_two);

    // setzte PC auf erste nicht DAT Anweisung
    ignore_DAT(pc_one,memory);
    ignore_DAT(pc_two,memory);
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
    opcode current_opc_one, current_opc_two;
    instruction memory[MEM_SIZE];

    init_memory(&pc_one, &pc_two, file1, file2, memory);

    //int i=0;
    //Eventloop
    do{
    	printf("Programm one (%04d):\t", pc_one);
        current_opc_one = evaluate_instuktion(&pc_one,memory);

        printf("Programm two (%04d):\t", pc_two);
        current_opc_two = evaluate_instuktion(&pc_two,memory);
        //if (i++> 2 * 28 -1)
        //    return 1;
        //current_opc_two =1;

    } while (current_opc_one && current_opc_two);
    
    if (current_opc_one)
        printf("Programm one wins\n");
    else 
        printf("Programm two wins\n");

    fclose(file1);
    fclose(file2);
    return 0;
}
