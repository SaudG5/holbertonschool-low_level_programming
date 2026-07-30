#include "variadic_functions.h"

/**
 * sum_them_all - Sums a variable number of unsigned int arguments.
 *
 * @n: The number of variadic arguments that follow. Since it is declared
 *     const, the function must not modify it.
 * @...: A variable-length argument list containing exactly n unsigned
 *       integers, to be accessed via the <stdarg.h> macros
 *       (va_list, va_start, va_arg, va_end).
 *
 * Description: If n is 0, the function returns 0 immediately without
 * touching the variadic list. Otherwise, it iterates n times, extracting
 * one unsigned int per iteration and accumulating them into a running
 * total.
 *
 * Return: An int representing the sum of all n arguments passed in.
 */
int sum_them_all(const unsigned int n, ...)
{
    int sum;
    unsigned int i;
    va_list args;

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
