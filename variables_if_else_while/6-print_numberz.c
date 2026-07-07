#include <stdlib.h>
#include <stdio.h>

/**
 * main - Prints the numbers from 0 to 9
 *
 * Return: Always 0
 */
int main(void)
{
	int i;

	i = 0;
	while (i < 10)
	{
		putchar('0' + i);
		i++;
	}
	putchar('\n');

	return (0);
}
