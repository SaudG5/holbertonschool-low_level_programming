#include "main.h"
#include <stdio.h>

/**
 * _isdigit - check if the c is a digit
 * @c: is the digit
 *
 * Return: return 1 if c is a digit 0 else
 */
int _isupper(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
