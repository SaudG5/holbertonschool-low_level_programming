#include "variadic_functions.h"
#include <stdio.h>
/**
 * print_numbers - prints a variable number of unsigned int
 * arguments, separated by a given string
 * @separator: the string to be printed between numbers; if NULL,
 * nothing is printed between numbers
 * @n: the number of integers passed to the function
 * @...: a variable-length argument list containing exactly n
 * unsigned integers
 *
 * Description: prints each of the n integers in order. Between
 * each pair of numbers, @separator is printed (unless it is NULL,
 * in which case nothing is printed there). After all numbers have
 * been printed, a newline is printed at the end.
 *
 * Return: void (nothing is returned)
 */
void print_numbers(const char *separator, const unsigned int n, ...)
{
	unsigned int i;
	va_list args;
	
	va_start(args, n);

	for (i = 0; i < n; i++)
	{
		printf("%d", va_arg(args, unsigned int));
		if (i != 0 && separator != NULL)
			printf("%s", separator);
	}
	va_end(args);
	printf("\n");
}
