/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Header file for readprog Module
* ******************************/
#include <stdio.h>
#include "memory.h"
#include <stdint.h>


void read_prog(Memory_T mem, FILE *fp);
int do_instr(Memory_T mem, uint32_t inst);

