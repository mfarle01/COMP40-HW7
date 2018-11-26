#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void emit_halt_test(Seq_T instructions);
extern void emit_loadval_test(Seq_T instructions);
extern void emit_output_test(Seq_T);
extern void emit_io_test(Seq_T);
extern void emit_verbose_halt_test(Seq_T instructions);
extern void emit_add_test(Seq_T instructions);
extern void emit_print_digit_test(Seq_T instructions);
extern void emit_map_test(Seq_T);
extern void emit_map_unmap_test(Seq_T instruction);
extern void emit_cond_move1_test(Seq_T instruction);
extern void emit_cond_move2_test(Seq_T instruction);
extern void emit_seg_store_test(Seq_T instruction);
extern void emit_seg_load_test(Seq_T instruction);
extern void emit_mult_test(Seq_T instruction);
extern void emit_div_test(Seq_T instruction);
extern void emit_nand_test(Seq_T instruction);
extern void emit_load_prog_real(Seq_T);
extern void emit_load_prog_seg0(Seq_T);
/*extern void emit_failure(Seq_T stream);*/


/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*emit_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", emit_halt_test },
        { "load_val",     NULL, "", emit_loadval_test},
        { "output",       NULL, "COMP40", emit_output_test},
        { "io",           "a", "a", emit_io_test},
        { "halt-verbose", NULL, "", emit_verbose_halt_test },
        { "add",          NULL, "B", emit_add_test},
        { "print_digit",  NULL, "6", emit_print_digit_test},
        { "map",          NULL, "", emit_map_test},
        { "map_unmap",    NULL, "", emit_map_unmap_test},
        { "cond_mov_move",NULL, "2", emit_cond_move1_test},
        { "cond_mov_2",   NULL, "1", emit_cond_move2_test},
        { "seg_store",    NULL, "", emit_seg_store_test},
        { "seg_load",     NULL, "", emit_seg_load_test},
        { "mult",         NULL, "6", emit_mult_test},
        { "divide",       NULL, "4", emit_div_test},
        { "nand",         NULL, "", emit_nand_test},
        { "load_prog2",   NULL, "A", emit_load_prog_real},
        { "load_prog1",   NULL, "", emit_load_prog_seg0},
       /* {"failure",       NULL, "", emit_failure}*/
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->emit_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
