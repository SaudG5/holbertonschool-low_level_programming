#include "main.h"
#include <stdlib.h>
/**
 * string_nconcat - Concatenates two strings.
 * @s1: The first string.
 * @s2: The second string.
 * @n: The maximum number of bytes of s2 to concatenate.
 *
 * Return: A pointer to the newly allocated memory containing s1
 *         followed by the first n bytes of s2 and null-terminated,
 *         or NULL if the allocation fails.
 */
char *string_nconcat(char *s1, char *s2, unsigned int n)
{
	unsigned int len1 = 0, len2 = 0, i = 0, j = 0;
	char *contact;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";

	while (s1[len1] != '\0')
		len1++;

	while (s2[len2] != '\0')
		len2++;

	if (n >= len2)
		n = len2;

	contact = malloc(sizeof(char) * (len1 + n + 1));

	if (contact == NULL)
		return (NULL);

	for (i = 0; i < len1; i++)
	{
		contact[i] = s1[i];
	}

	for (j = 0; j < n; j++, i++)
	{
		contact[i] = s2[j];
	}

	contact[i] = '\0';

	return (contact);
}
