#include <stdio.h>

/**
 * main - prints the alphabet in lowercase
 *
 * Return: always 0
 */
int main(void)
{
char alphabet;
for (alphabet = 'a'; alphabet <= 'z'; alphabet++)
{
if (alphabet == 'q' || alphabet == 'e')
{
continue;
}
putchar(alphabet);
}
putchar('\n');
return (0);

}
