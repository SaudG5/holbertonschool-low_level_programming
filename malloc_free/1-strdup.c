#include "main.h"

/**
 * _strdup - duplicates a string into a newly allocated buffer
 * @str: pointer to the string to duplicate
 *
 * Return: pointer to the duplicated string, or NULL if str is
 * NULL or if memory allocation fails
 */
char *_strdup(char *str)
{
	char *copy;
	int len = 0;
	int i;

	if (str == NULL)
		return (NULL);

	while (str[len] != '\0')
		len++;

	copy = malloc(sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);

	for (i = 0; i < len; i++)
		copy[i] = str[i];
	copy[len] = '\0';

	return (copy);
}
