#include <stdio.h>
#include <stdlib.h>

/**
 * main - adds all positive numbers passed as command-line arguments
 * @argc: argument count (including the program name)
 * @argv: array of argument strings, each expected to be a positive
 * number
 *
 * Return: 0 on success, 1 if any argument contains a non-digit
 * character
 */
int main(int argc, char *argv[])
{
	int i, j, sum;

	sum = 0;
	for (i = 1; i < argc; i++)
	{
		for (j = 0; argv[i][j] != '\0'; j++)
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error\n");
				return (1);
			}
		}
		sum += atoi(argv[i]);
	}

	printf("%d\n", sum);
	return (0);
}
