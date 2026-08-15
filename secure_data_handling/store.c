
#include <stdlib.h>
#include <string.h>
#include "store.h"

void store_init(store_t *st)
{
	if (st)
		st->head = NULL;
}

static node_t *node_create(session_t *s)
{
	node_t *n = (node_t *)malloc(sizeof(*n));
	if (!n)
		return (NULL);
	n->sess = s;
	n->next = NULL;
	return (n);
}

/*
 * store_add takes ownership of s unconditionally: on success s is linked
 * into the store; on ANY failure path (bad args, duplicate id, allocation
 * failure) s is destroyed here. Callers (see main.c's cmd_add) do not free
 * s themselves after a failed insert, so ownership must not be left
 * dangling in the caller's hands.
 */
int store_add(store_t *st, session_t *s)
{
	node_t *n, *cur;

	if (!st || !s || !s->id) {
		if (s)
			session_destroy(s);
		return (0);
	}

	cur = st->head;
	while (cur) {
		if (cur->sess && cur->sess->id && strcmp(cur->sess->id, s->id) == 0) {
			session_destroy(s);
			return (0);
		}
		cur = cur->next;
	}

	n = node_create(s);
	if (!n) {
		session_destroy(s);
		return (0);
	}

	n->next = st->head;
	st->head = n;
	return (1);
}

session_t *store_get(store_t *st, const char *id)
{
	node_t *cur;
	if (!st || !id)
		return (NULL);
	cur = st->head;
	while (cur) {
		if (cur->sess && cur->sess->id && strcmp(cur->sess->id, id) == 0)
			return (cur->sess);
		cur = cur->next;
	}
	return (NULL);
}

int store_delete(store_t *st, const char *id, session_t **out)
{
	node_t *cur, *prev;
	if (!st || !id)
		return (0);
	prev = NULL;
	cur = st->head;
	while (cur) {
		if (cur->sess && cur->sess->id && strcmp(cur->sess->id, id) == 0) {
			if (prev)
				prev->next = cur->next;
			else
				st->head = cur->next;
			/*
			 * Ownership handoff: if the caller wants the session
			 * (out != NULL) it becomes the new sole owner and we
			 * must NOT destroy it here. Only when out is NULL
			 * (as main.c's DEL command does) does the store retain
			 * responsibility for freeing it.
			 */
			if (out)
				*out = cur->sess;
			else
				session_destroy(cur->sess);
			free(cur);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

void store_destroy(store_t *st)
{
	node_t *cur, *next;
	if (!st)
		return;
	cur = st->head;
	while (cur) {
		next = cur->next;
		session_destroy(cur->sess);
		free(cur);
		cur = next;
	}
	st->head = NULL;
}
