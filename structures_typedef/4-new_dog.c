#include "dog.h"
#include <stdlib.h>

/**
 * new_dog - creates a new dog
 * @name: the name of the dog
 * @age: the age of the dog
 * @owner: the owner of the dog
 *
 * Description: allocates memory for a new struct dog and
 * initializes it with copies of name and owner, and the given age.
 *
 * Return: a pointer to the newly allocated struct dog, or NULL
 * if the function fails (allocation failure).
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *d;
	char *name_copy;
	char *owner_copy;
	int len, i;

	d = malloc(sizeof(dog_t));
	if (d == NULL)
		return (NULL);

	for (len = 0; name[len] != '\0'; len++)
		;
	name_copy = malloc(len + 1);
	if (name_copy == NULL)
	{
		free(d);
		return (NULL);
	}
	for (i = 0; i <= len; i++)
		name_copy[i] = name[i];

	for (len = 0; owner[len] != '\0'; len++)
		;
	owner_copy = malloc(len + 1);
	if (owner_copy == NULL)
	{
		free(name_copy);
		free(d);
		return (NULL);
	}
	for (i = 0; i <= len; i++)
		owner_copy[i] = owner[i];

	d->name = name_copy;
	d->age = age;
	d->owner = owner_copy;

	return (d);
}
