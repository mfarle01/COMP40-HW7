
#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include <except.h>

#define uintx_t uint32_t
#define intx_t int32_t
#define max_size 32

bool Bitpack_fitsu(uintx_t n, unsigned width);
bool Bitpack_fitss( intx_t n, unsigned width);
uintx_t Bitpack_getu(uintx_t word, unsigned width, unsigned lsb);
 intx_t Bitpack_gets(uintx_t word, unsigned width, unsigned lsb);
uintx_t Bitpack_newu(uintx_t word, unsigned width, unsigned lsb, uintx_t value);
uintx_t Bitpack_news(uintx_t word, unsigned width, unsigned lsb,  intx_t value);
extern Except_T Bitpack_Overflow;
#endif
