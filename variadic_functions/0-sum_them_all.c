#include "variadic_functions.h"

/**
 * sum_them_all - sums a variable number of unsigned int arguments
 * @n: the number of variadic arguments that follow
 * @...: a variable-length argument list containing exactly n
 * unsigned integers
 *
 * Return: an int representing the sum of all n arguments passed in,
 * or 0 if n is 0
 */
int sum_them_all(const unsigned int n, ...)
{
	va_list args;
	int sum;
	unsigned int i;

	va_start(args, n);

	if (n == 0)
	{
		va_end(args);
		return (0);
	}

	sum = 0;
	for (i = 0; i < n; i++)
		sum += va_arg(args, unsigned int);

	va_end(args);
	return (sum);
}
