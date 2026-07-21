#include "main.h"

/**
 * _calloc - Allocates memory for an array using malloc,
 *           setting the memory to zero.
 * @nmemb: The number of elements in the array.
 * @size: The size in bytes of each element.
 *
 * Return: A pointer to the allocated memory, or NULL if
 *         nmemb is 0, size is 0, or if malloc fails.
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
	void *calloc;
	
	if (nmemb == 0 || size == 0)
		return (NULL);

	calloc = malloc(sizeof(size) * nmemb);
	if (calloc == NULL)
		return (NULL);

	return (calloc);
