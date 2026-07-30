#include "variadic_functions.h"
#include <stdio.h>

/**
 * print_strings - prints a variable number of strings, separated
 * by a given string
 * @separator: the string to be printed between the strings; if
 * NULL, nothing is printed between them
 * @n: the number of strings passed to the function
 * @...: a variable-length argument list containing exactly n
 * strings (char *)
 *
 * Description: prints each of the n strings in order. Between each
 * pair of strings, @separator is printed (unless it is NULL, in
 * which case nothing is printed there). If any given string is
 * NULL, the literal text "(nil)" is printed in its place instead of
 * attempting to print the string itself. After all strings have
 * been printed, a newline is printed at the end.
 *
 * Return: void (nothing is returned)
 */
void print_strings(const char *separator, const unsigned int n, ...)
{
	char *c;
	unsigned int i;
	va_list args;

	va_start(args, n);
	for (i = 0; i < n; i++)
	{
		if (i != 0 && separator != NULL)
			printf("%s", separator);
		c = va_arg(args, char *);
		if(c == NULL)
			printf("(nil)");
		else
			printf("%s", c);
	}
	va_end(args);
	printf("\n");
}
