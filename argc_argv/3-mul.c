#include <stdio.h>
#include <stdlib.h>

/**
 * main - multiplies two numbers passed as command-line arguments
 * @argc: argument count (including the program name)
 * @argv: array of argument strings; argv[1] and argv[2] are the
 * two numbers to multiply
 *
 * Return: 0 on success, 1 if not exactly two arguments are given
 */
int main(int argc, char *argv[])
{
	int num1, num2, result;

	if (argc != 3)
	{
		printf("Error\n");
		return (1);
	}

	num1 = atoi(argv[1]);
	num2 = atoi(argv[2]);
	result = num1 * num2;

	printf("%d\n", result);
	return (0);
}
