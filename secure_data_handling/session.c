#include <stdlib.h>
#include <string.h>
#include "session.h"

session_t *session_create(const char *id, unsigned int uid,
	const unsigned char *data, size_t data_len)
{
	session_t *s;

	s = (session_t *)malloc(sizeof(*s));
	if (!s)
		return (NULL);

	s->id = NULL;
	s->data = NULL;
	s->data_len = 0;
	s->uid = uid;

	if (id) {
		/*
		 * session_t owns its own copy of the id string. The caller's
		 * buffer (e.g. main.c's line[]) is not guaranteed to live
		 * past this call, so we must not alias it.
		 */
		s->id = (char *)malloc(strlen(id) + 1);
		if (!s->id) {
			free(s);
			return (NULL);
		}
		strcpy(s->id, id);
	}

	if (data_len > 0 && data) {
		s->data = (unsigned char *)malloc(data_len);
		if (!s->data) {
			/* partial failure: release everything already owned */
			free(s->id);
			free(s);
			return (NULL);
		}
		memcpy(s->data, data, data_len);
		s->data_len = data_len;
	}

	return (s);
}

int session_set_data(session_t *s, const unsigned char *data, size_t data_len)
{
	unsigned char *tmp;

	if (!s)
		return (0);

	if (data_len == 0) {
		free(s->data);
		s->data = NULL;
		s->data_len = 0;
		return (1);
	}

	/*
	 * Never overwrite s->data with the result of realloc() before
	 * checking it: on failure the original block is still valid and
	 * must remain reachable through s->data, or it leaks.
	 */
	tmp = (unsigned char *)realloc(s->data, data_len);
	if (!tmp)
		return (0);

	s->data = tmp;
	memcpy(s->data, data, data_len);
	s->data_len = data_len;
	return (1);
}

void session_destroy(session_t *s)
{
	if (!s)
		return;
	free(s->id);
	free(s->data);
	free(s);
}
