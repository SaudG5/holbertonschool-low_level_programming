#include "main.h"
#include <stdio.h>
/**
 * print_to_98 - count any nymber to 98
 * @n: number that conut start from
 *
 * Return: the counts from n to 98
 */
void  print_to_98(int n)
{
	if (n > 98)
	{
		while (n > 98)
		{
			printf("%d, ", n);
			n = n - 1;
		}
		printf("%d\n", n);
		return;
	}
	else if (n < 98)
	{
		while (n < 98)
		{
			printf("%d, ", n);
			n = n + 1;
		}
		printf("%d\n", n);
		return;
	}
	else
	{
		printf("%d\n", n);
		return;
	}
}
