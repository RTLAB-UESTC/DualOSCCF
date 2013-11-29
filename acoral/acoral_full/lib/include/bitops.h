#ifndef ACORAL_BITOPS_H
#define ACORAL_BITOPS_H
#include "type.h"
acoral_u32 acoral_find_first_bit(const acoral_u32 *b,acoral_u32 length);
void acoral_set_bit(acoral_32 nr,acoral_u32 *p);
void acoral_clear_bit(acoral_32 nr,acoral_u32 *p);
acoral_u32 acoral_find_next_zero_bit(acoral_u32 *bitmap, acoral_u32 size, acoral_u32 offset);
acoral_u32 acoral_find_next_set_bit(acoral_u32 *bitmap, acoral_u32 size, acoral_u32 offset);

#endif
