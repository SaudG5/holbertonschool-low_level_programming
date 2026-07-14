#include "main.h"

/**
 * _stlen - lenth of a string
 * @s: the string
 *
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int len = 0;

	while (*s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}
