/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Header file for Memory ADT
* ******************************/
#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

#include <stdio.h>
#include <seq.h>
#include <stdint.h>
#define T Memory_T
typedef struct T *T;


/* Initializes and frees the Memory struct */
extern T Memory_init(uint32_t);
void Memory_free(T *);

/* Returns word from the provided */
uint32_t get_register(T, int); 
/* Sets a register to the given value */
void set_register(T, int, uint32_t);

/* Gets word from memory based on segment_id and offset */
uint32_t get_memory(T, uint32_t, uint32_t);
/* Sets word in memory from segment_id and offset, val */
void set_memory(T, uint32_t, uint32_t, uint32_t);

/* Creates/Allocates new segment, gets length of new segment from input */
uint32_t map_segment(T, uint32_t);
/* Frees the segments that has the seg_id inputted */
void unmap_segment(T, uint32_t);

/* Returns word pointed to by the program counter */
uint32_t get_next_instr(T);

/* Moves a new segment to Segment 0 and frees the old seg0.  
 * Moves the program ctr to the correct new place */
void mem_load_program(T, uint32_t, uint32_t);


/* HELPER FUNCTIONS FOR TESTING PURPOSES */
void print_registers(T);
void print_segments(T);

#undef T
#endif
