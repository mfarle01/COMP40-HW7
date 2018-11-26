#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "memory.h"

uint32_t test_register(Memory_T, uint32_t);
uint32_t test_memory(Memory_T, uint32_t);
uint32_t test_map(Memory_T, uint32_t);
void test_unmap(Memory_T, uint32_t);

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;
        Memory_T my_mem;
        my_mem = Memory_init(12);
        if (my_mem == NULL) {
                printf("Couldn't init\n");
                exit(1);
        }
        printf("You allocated memory yay.\n");

        uint32_t x = 4;
        uint32_t returned = test_register(my_mem, x);
        if (returned == 4) {
                printf("TEST 1: SUCCESS\n");
        } else {
                printf("TEST 1: FAILED; Expected: %d, Actual: %d\n", x,
                                                                   returned);
        }

        returned = test_memory(my_mem, x);
        if (returned == 4) {
                printf("TEST 2: SUCCESS\n");
        } else {
                printf("TEST 2: FAILED; Expected: %d, Actual: %d\n", x,
                                                                   returned);
        }

        uint32_t mapped_seg = test_map(my_mem, 100);
        printf("TEST 3: If you got this far, mapping the segment = SUCCESS.\n");
        printf("mapped seg_id: %u\n", mapped_seg);

        printf("TEST 4:\n");
        test_unmap(my_mem, mapped_seg);
        printf("If the print segments output no longer has the id %d\n", 
                                                                mapped_seg);
        printf("then Test 4 = SUCCESS.  Else, FAILED.\n");

        Memory_free(&my_mem);
        if (my_mem == NULL) {
                printf("TEST 5: SUCCESS; successfully free'd memory\n");
        } else {
                printf("TEST 5: FAILED; freeing memory didn't work\n");
        }

        return EXIT_SUCCESS;
}


uint32_t test_register(Memory_T mem, uint32_t x)
{
        set_register(mem, 0, x);
        return get_register(mem, 0);
}

uint32_t test_memory(Memory_T mem, uint32_t x)
{
        set_memory(mem, 0, 0, x);
        return get_memory(mem, 0, 0);
}

uint32_t test_map(Memory_T mem, uint32_t length)
{
        uint32_t x = map_segment(mem, length);
        set_memory(mem, x, 0, 127);
        uint32_t word = get_memory(mem, x, 0);
        if (word == 127)
                printf("setting memory worked again\n");
        return x;
}

void test_unmap(Memory_T mem, uint32_t segid)
{
        unmap_segment(mem, segid);
        print_segments(mem);
}


