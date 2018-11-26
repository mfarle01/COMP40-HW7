/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>

#include <assert.h>
#include <seq.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        assert(op >= 0 && op < 16);
        assert(ra >= 0 && ra < 8);
        assert(rb >= 0 && rb < 8);
        assert(rc >= 0 && rc < 8);

        Um_instruction out = 0;
        out |= op << 28;
        out |= ra << 6;
        out |= rb << 3;
        out |= rc;
        //printf("three_reg func: %d %d %d %d, %d\n", op, ra, rb, rc, out);
        return out;
}




Um_instruction loadval(unsigned ra, unsigned val)
{
        assert(ra < 8);
        assert(val < (1 << 25));
        Um_instruction out = 0xD0000000;
        out |= ra << 25;
        out |= val;
        return out;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction cond_mov(Um_register a, Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction seg_load(Um_register a, Um_register b, Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction seg_store(Um_register a, Um_register b, Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map_seg(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_seg(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{       
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction load_prog(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

static inline Um_instruction load_val(Um_register a, unsigned val)
{
        return loadval(a, val);
}






/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

void Um_write_sequence(FILE *output, Seq_T stream)
{
        while (Seq_length(stream) > 0) {
                Um_instruction temp = (Um_instruction)(uintptr_t)Seq_remlo(stream);
                printf("0x%x\n", temp);
                for (int k = 24; k >= 0; k -= 8) {
                        putc(temp >> k, output);
                }
        } 
}


/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}


void emit_loadval_test(Seq_T stream)
{
        emit(stream, load_val(r0, 1 << 24));
        emit(stream, halt());
}

void emit_output_test(Seq_T stream)
{
        emit(stream, load_val(r0, 67));
        emit(stream, load_val(r1, 79));
        emit(stream, load_val(r2, 77));
        emit(stream, load_val(r3, 80));
        emit(stream, load_val(r4, 52));
        emit(stream, load_val(r5, 48));
        emit(stream, output(r0));
        emit(stream, output(r1));
        emit(stream, output(r2));
        emit(stream, output(r3));
        emit(stream, output(r4));
        emit(stream, output(r5));
        emit(stream, halt());
}

void emit_io_test(Seq_T stream)
{
        emit(stream, input(r0));
        emit(stream, output(r0));
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, load_val(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, load_val(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, load_val(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, load_val(r1, '!'));
        emit(stream, output(r1));
        emit(stream, load_val(r1, '\n'));
        emit(stream, output(r1));
}

void emit_add_test(Seq_T stream)
{
        emit(stream, load_val(r2, 64));
        emit(stream, load_val(r3, 2));
        emit(stream, add(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_print_digit_test(Seq_T stream)
{
        emit(stream, load_val(r1, 48));
        emit(stream, load_val(r2, 6));
        emit(stream, add(r3, r1, r2));
        emit(stream, output(r3));
        emit(stream, halt());
}


void emit_map_test(Seq_T stream)
{
        emit(stream, load_val(r0, 10));
        emit(stream, map_seg(r1, r0));
        emit(stream, halt());
}


void emit_map_unmap_test(Seq_T stream)
{
        emit(stream, load_val(r2, 10));
        emit(stream, map_seg(r1, r2));
        emit(stream, unmap_seg(r1));
        emit(stream, halt());
}

/* Shouldn't actually move because $r3 = 0*/
void emit_cond_move1_test(Seq_T stream)
{
        emit(stream, load_val(r1, 1));
        emit(stream, load_val(r2, 2));
        emit(stream, load_val(r3, 0));
        emit(stream, cond_mov(r2, r1, r3));
        emit(stream, load_val(r0, 48));
        emit(stream, add(r2, r0, r2));
        emit(stream, output(r2));
        emit(stream, halt());
}


/* Should actually move b/c $r3 != 0 
 * Expected output: 1 */
void emit_cond_move2_test(Seq_T stream)
{
        emit(stream, load_val(r1, 1));
        emit(stream, load_val(r2, 2));
        emit(stream, load_val(r3, 3));
        emit(stream, cond_mov(r2, r1, r3));
        emit(stream, load_val(r0, 48));
        emit(stream, add(r2, r0, r2));
        emit(stream, output(r2));
        emit(stream, halt());
}

/* stores value form register into memory */
void emit_seg_store_test(Seq_T stream)
{
        emit(stream, load_val(r1, 5));
        emit(stream, map_seg(r2, r1));
        emit(stream, load_val(r2, 1));
        emit(stream, load_val(r3, 0));
        emit(stream, seg_store(r2, r3, r1));
        emit(stream, halt());
} 


/* loads value from memory into register */
void emit_seg_load_test(Seq_T stream)
{
        emit(stream, load_val(r2, 0));
        emit(stream, load_val(r3, 1));
        emit(stream, seg_load(r1, r2, r3));
        emit(stream, halt());
} 

void emit_mult_test(Seq_T stream)
{
        emit(stream, load_val(r0, 48));
        emit(stream, load_val(r2,2));
        emit(stream, load_val(r3, 3));
        emit(stream, multiply(r1, r2, r3));
        emit(stream, add(r0, r1, r0));
        emit(stream, output(r0));
        emit(stream, halt());
}

void emit_div_test(Seq_T stream)
{
        emit(stream, load_val(r0, 48));
        emit(stream, load_val(r2, 12));
        emit(stream, load_val(r3, 3));
        emit(stream, divide(r1, r2, r3));
        emit(stream, add(r0, r1, r0));
        emit(stream, output(r0));
        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        emit(stream, load_val(r3, 1));
        emit(stream, nand(r1, r2, r3));
        emit(stream, halt());
}

void emit_load_prog_real(Seq_T stream)
{
        emit(stream, load_val(r1, 3));
        emit(stream, map_seg(r2, r1));
        emit(stream, load_val(r0, 12));
        emit(stream, seg_load(r3, r6, r0));
        emit(stream, seg_store(r2, r2, r3));
        emit(stream, load_val(r7, 65));
        emit(stream, load_val(r0, 13));
        emit(stream, seg_load(r3, r6, r0));
        emit(stream, seg_store(r2, r6, r3));

        emit(stream, load_prog(r2, r5));
        emit(stream, load_val(r6, 'B'));
        emit(stream, output(r6));
        emit(stream, halt());
        emit(stream, output(r7));
}


void emit_load_prog_seg0(Seq_T stream)
{
        emit(stream, load_val(r1, 4));
        emit(stream, load_prog(r0, r1));
        emit(stream, load_val(r2, 'B'));
        emit(stream, output(r2));
        emit(stream, halt());
}
/*void emit_failure(Seq_T stream)
{
        emit(stream, load_val(r1, 4));
        emit(stream, seg_store(r1, r2, r2));
        emit(stream, halt());
}*/
