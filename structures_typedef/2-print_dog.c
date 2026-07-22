#include "dog.h"
#include <stdio.h>

/**
 * print_dog - prints a struct dog
 * @d: pointer to the struct dog to be printed
 *
 * Description: prints the name, age, and owner of the dog pointed
 * to by d, in the format "Name: ...", "Age: ...", "Owner: ...".
 * If any member is NULL, "(nil)" is printed instead of that member.
 * If d itself is NULL, nothing is printed.
 */
void print_dog(struct dog *d)
{
	if (d == NULL)
		return;

	if (d->name == NULL)
		printf("Name: (nil)\n");
	else
		printf("Name: %s\n", d->name);

	printf("Age: %f\n", d->age);

	if (d->owner == NULL)
		printf("Owner: (nil)\n");
	else
		printf("Owner: %s\n", d->owner);
}
