#include <stdio.h>
#include "variadic_functions.h"

/**
 * print_char - prints a char from a va_list
 * @args: the va_list to pull the argument from
 */
void print_char(va_list args)
{
	printf("%c", va_arg(args, int));
}

/**
 * print_int - prints an int from a va_list
 * @args: the va_list to pull the argument from
 */
void print_int(va_list args)
{
	printf("%d", va_arg(args, int));
}

/**
 * print_float - prints a float from a va_list
 * @args: the va_list to pull the argument from
 */
void print_float(va_list args)
{
	printf("%f", va_arg(args, double));
}

/**
 * print_string - prints a string from a va_list, or (nil) if NULL
 * @args: the va_list to pull the argument from
 */
void print_string(va_list args)
{
	char *str;

	str = va_arg(args, char *);
	if (str == NULL)
		str = "(nil)";
	printf("%s", str);
}

/**
 * print_all - prints anything, based on a format string
 * @format: a string listing the type of each argument ('c', 'i',
 * 'f', 's'); any other character is ignored
 * @...: the variadic arguments matching the recognized characters
 * in format
 *
 * Return: void
 */
void print_all(const char * const format, ...)
{
	va_list args;
	unsigned int i, j;
	char *separator;
	format_t formats[] = {
		{"c", print_char},
		{"i", print_int},
		{"f", print_float},
		{"s", print_string}
	};

	i = 0;
	separator = "";
	va_start(args, format);
	while (format != NULL && format[i] != '\0')
	{
		j = 0;
		while (j < 4)
		{
			if (format[i] == formats[j].format[0])
			{
				printf("%s", separator);
				formats[j].f(args);
				separator = ", ";
			}
			j++;
		}
		i++;
	}
	va_end(args);
	printf("\n");
}
