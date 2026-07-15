#include "main.h"

/**
 * puts_half - prints half of a string, followed by a new line
 * @str: the string to print half of
 *
 * Description: if the number of characters is odd, the function
 * prints the last n characters of the string, where
 * n = (length_of_the_string + 1) / 2
 *
 * Return: void
 */
void puts_half(char *str)
{
	int len = 0;
	int start;
	int i;

	while (str[len] != '\0')
		len++;
	if (len % 2 == 0)
		start = len / 2;
	else
		start = (len + 1) / 2;

	for (i = start; i < len; i++)
		_putchar(str[i]);

	_putchar('\n');
}
