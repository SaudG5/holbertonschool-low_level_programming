#include "main.h"
#include <stdio.h>

/**
 * print_line - print (_) n times in the terminal make it a line
 * @n: the times of (_) shuld be printed
 *
 * Return: void
 */
void print_line(int n)
{
	if (n > 0)
	{
		int i;

		for (i = 0; i < n; i++)
		{
			_putchar('_');
		}
	}
	_putchar('\n');
}
