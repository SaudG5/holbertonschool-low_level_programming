#include "main.h"

/**
 * puts2 - prints every other character of a string, starting with the
 *         first character, followed by a newline
 * @str: the string to print
 *
 * Return: void
 */
void puts2(char *str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i = i + 2;
	}
	_putchar('\n');
}
