/********************************
* COMP40 HW#6: UM
* Meg Farley and Daniel Mahoney
* 11/18/18
* Main function for the universal machine
* ******************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <sys/stat.h>
#include "memory.h"
#include "readprog.h"
#include "instructions.h"

void run_program(Memory_T);

int main(int argc, char *argv[])
{
        /* Takes exactly one command line argument, for name of file that
         * contains the instructions to be run */
        if (argc != 2) {
                fprintf(stderr, "Usage: ./um <programfile.um>\n");
                exit(EXIT_FAILURE);
        }

        /* Gets length of input file in number of bytes */
        struct stat buf;
        stat(argv[1], &buf);
        uint32_t size = buf.st_size;

        Memory_T my_mem = Memory_init(size / 4 + 1);
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL) {
                fprintf(stderr, "Program file could not be read.\n");
                exit(EXIT_FAILURE);
        }

        read_prog(my_mem, fp);
        fclose(fp);

        run_program(my_mem);
        Memory_free(&my_mem);



        return EXIT_SUCCESS;
}

/* Runs the UM.  Keeps reading instructions from segment 0 until a halt
 *      command is found, which would return 1 from do_instr */
void run_program(Memory_T mem)
{
        int going = 1;
        uint32_t inst;
       /* int counter = 0;*/
        while (going == 1) {
                inst = get_next_instr(mem);
                going = do_instr(mem, inst);
               /* counter++;*/
        }
       /* fprintf(stderr,"counter is %d\n",counter); */
}
