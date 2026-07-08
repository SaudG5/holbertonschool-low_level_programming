#include "main.h"

/**
 * print_alphabet - prints the lowercase alphabet, followed by a newline
 *
 * Return: void
 */
void print_alphabet(void)
{
	int i = 0;

	do {
		_putchar('a' + i);
		i++;
	} while (i < 26);

	_putchar('\n');
}
