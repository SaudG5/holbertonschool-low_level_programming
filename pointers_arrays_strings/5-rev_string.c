#include "main.h"

/**
 * rev_string - reverses a string in place
 * @s: the string to reverse
 *
 * Return: void
 */
void rev_string(char *s)
{
	char *start, *end, tmp;

	start = s;
	end = s;
	while (*end != '\0')
		end++;
	end--;
	while (start < end)
	{
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
}
