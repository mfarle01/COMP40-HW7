/*
 * bitpack.c
 * Lauren Labell and Daniel Mahoney
 * Homework 4
 *
 * Provides the implementation for bitpack.h. Used to pack multiple small values
 * (fields) into a single uint64_t 
 *
 */

#include <assert.h>
#include "bitpack.h"

//#define intx_t int32_t
//#define uintx_t uint32_t
//#define max_size 32

/*
 * Tests to see if an unsigned integer n can be represented in width bits.
 * Returns true iff n < 2^w
 */
bool Bitpack_fitsu(uintx_t n, unsigned width)
{
        /* Fields of width zero are defined to contain the value 0 */
        if (width == 0) {
                if (n == 0) 
                        return true;
                else
                        return false;
        } 

        /* Any possible value of n (a uint64_t), fits in 64 bits */
        if (width >= max_size)
                return true;

        /* Set max = 2^width */
        uintx_t max = 1;
        max = max << width;     
        if (n < max)
                return true;
        return false;
}

/*
 * Tests to see if an unsigned integer n can be represented in width bits.
 * Returns true iff n ≥ -(2^(w-1)) and n ≤ 2^(w-1) - 1
 */
bool Bitpack_fitss(intx_t n, unsigned width)
{
        /* Fields of width zero are defined to contain the value 0 */
        if (width == 0) {
                if (n == 0) 
                        return true;
                else
                        return false;
        } 
        
        /* Any possible value of n (a uint64_t), fits in 64 bits */
        if (width >= max_size)
                return true;

        /* Set min = -(2^(w-1)) and max 2^(w-1) */
        intx_t min = -1;
        intx_t max = 1;
        min = min << (width - 1);
        max = max << (width - 1);

        if (n >= min && n < max)
                return true;
        return false;
}

/*
 * Gets back width bits from the input word, ending with the bit at lsb
 */
uintx_t Bitpack_getu(uintx_t word, unsigned width, unsigned lsb)
{
        assert(width <= max_size);
        assert((width + lsb) <= max_size);

        if (width == 0)
                return 0;

        uintx_t mask = ~0;
        mask = mask >> (max_size - width);
        mask = mask << lsb;

        word = (word & mask) >> lsb;

        return word;
}

/*
 * Gets back width bits from the input word, ending with the bit at lsb
 */
intx_t Bitpack_gets(uintx_t word, unsigned width, unsigned lsb)
{
        assert(width <= max_size);
        assert((width + lsb) <= max_size);    

        if (width == 0)
                return 0;   

        uintx_t mask = ~0;
        mask = mask >> (max_size - width);
        mask = mask << lsb;

        intx_t out = (word & mask);
        out = out << (max_size - (width + lsb));
        out = out >> (max_size - width);

        return out;
}

/*
 * Replaces a field in a given word 
 */
uintx_t Bitpack_newu(uintx_t word, unsigned width, unsigned lsb, uintx_t value)
{
        assert(width <= max_size);
        assert((width + lsb) <= max_size); 

        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uintx_t mask = ~0;
        mask = mask >> (max_size - width);
        mask = mask << lsb; 
        mask = ~mask;

        uintx_t out_word = (word & mask) | (value << lsb);

        return out_word;
}

/*
 * Replaces a field in a given word 
 */
uintx_t Bitpack_news(uintx_t word, unsigned width, unsigned lsb,  intx_t value)
{
        assert(width <= max_size);
        assert((width + lsb) <= max_size); 

        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }       

        uintx_t u_value = value << (max_size - width);
        u_value = u_value >> (max_size - width);

        return Bitpack_newu(word, width, lsb, u_value);
}

Except_T Bitpack_Overflow = { "Overflow packing bits" };




