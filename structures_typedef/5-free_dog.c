#include "dog.h"
#include <stdio.h>
/**
 * free_dog - frees a dog
 * @d: pointer to the struct dog to be freed
 *
 * Description: frees the memory allocated for the name and owner
 * strings of the dog pointed to by d, then frees d itself.
 */
void free_dog(dog_t *d)
{
	if (d == NULL)
		return;

	free(d->name);
	free(d->age);
	free(d->owner);
}
