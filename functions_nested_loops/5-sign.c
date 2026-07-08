#include <stdio.h>
#include "main.h"
/**
 * print_sign - print the sign of the number
 *@n: the number
 *
 * Return: 1 is + and 0 if 0 and -1 if -
 */
int print_sign(int n)
{
	if (n > 0)
	{
		puts("+");
		return (1);
	}
	else if (n == 0)
	{
		puts("0");
		return (0);
	}
	else
	{
		puts("-");
		return (-1);
	}
}
