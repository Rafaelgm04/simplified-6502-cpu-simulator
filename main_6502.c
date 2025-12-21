#include <stdio.h>
#include <stdlib.h>

#define MEM_MAXIMA (1024 * 64) //64 KB




typedef unsigned char byte; // 8 bits

typedef unsigned short word; //16 bits 





typedef struct
{
    byte data[MEM_MAXIMA];
    

}MEM;

void mem_write(MEM *mem, word address, byte value){
    if (address >= MEM_MAXIMA){
        printf("Erro: Acesso de memória fora dos limites: 0x%04X\n", address);
        exit(1);
    }
    mem->data[address] = value;
}

byte mem_read(MEM *mem, word address){
    if (address >= MEM_MAXIMA){
        printf("Erro: Acesso de memória fora dos limites: 0x%04X\n", address);
        exit(1);
    }
    return mem->data[address];
}

typedef struct
{
        /* Registradores */
    //PC = program couter e um resgistrador da cpu que guarda o endereço da proxima instruçao que sera executada
    word PC;

    //Stack Pointer  é um registrador da CPU que guarda a posição atual da pilha (stack) pilha = (uma área especial da memória usada para guardar informações temporárias)
    byte SP;


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

    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->SP = 0xFD;          // valor padrão da 6502 a pilha fica na página 0x0100–0x01FF
    cpu->P  = FLAG_U;        // bit não usado sempre ligado




}


// Busca o próximo opcode a ser executado
byte cpu_fetch(CPU *cpu, MEM *mem){
    byte opcode = mem_read(mem, cpu->PC); // Lê o byte na posição do PC
    cpu->PC++; // Incrementa o PC para apontar para a próxima instrução
    return opcode;
}

