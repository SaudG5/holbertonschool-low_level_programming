#include "main.h"
#include <stdio.h>

/**
 * more_numbers - print the number for 0 to 14 10times
 *
 * Return: Alwayes 0
 */
void more_numbers(void)
{
	int i;

	for (i = 0; i < 10; i++)
	{
		int j;

		for (j = 0; j <= 14; j++)
		{
			if (j >= 10)
				_putchar((j / 10) + '0');
			_putchar((j % 10) + '0');
		}
		_putchar('\n');
	}
}
