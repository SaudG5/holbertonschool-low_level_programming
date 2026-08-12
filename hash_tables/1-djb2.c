#include "hash_tables.h"

/**
 * hash_djb2 - Implements the djb2 hash algorithm
 * @str: The string to hash
 *
 * Description: This hash function was first reported by Dan Bernstein.
 * magic constant 5381 is used as the starting hash value, and each
 * character is folded in via hash * 33 + c
 *
 * Return: The hash of str
 */
unsigned long int hash_djb2(const unsigned char *str)
{
	unsigned long int hash;
	int c;

	hash = 5381;
	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return (hash);
}
