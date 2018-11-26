/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Header file of instructions module
* ******************************/
#include <stdio.h>
#include "memory.h"
#include <stdint.h>

void cond_mov(Memory_T mem, int ra, int rb, int rc);
void seg_load(Memory_T mem, int ra, int rb, int rc);
void seg_store(Memory_T mem, int ra, int rb, int rc);
void add(Memory_T mem, int ra, int rb, int rc);
void multiply(Memory_T mem, int ra, int rb, int rc);
void divide(Memory_T mem, int ra, int rb, int rc);
void nand(Memory_T mem, int ra, int rb, int rc);
void map_seg(Memory_T mem, int rb, int rc);
void unmap_seg(Memory_T mem, int rc);
void output(Memory_T mem, int rc);
void input(Memory_T mem, int rc);
void load_prog(Memory_T mem, int rb, int rc);
void load_val(Memory_T mem, int ra, unsigned val);
void halt(Memory_T mem);


