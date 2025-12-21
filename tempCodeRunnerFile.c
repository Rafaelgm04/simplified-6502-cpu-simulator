#include <stdio.h>
#include <stdlib.h>

#define MEM_MAXIMA (1024 * 64) //64 KB




typedef unsigned char byte; // 8 bits

typedef unsigned short word; //16 bits 





typedef struct
{
    byte data[MEM_MAXIMA];
    

}MEM;

typedef struct
{
    //PC = program couter e um resgistrador da cpu que guarda o endereço da proxima instruçao que sera executada
    word PC;

    //Stack Pointer  é um registrador da CPU que guarda a posição atual da pilha (stack) pilha = (uma área especial da memória usada para guardar informações temporárias)
    byte SP;
 
    /* Registradores */
    word PC;   // Program Counter (16 bits)
    byte SP;   // Stack Pointer (8 bits) → página 0x0100
    byte A;    // Acumulador
    byte X;    // Registrador X
    byte Y;    // Registrador Y

    
    /* Registradore de Status (P) */
    byte P;

}CPU;
/* ================= FLAGS ================= */
    /* para ligar a flag basta P = P | FLAG_C(a flag que deseja alterar)*/
    /* para disligar a flag basta P = P & ~FLAG_C(a flag que deseja alterar)*/
    
    #define FLAG_C 0x01  // Carry
    #define FLAG_Z 0x02  // Zero
    #define FLAG_I 0x04  // Interrupt Disable
    #define FLAG_D 0x08  // Decimal Mode
    #define FLAG_B 0x10  // Break
    #define FLAG_U 0x20  // Unused (sempre 1)
    #define FLAG_V 0x40  // Overflow
    #define FLAG_N 0x80  // Negative

void cpu_reset(CPU *cpu, MEM *mem){

    
    // Vetor de  reset (endereço inicial de execução) está localizado nos endereços 0xFFFC (LSB) e 0xFFFD (MSB)
    // LSB = menos significativo (menor endereço da memoria)
    // MSB = mais significativo (maioir endereço da memoria)
    //(mem->data[0xFFFD] << 8) le o endereço e delosca esse byte para parte alta do endereço
    //mem->data[0xFFFC] Lê o byte menos significativo (LSB) e 
    //Une os dois bytes: 0x8000 | 0x0000 = 0x8000 Esse é o endereço inicial do programa.
    cpu->PC = (word)(mem->data[0xFFFD] << 8) | mem->data[0xFFFC];

    cpu->SP = 0xFD; // valor inicial do stack pointer
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->P = FLAG_U; // seta a flag unused como 1



}


int main(){
    CPU cpu;
    MEM mem = {0};
    cpu_reset(&cpu,&mem);


    

    

    return 0;
}