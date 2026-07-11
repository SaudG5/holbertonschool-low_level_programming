#include "main.h"
#include <stdio.h>

/**
 * print_most_numbers - print the number for 0 to 9 but 2 and 4
 *
 * Return: Alwayes 0
 */
void print_most_numbers(void)
{
	int i;

	for (i = 0; i <= 9; i++)
	{
		if (i == 2 || i == 4)
		{
			i++;
		}
		_putchar(i + '0');
	}
	_putchar('\n');
}
