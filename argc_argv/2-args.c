#include <stdio.h>
/**
 * main - prints all arguments passed to the program, one per line
 * @argc: the argument count (including the program name)
 * @argv: array of argument strings, each to be printed on its own
 * line, starting from argv[0]
 *
 * Return: always 0 (Success)
 */
int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < argc; i++)
		printf("%s\n", argv[i]);
	return (0);
}
