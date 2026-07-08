#include "main.h"

/**
 * print_alphabet_x10 - prints the lowercase alphabet 10 times,
 * each followed by a newline
 *
 * Return: void
 */
void print_alphabet_x10(void)
{
	int b = 0;
	int i;

	while (b < 10)
	{
		i = 0;
		do {
			_putchar('a' + i);
			i++;
		} while (i < 26);
		_putchar('\n');
		b++;
	}
}
