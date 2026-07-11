#include "main.h"
#include <stdio.h>

/**
 * _isupper - check if the c is an uppercase
 * @c: is the charcter
 *
 * Return: return 1 if c is an uppercase 0 else
 */
int _isupper(int c)
{
	if (c >= 65 && c <= 90)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
