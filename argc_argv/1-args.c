#include <stdio.h>

/**
 * main - prints the number of arguments passed to the program
 * @argc: the argument count (includes the program name itself,
 * so it must be decremented by 1 before printing)
 * @argv: array of argument strings (unused here)
 *
 * Return: always 0 (Success)
 */
int main(int argc, char *argv[])
{
	(void)argv;

	printf("%d", argc - 1);
	return (0);
}
