#include "hash_tables.h"

/**
 * key_index - Gives the index of a key
 * @key: The key
 * @size: The size of the array of the hash table
 *
 * Description: The index returned must be within the range of
 * the array of the hash table. That's why we use the modulo
 * operator (%) on the result of hash_djb2
 *
 * Return: The index at which the key/value pair should be stored
 */
unsigned long int key_index(const unsigned char *key, unsigned long int size)
{
	unsigned long int hash;

	hash = hash_djb2(key);

	return (hash % size);
}
