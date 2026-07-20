#include "main.h"

/**
 * _strdup - function returns a pointer to a new string which is a duplicate of the string str
 * @str: pointer
 *
 * Return: NULL if str = NULL or insufficient memory, on success function returns a pointer to the duplicated string
 */
char *_strdup(char *str)
{
	int len = 0;
	char *copy;
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
