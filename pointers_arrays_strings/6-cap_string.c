#include "main.h"

/**
 * is_separator - checks if a character is a word separator
 * @c: character to check
 *
 * Return: 1 if c is a separator, 0 otherwise
 */
int is_separator(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == ',' || c == ';' ||
	    c == '.' || c == '!' || c == '?' || c == '"' || c == '(' ||
	    c == ')' || c == '{' || c == '}')
		return (1);

	return (0);
}

/**
 * cap_string - capitalizes all words of a string
 * @str: string to capitalize
 *
 * Return: pointer to the capitalized string
 */
char *cap_string(char *str)
{
	int i = 0;
	int cap_next = 1;

	while (str[i] != '\0')
	{
		if (is_separator(str[i]))
		{
			cap_next = 1;
		}
		else
		{
			if (cap_next && str[i] >= 'a' && str[i] <= 'z')
				str[i] = str[i] - 'a' + 'A';
			cap_next = 0;
		}
		i++;
	}

	return (str);
}
