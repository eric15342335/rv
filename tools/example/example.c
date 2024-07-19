#include "rv.h"

#include <stdio.h>
#include <string.h>

#define RAM_BASE 0x80000000
#define RAM_SIZE 0x100
#include "ch32v003fun.h"

rv_res bus_cb(
    void * user, rv_u32 addr, rv_u8 * data, rv_u32 is_store, rv_u32 width);

/** @note Strange behaviour
 * There is n instructions in the program,
 * but the length of the program is n+1,
 * otherwise at the end of the program,
 * the following will happen:
 * @details
 * PC: 0, Trap: 2 (Illegal instruction)
 * PC: 0, Trap: 1 (Instruction fault)
 * ... // infinite loop
 * 7/19 update: maybe a problem with RAM_BASE or mem[RAM_BASE]
 */
rv_u16 program[RAM_SIZE/2] = {
    // description: multiply a0 by a1
    // load 5 into a0, 4 into a1
    // 0: c.li a0, 5
    0x4515,
    // 2: c.li a1, 4
    0x4591,
    // copy a0 to a2
    // 4: c.andi a2, 0
    0x8a01,
    // 6: c.or a2, a0
    0x8e49,
    // 8: c.addi a1, -1
    0x15fd,
    // 10: c.beqz a1, 8
    0xc581,
    // 12: c.addi a1, -1
    0x15fd,
    // 14: c.add a0, a2
    0x9532,
    // 16: c.j -6
    0xbfed,
    // 18: ecall
    0x0073
};

void display_all_registers(rv * cpu) {
    for (int i = 0; i < 32; i++) {
        if (cpu->r[i] != 0)
            printf("r%d: %d ", i, cpu->r[i]);
    }
    printf("\n");
}


int main(void) {
    SystemInit();
    printf("Hello, World!\n");
    rv_u8 mem[RAM_SIZE];
    rv cpu;
    rv_init(&cpu, (void *)mem, &bus_cb);
    memcpy((void *)mem, (void *)program, sizeof(program));
    while (1) {
        rv_u32 trap = rv_step(&cpu);
        printf("PC: %X, ", cpu.pc);
        printf("Trap: %X, ", trap);
        display_all_registers(&cpu);
        if (trap == RV_EMECALL)
            break;
        Delay_Ms(100);
    }
    printf("Environment call @ %X\n", cpu.csr.mepc);
    display_all_registers(&cpu);
    return 0;
}

rv_res bus_cb(
    void * user, rv_u32 addr, rv_u8 * data, rv_u32 is_store, rv_u32 width) {
    rv_u8 * mem = (rv_u8 *)user + addr - RAM_BASE;
    if (addr < RAM_BASE || addr + width >= RAM_BASE + RAM_SIZE)
        return RV_BAD;
    memcpy(is_store ? mem : data, is_store ? data : mem, width);
    return RV_OK;
}
