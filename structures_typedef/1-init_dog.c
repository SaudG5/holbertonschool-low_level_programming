#include "dog.h"

/**
 * init_dog - initializes a variable of type struct dog
 * @d: pointer to the struct dog to be initialized
 * @name: the name to assign to the dog
 * @age: the age to assign to the dog
 * @owner: the owner to assign to the dog
 *
 * Description: sets the name, age, and owner members of the
 * struct dog pointed to by d, using the values passed in.
 */
void init_dog(struct dog *d, char *name, float age, char *owner)
{
	if (d == NULL)
		return;

	d->name = name;
	d->age = age;
	d->owner = owner;
}
