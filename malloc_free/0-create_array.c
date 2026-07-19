#include "main.h"

/**
 * create_array - creates an array of chars, initialized with a char
 * @size: the size of the array
 * @c: the character to fill the array with
 *
 * Return: pointer to the array, or NULL if it fails or size == 0
 */
char *create_array(unsigned int size, char c)
{
    char *array;
    unsigned int i;

    if (size == 0)
        return (NULL);

    array = malloc(sizeof(char) * size);
    if (array == NULL)
        return (NULL);

    for (i = 0; i < size; i++)
        array[i] = c;

    return (array);
}