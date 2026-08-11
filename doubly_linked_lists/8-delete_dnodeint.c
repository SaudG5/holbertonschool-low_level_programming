#include <stdlib.h>
#include "lists.h"

/**
 * delete_dnodeint_at_index - deletes the node at index of a
 * dlistint_t linked list
 * @head: pointer to the head of the list
 * @index: index of the node that should be deleted, starting at 0
 *
 * Return: 1 if it succeeded, -1 if it failed
 */
int delete_dnodeint_at_index(dlistint_t **head, unsigned int index)
{
	dlistint_t *tmp;
	unsigned int i;

	if (head == NULL || *head == NULL)
		return (-1);

	tmp = *head;

	for (i = 0; i < index; i++)
	{
		if (tmp == NULL)
			return (-1);
		tmp = tmp->next;
	}

	if (tmp == NULL)
		return (-1);

	if (tmp->prev != NULL)
		tmp->prev->next = tmp->next;
	else
		*head = tmp->next;

	if (tmp->next != NULL)
		tmp->next->prev = tmp->prev;

	free(tmp);

	return (1);
}
