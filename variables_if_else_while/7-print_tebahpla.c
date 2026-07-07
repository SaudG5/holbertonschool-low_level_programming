#include <stdio.h>

/**
 * main - prints the lowercase alphabet in reverse
 *
 * Return: Always 0
 */
int main(void)
{
	char alphabet;

	alphabet = 'z';
	while (alphabet >= 'a')
	{
		putchar(alphabet);
		alphabet--;
	}
	putchar('\n');

	return (0);
}
