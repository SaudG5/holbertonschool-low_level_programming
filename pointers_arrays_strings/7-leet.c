#include "main.h"

/**
 * leet - encodes a string into 1337
 * @str: string to encode
 *
 * Return: pointer to the encoded string
 */
char *leet(char *str)
{
	char find[] = "aAeEoOtTlL";
	char replace[] = "4433007711";
	int i = 0;
	int j;

	while (str[i] != '\0')
	{
		j = 0;
		for (j = 0; j < 10; j++)
		{
			if (str[i] == find[j])
				str[i] = replace[j];
		}
		i++;
	}

	return (str);
}
