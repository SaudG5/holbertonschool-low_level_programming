#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 *
 * Return: void
 */
void times_table(void)
{
	int i = 0, b, result;

	while (i <= 9)
	{
		b = 0;
		while (b <= 9)
		{
			result = i * b;

			if (b != 0)
			{
				_putchar(',');
				_putchar(' ');
			}

			if (result >= 10)
				_putchar((result / 10) + '0');
			else
				_putchar(' ');

			_putchar((result % 10) + '0');
			b++;
		}
		_putchar('\n');
		i++;
	}
}
