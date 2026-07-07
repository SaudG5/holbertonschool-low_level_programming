#include <stdlib.h>
#include <stdio.h>

/**
 * main - prints all numbers of base 16 in lowercase
 *
 * Return: Always 0
 */
int main(void)
{
	int i;

	i = 0;
	while (i < 16)
	{
		if (i < 10)
			putchar('0' + i);
		else
			putchar('a' + (i - 10));
		i++;
	}
	putchar('\n');

	return (0);
}
