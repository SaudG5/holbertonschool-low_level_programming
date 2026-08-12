#include <stdio.h>
#include "hash_tables.h"

/**
 * hash_table_print - Prints a hash table
 * @ht: The hash table to print
 *
 * Description: Prints key/value pairs in the order they appear in
 * the array of the hash table (array order, then list order within
 * each bucket), Python dict style
 */
void hash_table_print(const hash_table_t *ht)
{
	unsigned long int i;
	hash_node_t *tmp;
	int first;

	if (ht == NULL)
		return;

	printf("{");
	first = 1;

	for (i = 0; i < ht->size; i++)
	{
		tmp = ht->array[i];
		while (tmp != NULL)
		{
			if (first == 0)
				printf(", ");
			printf("'%s': '%s'", tmp->key, tmp->value);
			first = 0;
			tmp = tmp->next;
		}
	}

	printf("}\n");
}
