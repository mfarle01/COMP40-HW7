/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Implementation of readprog module
* ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <seq.h>
#include <mem.h>
#include <assert.h>
#include "bitpack.h"
#include "memory.h"
#include "instructions.h"
#include "readprog.h"



/* Reads um intructions from a open file by byte
 * bitpacks them in buig endian order into
 * uint32_t which are then loaded into segment 0 
 * PARAMS: takes a memory_t and an open file pointer
 * RETURNS: void*/
void read_prog(Memory_T mem, FILE *fp)
{
        uint32_t curr_word = 0;
        uint32_t counter = 0;
        int32_t ch = getc(fp);
        while (ch != EOF) {
                for (int i = 24; i >= 0; i -= 8) {
                        curr_word = Bitpack_newu(curr_word, 8, i, ch);
                        ch = getc(fp);
                        if (ch == EOF)
                                break; 
                }
                set_memory(mem, 0, counter, curr_word);
                if (ch == EOF)
                        break;
                //ch = getc(fp);
                counter++;
        }
}


/* does the instruction and unbitpacks it.
 * Then calls function based on the opcode using switch
 * statements to do the actual stuff
 * returns 0 if halt was called, 1 otherwise */
int do_instr(Memory_T mem, uint32_t inst)
{       
        unsigned opcode = Bitpack_getu(inst, 4, 28);
        int ra = 0, rb = 0, rc = 0;
        unsigned val = 0;
        if (opcode == 13) {
                ra = Bitpack_getu(inst, 3, 25);
                val = Bitpack_getu(inst, 25, 0);
                
        } else {
                ra = Bitpack_getu(inst, 3, 6);
                rb = Bitpack_getu(inst, 3, 3);
                rc = Bitpack_getu(inst, 3, 0);
        }
       
        int next = 1; 
        switch (opcode) {
                case 0:
                        cond_mov(mem, ra, rb, rc);
                        break;
                case 1:
                        seg_load(mem, ra, rb, rc);
                        break;
                case 2: 
                        seg_store(mem, ra, rb, rc);
                        break;
                case 3:
                        add(mem, ra, rb, rc);
                        break;
                case 4:
                        multiply(mem, ra, rb, rc);
                        break;
                case 5:
                        divide(mem, ra, rb, rc);
                        break;
                case 6:
                        nand(mem, ra, rb, rc);
                        break;
                case 7:
                        halt(mem);
                        next = 0;
                        break;
                case 8:
                        map_seg(mem, rb, rc);
                        break;
                case 9:
                        unmap_seg(mem, rc);
                        break;
                case 10:
                        output(mem, rc);
                        break;
                case 11:
                        input(mem, rc);
                        break;
                case 12:
                        load_prog(mem, rb, rc);
                        break;
                case 13:
                        load_val(mem, ra, val);
                        break;
        }
        return next;
}



