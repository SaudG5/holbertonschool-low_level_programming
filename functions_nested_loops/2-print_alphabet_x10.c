#include "main.h"

/**
 * print_alphabet - prints the lowercase alphabet 10 times, followed by a newline
 *
 * Return: void
 */
void print_alphabet_x10(void)
{
	int b = 0;
	while (b < 10)
	{
		int i = 0;

		do {
				_putchar('a' + i);
				i++;
		} while (i < 26);

	_putchar('\n');
	b++;
	}
}
