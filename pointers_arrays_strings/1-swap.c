#include "main.h"

/**
 * swap_int - swap values of two inputs
 * @a: first pointer
 * @b: second pointer
 *
 * Return: void
 */
void swap_int(int *a, int *b)
{
	int c = *a;

	*a = *b;
	*b = c;
}
