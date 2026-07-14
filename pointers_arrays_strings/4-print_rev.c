#include "main.h"

/**
 * print_rev - prints a string in reverse, followed by a newline
 * @s: the string to print
 *
 * Return: void
 */
void print_rev(char *s)
{
	char *end;

	end = s;
	while (*end != '\0')
		end++;
	end--;
	while (end >= s)
	{
		_putchar(*end);
		end--;
	}
	_putchar('\n');
}
