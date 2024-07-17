#include <stdio.h>
#include <string.h>

#include "rv.h"
#include "ch32v003fun.h"
const rv_u32 RAM_BASE = 0x20000100;
const rv_u32 RAM_END = 0x20000700;
const rv_u32 RAM_SIZE = (RAM_END - RAM_BASE);


rv_res bus_cb(void *user, rv_u32 addr, rv_u8 *data, rv_u32 is_store,
              rv_u32 width) {
  rv_u8 *mem = (rv_u8 *)user + addr - RAM_BASE;
  if (addr < RAM_BASE || addr + width >= RAM_BASE + RAM_SIZE)
    return RV_BAD;
  memcpy(is_store ? mem : data, is_store ? data : mem, width);
  return RV_OK;
}

rv_u32 program[2] = {
    /*            */             /* _start: */
    /* 0x80000000 */ 0x02A88893, /* add a7, a7, 42 */
    /* 0x80000004 */ 0x00000073  /* ecall */
};

int main(void) {
    SystemInit();
    printf("Hello, World!\n");
  rv_u8 mem[RAM_SIZE];
  rv cpu;
  rv_init(&cpu, (void *)mem, &bus_cb);
  memcpy((void *)mem, (void *)program, sizeof(program));
  while (rv_step(&cpu) != RV_EMECALL) {
  }
  printf("Environment call @ %08lX: %lu\n", cpu.csr.mepc, cpu.r[17]);
  return 0;
}
