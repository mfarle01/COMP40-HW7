/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Implementation of instructions module
* ******************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <mem.h>
#include <assert.h>
#include "memory.h"
#include "instructions.h"

/* Copies value in rb into ra IF the value in rc is not 0 */
void cond_mov(Memory_T mem, int ra, int rb, int rc)
{
        if (get_register(mem, rc) != 0) {
                uint32_t temp = get_register(mem, rb);
                set_register(mem, ra, temp);
        }
}

/* Loads from memory a uint32_t word from segment $r[B] and offset $r[C]
 * into the register $r[A] */
void seg_load(Memory_T mem, int ra, int rb, int rc)
{
        set_register(mem, ra, get_memory(mem, get_register(mem, rb), 
                                              get_register(mem, rc)));
}

/* Stores a uint32_t word from r[C] to memory in segment $r[A] at offset $r[B]
 */
void seg_store(Memory_T mem, int ra, int rb, int rc)
{
        set_memory(mem, get_register(mem, ra),
                        get_register(mem, rb), get_register(mem, rc));
}

/* Adds values in rB and rC and puts result into rA */
void add(Memory_T mem, int ra, int rb, int rc)
{
        set_register(mem, ra, get_register(mem, rb) + get_register(mem, rc));
}

/* Multiplies the values in rB and rC and puts result into rA */
void multiply(Memory_T mem, int ra, int rb, int rc)
{
        set_register(mem, ra, get_register(mem, rb) * get_register(mem, rc));
}

/* Divides the value in rB by the value in rC and puts result in rA */
void divide(Memory_T mem, int ra, int rb, int rc)
{
        set_register(mem, ra, get_register(mem, rb) / get_register(mem, rc));

}

/* Bitwise NANDs the values in rB and rC and puts result in rA. */
void nand(Memory_T mem, int ra, int rb, int rc)
{
        set_register(mem, ra,  ~(get_register(mem, rb) & get_register(mem, rc)));
}

/* Does nothing.  When called by read_prog, should terminate execution */
void halt(Memory_T mem)
{
        (void)mem;        
}

/* Creates a new segment of length $r[C] and puts the newly mapped 
 * segment ID into $r[B] */
void map_seg(Memory_T mem, int rb, int rc)
{
        uint32_t id = map_segment(mem, get_register(mem, rc));
        set_register(mem, rb, id);
}

/* Unmaps the segment with the ID $r[C] */
void unmap_seg(Memory_T mem, int rc)
{
        unmap_segment(mem, get_register(mem, rc));
}

/* Prints a single character (0-255) to stdout */
void output(Memory_T mem, int rc)
{
        putc(get_register(mem, rc), stdout);        
}

/* Takes a single character from stdin, puts that value in r[C]  
 * If the end of input is signaled, puts all 1's into r[C] */
void input(Memory_T mem, int rc)
{
        int32_t in = getc(stdin);
        if (in == EOF) {
                set_register(mem, rc, ~0);
        } else {
                set_register(mem, rc, in);
        }
}

/* loads Program, puts register with id of $r[B] at segment 0, and sets
 * program counter to $r[C] as the offset of seg0..  */
void load_prog(Memory_T mem, int rb, int rc)
{
        mem_load_program(mem, get_register(mem, rb), get_register(mem, rc));
}

/* Loads a value between 0 and 2^25 and stores it in $r[A] */
void load_val(Memory_T mem, int ra, unsigned val)
{
        set_register(mem, ra, val);
}
