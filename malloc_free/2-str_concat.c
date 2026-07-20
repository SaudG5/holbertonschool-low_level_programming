#include "main.h"

/**
 * str_concat - concatenates two strings into a new buffer
 * @s1: first string (treated as empty string if NULL)
 * @s2: second string (treated as empty string if NULL)
 *
 * Return: pointer to the newly allocated concatenated string,
 * or NULL if malloc fails
 */
char *str_concat(char *s1, char *s2)
{
	char *copy;
	int len1 = 0;
	int len2 = 0;
	int i;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";

	while (s1[len1] != '\0')
		len1++;
	while (s2[len2] != '\0')
		len2++;

	copy = malloc(sizeof(char) * (len1 + len2 + 1));
	if (copy == NULL)
		return (NULL);

	for (i = 0; i < len1; i++)
		copy[i] = s1[i];
	for (i = 0; i < len2; i++)
		copy[len1 + i] = s2[i];
	copy[len1 + len2] = '\0';

	return (copy);
}
