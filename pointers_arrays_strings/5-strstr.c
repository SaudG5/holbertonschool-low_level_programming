#include "main.h"
#include <stddef.h>

/**
 * _strstr - locates a substring
 * @haystack: the string to search
 * @needle: the substring to find
 *
 * Return: a pointer to the beginning of the located substring,
 * or NULL if the substring is not found
 */
char *_strstr(char *haystack, char *needle)
{
	unsigned int i;
	unsigned int j;

	while (*haystack != '\0')
	{
		i = 0;
		j = 0;

		while (haystack[i] == needle[j] && needle[j] != '\0')
		{
			i++;
			j++;
		}

		if (needle[j] == '\0')
		{
			return (haystack);
		}

		haystack++;
	}

	if (*needle == '\0')
	{
		return (haystack);
	}

	return (NULL);
}