void cpu_execute(CPU *cpu, MEM *mem, byte opcode){
    // Implementação da execução do opcode
    switch(opcode){
        //controle basico de fluxo
        case 0xEA: // NOP - No Operation
        {
            // Não faz nada
        }
        break;
        
        case 0x00: // BRK - Break (interrupção)
        {
            printf("BRK executado. Encerrando a execucao.\n");
            exit(0);
        }
        break;
        
    //load immediate
        //vai ler o valor imediato e carregar no acumulador
        //antes ele passou pelo fetch e incrementou o PC para apontar para o próximo byte que é o valor imediato 
        case 0xA9: // LDA Immediate 
            {
                byte value = mem_read(mem, cpu->PC);
                cpu->A = value;
                // Atualiza flags
                if(cpu->A == 0){
                    cpu->P |= FLAG_Z; // Seta a flag Zero
                } else {
                    cpu->P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu->P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu->P &= ~FLAG_N; // Limpa a flag Negative
                }
                cpu->PC++; // Incrementa o PC após ler o valor imediato
            }
            break;
        
        

        
       
        case 0xA2: // LDX #imediato
            {
                cpu->X = mem_read(mem, cpu->PC);
                // Atualiza flags
                if(cpu->X == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->X & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
                cpu->PC++; // Incrementa o PC após ler o valor imediato
            }
            break;
        case 0xA0: // LDY #imediato
            {
                cpu->Y = mem_read(mem, cpu->PC);
                // Atualiza flags
                if(cpu->Y == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->Y & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
                cpu->PC++; // Incrementa o PC após ler o valor imediato
            }
            break;
        

        // Incremento / decremento
        case 0xE8: // INX
            {   
                cpu->X++;
                // Atualiza flags
                if(cpu->X == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->X & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
                
            }
            break;
        case 0xC8: // INY
            {
                cpu->Y++;
                // Atualiza flags
                if(cpu->Y == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->Y & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;
        case 0xCA: // DEX
            {
                cpu->X--;
                // Atualiza flags
                if(cpu->X == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->X & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;
        case 0x88: // DEY
            {
                cpu->Y--;
                // Atualiza flags
                if(cpu->Y == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->Y & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;

        // Transferência
        case 0xAA: // TAX
            {
                cpu->X = cpu->A;
                // Atualiza flags
                if(cpu->X == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->X & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;
        case 0x8A: // TXA
            {
                cpu->A = cpu->X;
                // Atualiza flags
                if(cpu->A == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;
        case 0xA8: // TAY
            {
                cpu->Y = cpu->A;
                // Atualiza flags
                if(cpu->Y == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->Y & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;
        case 0x98: // TYA
            {
                cpu->A = cpu->Y;
                // Atualiza flags
                if(cpu->A == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;

        // Jump
        case 0x4C: // JMP absoluto
            {   
                // le dois bytes da memoria e forma o endereço completo para onde o PC deve pular
                // mem_read(mem, cpu->PC) lê o byte menos significativo (LSB)
                // mem_read(mem, cpu->PC + 1) lê o byte mais significativo (MSB)
                // O MSB é deslocado 8 bits para a esquerda e combinado com o LSB usando o operador OR
                // O resultado é o endereço completo para onde o PC deve pular
                byte lo = mem_read(mem, cpu->PC++);
                byte hi = mem_read(mem, cpu->PC++);
                cpu->PC = (hi << 8) | lo;
            }
            break;

        // Branch
        case 0xF0: // BEQ
            {
                if(cpu->P & FLAG_Z){
                    byte offset = mem_read(mem, cpu->PC);
                    cpu->PC += (offset < 0x80) ? offset : offset - 0x100; // Ajusta o PC com o offset
                }
                cpu->PC++; // Incrementa o PC após ler o offset
            }
            break;
        case 0xD0: // BNE
            {
                if(!(cpu->P & FLAG_Z)){
                    byte offset = mem_read(mem, cpu->PC);
                    cpu->PC += (offset < 0x80) ? offset : offset - 0x100; // Ajusta o PC com o offset
                }
                cpu->PC++; // Incrementa o PC após ler o offset
            }
            break;

        // Stack
        case 0x48: // PHA
            {
                mem_write(mem, 0x0100 + cpu->SP, cpu->A);
                cpu->SP--;
            }
            break;
        case 0x68: // PLA
            {
                cpu->SP++;
                cpu->A = mem_read(mem, 0x0100 + cpu->SP);
                // Atualiza flags
                if(cpu->A == 0){
                    cpu-> P |= FLAG_Z; // Seta a flag Zero
                }else {
                    cpu-> P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu-> P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu-> P &= ~FLAG_N; // Limpa a flag Negative
                }
            }
            break;

        // Flags
        case 0x18: // CLC
            {
                cpu->P &= ~FLAG_C; // Limpa a flag Carry
            }
            break;
        case 0x38: // SEC
            {
                cpu->P |= FLAG_C; // Seta a flag Carry
            }
            break;

        // Aritmética
        case 0x69: // ADC imediato
            {
                byte valor = mem_read(mem, cpu->PC);
                word sum = (word)cpu->A + (word)valor + (word)(cpu->P & FLAG_C ? 1 : 0);
                
                // Atualiza flag Carry
                if(sum > 0xFF){
                    cpu->P |= FLAG_C; // Seta a flag Carry
                } else {
                    cpu->P &= ~FLAG_C; // Limpa a flag Carry
                }
                
                cpu->A = (byte)(sum & 0xFF);
                
                // Atualiza flags Zero e Negative
                if(cpu->A == 0){
                    cpu->P |= FLAG_Z; // Seta a flag Zero
                } else {
                    cpu->P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu->P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu->P &= ~FLAG_N; // Limpa a flag Negative
                }
                
                // Atualiza flag Overflow
                if(((cpu->A ^ valor) & 0x80) == 0 && ((cpu->A ^ (byte)(sum & 0xFF)) & 0x80) != 0){
                    cpu->P |= FLAG_V; // Seta a flag Overflow
                } else {
                    cpu->P &= ~FLAG_V; // Limpa a flag Overflow
                }
                
                cpu->PC++; // Incrementa o PC após ler o valor imediato
            }
            break;
        case 0xE9: // SBC imediato
            {
                byte valor = mem_read(mem, cpu->PC);
                word sub = (word)cpu->A - (word)valor - (word)(cpu->P & FLAG_C ? 0 : 1);
                
                // Atualiza flag Carry
                if(sub <= 0xFF){
                    cpu->P |= FLAG_C; // Seta a flag Carry
                } else {
                    cpu->P &= ~FLAG_C; // Limpa a flag Carry
                }

                cpu->A = (byte)(sub & 0xFF);

                // Atualiza flags Zero e Negative
                if(cpu->A == 0){
                    cpu->P |= FLAG_Z; // Seta a flag Zero
                } else {
                    cpu->P &= ~FLAG_Z; // Limpa a flag Zero
                }
                if(cpu->A & 0x80){
                    cpu->P |= FLAG_N; // Seta a flag Negative
                } else {
                    cpu->P &= ~FLAG_N; // Limpa a flag Negative
                }
                
                // Atualiza flag Overflow
                if(((cpu->A ^ valor) & 0x80) != 0 && ((cpu->A ^ (byte)(sub & 0xFF)) & 0x80) != 0){
                    cpu->P |= FLAG_V; // Seta a flag Overflow
                } else {
                    cpu->P &= ~FLAG_V; // Limpa a flag Overflow
                }
                
                cpu->PC++; // Incrementa o PC após ler o valor imediato
            }
            break;


        default:
        printf("Opcode desconhecido: 0x%02X\n", opcode);
        exit(1);
    }
}



int main(){
    CPU cpu; // CRIA A ESTRUTURA DA CPU
    MEM mem = {0}; // Inicializa toda a memória com zeros(MEMORIA RAM ZERADA)
    /*
    mem.data[0x8000] = 0xA9;
    mem.data[0x8001] = 0x01;
    
    mem.data[0xFFFC] = 0x00;
    mem.data[0xFFFD] = 0x80;
    */
    mem.data[0x8000] = 0xA9;
    mem.data[0x8001] = 0x01;
    mem.data[0x8002] = 0xAA;
    mem.data[0x8003] = 0xE8;
    mem.data[0x8004] = 0x69;
    mem.data[0x8005] = 0x02;
    mem.data[0x8006] = 0x00;

    mem.data[0xFFFC] = 0x00; // vetor de reset LSB
    mem.data[0xFFFD] = 0x80; // vetor de reset MSB

    cpu_reset(&cpu,&mem); //BOTA A CPU EM ESTADO DE RESET
    
    
    while(1){
        byte opcode = cpu_fetch(&cpu,&mem);
        cpu_execute(&cpu,&mem,opcode);
        printf("PC: %04X  A: %02X  X: %02X  Y: %02X  P: %02X  SP: %02X\n",
       cpu.PC, cpu.A, cpu.X, cpu.Y, cpu.P, cpu.SP);

    
    }
    return 0;
}