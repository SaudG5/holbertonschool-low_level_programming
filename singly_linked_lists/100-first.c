#include <stdio.h>

/**
 * first - prints a message before main() is executed
 */
__attribute__((constructor)) void first(void)
{
	printf("You're beat! and yet, you must allow,\n");
	printf("I bore my house upon my back!\n");
}
