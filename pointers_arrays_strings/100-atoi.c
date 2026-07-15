#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: the string to be converted
 *
 * Description: the number in the string can be preceded by any
 * number of non-digit characters. Every '-' encountered before the
 * first digit flips the sign; '+' and all other characters have no
 * effect on the sign. If no digits are found, 0 is returned.
 *
 * Return: the converted integer
 */
int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int num = 0;

	while (s[i] != '\0' && (s[i] < '0' || s[i] > '9'))
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}

	while (s[i] >= '0' && s[i] <= '9')
	{
		num = num * 10 - (s[i] - '0');
		i++;
	}

	return (sign == -1 ? num : -num);
}
