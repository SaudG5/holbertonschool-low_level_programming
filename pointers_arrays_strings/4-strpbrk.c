#include "main.h"
#include <stddef.h>

/**
 * _strpbrk - searches a string for any of a set of bytes
 * @s: the string to search
 * @accept: the string of bytes to look for
 *
 * Return: a pointer to the byte in s that matches one of the
 * bytes in accept, or NULL if no such byte is found
 */
char *_strpbrk(char *s, char *accept)
{
	unsigned int i;

	while (*s != '\0')
	{
		i = 0;

		while (accept[i] != '\0')
		{
			if (*s == accept[i])
			{
				return (s);
			}
			i++;
		}

		s++;
	}

	return (NULL);
}
