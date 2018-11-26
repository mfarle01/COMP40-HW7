/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Implementation of memory_t ADT
* ******************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <mem.h>
#include <seq.h>
#include <uarray.h>
#include "memory.h"

#define T Memory_T
#define num_registers 8
#define word_size 32

/* Memory_T struct definition*/
struct Memory_T {
        UArray_T registers;
        Seq_T unmapped_ids;
        Seq_T memory;
        uint32_t program_ctr;
};


/* Initialize Memory struct 
 * PARAMS: takes a unit32_t for prog_len
 * RETURNS: Memory_T */
T Memory_init(uint32_t prog_len)
{
        Memory_T x = NEW(x);
        x->registers = UArray_new(num_registers, word_size);
        x->unmapped_ids = Seq_new(10);
        x->memory = Seq_new(10);
        
        UArray_T seg0 = UArray_new(prog_len, word_size);
        Seq_addlo(x->memory, seg0);
        x->program_ctr = 0;
        return x;
}

/* Frees Memory struct 
 * PARAMS: takes a pointer to a memory_t
 * RETURNS: void*/
void Memory_free(T *mem)
{
        assert(mem && *mem);
        while (Seq_length((*mem)->memory) > 0) {
                UArray_T temp = Seq_remlo((*mem)->memory);
                if (temp != NULL) {
                        UArray_free(&temp);
                }
        }
        Seq_free(&((*mem)->memory) );
        Seq_free(&((*mem)->unmapped_ids) );
        UArray_free(&((*mem)->registers) );
        FREE(*mem);
}

/* Returns word from the provided
 * PARAMS: takes a memory_t and a int representing the register
 * RETURNS: unit32_t for whats in that register*/
uint32_t get_register(T mem, int reg)
{
        assert(mem); 
        return *((uint32_t *)(UArray_at(mem->registers, reg) ) ) ;
}
/* Sets a register to the given value  
 * PARAMS: takes a memory_t, an int for the register and 
 * a unit32_t to load in that register
 * RETURNS: void*/
void set_register(T mem, int reg, uint32_t val)
{         
        assert(mem);
        *((uint32_t *)(UArray_at(mem->registers, reg) ) ) = val;
}       

/* Gets word from memory based on segment_id and offset 
 * PARAMS: takes a memory_T, unit32_t for seg and a uint32_t for offset
 * RETURNS: uint32_t at the segment and offset provided*/
uint32_t get_memory(T mem, uint32_t seg, uint32_t offset)
{
        assert(mem);
        UArray_T temp = (UArray_T)Seq_get(mem->memory, seg);
        return *((uint32_t *)(UArray_at(temp, offset)) );
}

/* Sets word in memory from segment_id and offset 
 * PARAMS: takes a memory_t, unit32_t for segment, unit32_t for offset,
 * and a unit32_t for the value
 * RETURNS: void*/
void set_memory(T mem, uint32_t seg, uint32_t offset, uint32_t val)
{
        assert(mem);
        UArray_T temp = (UArray_T)Seq_get(mem->memory, seg);
        *((uint32_t *)(UArray_at(temp, offset)) ) = val;
}



/* Creates/Allocates new segment, gets length of new segment from input 
 * If there's some unmapped IDs, use that first
 * Returns the ID of the just allocated segment */
uint32_t map_segment(T mem, uint32_t length)
{
        assert(mem);
        UArray_T temp = UArray_new(length, word_size);
        if (Seq_length(mem->unmapped_ids) > 0) {
                uint32_t id = (uint32_t)(uintptr_t)Seq_remlo(mem->unmapped_ids);
                temp = Seq_put(mem->memory, id, temp);
                return id;
        }
        temp = Seq_addhi(mem->memory, temp);
        return Seq_length(mem->memory) - 1;
}



/* Frees the segments that has the seg_id inputted  
 * PARAMS: takes a memory_t and uint32_t for the segid
 * RETURNS: void */
void unmap_segment(T mem, uint32_t segid)
{
        assert(mem);
        UArray_T temp = (UArray_T)Seq_get(mem->memory, segid);
        UArray_free(&temp);
        temp = Seq_put(mem->memory, segid, NULL);
        void *id = Seq_addhi(mem->unmapped_ids, (void *)(uintptr_t)segid);
        (void)id;
}

/* Returns word pointed to by the program counter 
 * PARAMS: takes a memory_T
 * RETURNS: a uint32_t that is the next instruction */
uint32_t get_next_instr(T mem)
{
        assert(mem);
        uint32_t inst = get_memory(mem, 0, mem->program_ctr);
        mem->program_ctr++;
        return inst;
}


/* Frees segment 0, duplicates the segment with id <segid>, and makes
 *      the sequence for seg0 point to the newly duplicated spot.  
 * Then sets program_ctr to the given offset 
 * If the <segid> input is 0, does nothing except move prgm_ctr.*/
void mem_load_program(T mem, uint32_t segid, uint32_t offset)
{
        assert(mem);
        if (segid == 0) {
                mem->program_ctr = offset;
                return;
        }
        
        UArray_T seg0 = (UArray_T)Seq_get(mem->memory, 0);
        UArray_free(&seg0);
        UArray_T segb = (UArray_T)Seq_get(mem->memory, segid);

        int length = UArray_length(segb);
        seg0 = UArray_new(length, word_size);
        for (int i = 0; i < length; i++) {
                uint32_t val = *((uint32_t *)(UArray_at(segb, i)) );
                *((uint32_t *)(UArray_at(seg0, i)) ) = val;
        }
        seg0 = Seq_put(mem->memory, 0, seg0);
        mem->program_ctr = offset;
}



/* HELPER FUNCTIONS FOR TESTING PURPOSES */
void print_registers(T mem)
{
        for (int i = 0; i < num_registers; i++) {
                printf("Register %d: %u\n", i, get_register(mem, i));
        }

}
void print_segments(T mem)
{
        printf("UNMAPPED SEGMENT_IDS\n");

        int x = Seq_length(mem->unmapped_ids);
        for (int i = 0; i < x; i++) {
                uint32_t blah = (uint32_t)(uintptr_t)
                                                Seq_get(mem->unmapped_ids, i);
                printf("%u\n", blah);
        }
        
        x = Seq_length(mem->memory);
        for (int i = 0; i < x; i++) {
                UArray_T temp = (UArray_T)Seq_get(mem->memory, i);
                if (temp == NULL)
                        continue;
                printf("SEGMENT %d:\n", i);
                int y = UArray_length(temp);
                for (int j = 0; j < y; j++) {
                        printf("Offset: %d, Word: 0x%08x\n", j,
                                        *(uint32_t *)UArray_at(temp, j));
                }
                printf("\n");
        }
}
