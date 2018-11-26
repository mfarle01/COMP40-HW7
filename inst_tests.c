#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "memory.h"
#include "instructions.h"

void initialize(Memory_T);
void test_insts(Memory_T);
typedef enum reg { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } reg;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        Memory_T mymem = Memory_init(20);
        
        initialize(mymem);
        test_insts(mymem);


        return EXIT_SUCCESS;
}

void initialize(Memory_T mem)
{
        for (int i = 0; i < 8; i++) {
                set_register(mem, i, (uint32_t)i);
        }
        for (int i = 0; i < 5; i++) {
                set_memory(mem, 0, i, (uint32_t)(i * i));
        }
}


void test_insts(Memory_T mem)
{
        printf("TEST 1a: Cond Mov with $rc=0\n");
        cond_mov(mem, r1, r2, r0);
        if (get_register(mem, r1) == 1)
                printf("SUCCESS. Didn't move on $rc=0\n");
        else
                printf("FAILED.  Expeceted %d, Actual %d\n", 1,
                                                 get_register(mem, r1));

        printf("TEST 1b: Cond mov with $rc != 0\n");
        cond_mov(mem, r1, r2, r3);
        uint32_t x = get_register(mem, r1);
        if (x == 2)
                printf("SUCCESS.  Moved data\n");
        else
                printf("FAILED. Expected: %d, Actual %d\n", 2, x);


        printf("TEST 2: Seg store\n");
        




}









