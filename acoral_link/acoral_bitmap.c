#include "acoral_bitmap.h"
int test_and_set_bit(int offset, void *addr)
{
	// get the last 32bit mask
	unsigned long mask = 1UL << (offset & (sizeof(unsigned long) * BITS_PER_BYTE - 1));
	// get the right address
	unsigned long *p = ((unsigned long*)addr) + (offset >> (sizeof(unsigned long) + 1));
	unsigned long old = *p;

	*p = old | mask;

	return (old & mask) != 0;
}

void clear_bit(int offset, void *addr)
{
	unsigned long mask = 1UL << (offset & (sizeof(unsigned long) * BITS_PER_BYTE - 1));
	unsigned long *p = ((unsigned long*)addr) + (offset >> (sizeof(unsigned long) + 1));
	unsigned long old = *p;

	*p = old & ~mask;
}

int find_next_zero_bit(void *addr, int size, int offset)
{
	unsigned long *p;
	unsigned long mask;

	while (offset < size)
	{
		p = ((unsigned long*)addr) + (offset >> (sizeof(unsigned long) + 1));
		mask = 1UL << (offset & (sizeof(unsigned long) * BITS_PER_BYTE - 1));

		if ((~(*p) & mask))
		{
			break;
		}

		++offset;
	}

	return offset;
}


