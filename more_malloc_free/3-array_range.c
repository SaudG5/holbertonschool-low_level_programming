#include "main.h"
#include <stdlib.h>

/**
 * array_range - creates an array of integers ranging from min to max
 * @min: the starting value of the array (included)
 * @max: the ending value of the array (included)
 *
 * Return: a pointer to the newly created array, or NULL on failure
 */
int *array_range(int min, int max)
{
	int *arr;
	int size;
	int i;

	if (min > max)
		return (NULL);

	size = max - min + 1;

	arr = malloc(sizeof(int) * size);
	if (arr == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
		arr[i] = min + i;

	return (arr);
}
