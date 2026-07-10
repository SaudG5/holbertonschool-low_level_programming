#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 *
 * Return: void
 */
void times_table(void)
{
	int i, b, result;

	for (i = 0; i <= 9; i++)
	{
		for (b = 0; b <= 9; b++)
		{
			result = i * b;

			if (b == 0)
			{
				_putchar((result % 10) + '0');
			}
			else
			{
				_putchar(',');
				_putchar(' ');
				if (result >= 10)
					_putchar((result / 10) + '0');
				else
					_putchar(' ');
				_putchar((result % 10) + '0');
			}
		}
		_putchar('\n');
	}
}
