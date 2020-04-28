#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Str.h"
#include "Vec.h"

static char NULL_CHAR = '\0';

Str Str_value(size_t capacity)
{
    Str s = Vec_value(capacity + 1, sizeof(char));
    Vec_set(&s, 0, &NULL_CHAR);
    return s;
}

void Str_drop(Str *self)
{
    Vec_drop(self);
}

size_t Str_length(const Str *self)
{
    return Vec_length(self) - 1;
}

const char* Str_cstr(const Str *self)
{
    return (char*) Vec_ref(self, 0);
}

char* Str_ref(const Str *self, const size_t index)
{
    return (char*) Vec_ref(self, index);
}

Str Str_from(const char *cstr)
{
	Str s = Str_value(0);
	size_t i = 0;
	while (cstr[i] != NULL_CHAR) {
		Vec_splice(&s, i, 0, cstr+i, 1);
		i++;
	}

	return s;
}

void Str_splice(Str *self, size_t index, size_t delete_count, const char *cstr, size_t insert_count)
{
	// Extra check necessary to make sure caller doesn't attempt to delete null terminator
	if (index+delete_count > Str_length(self)) {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}

	size_t len = 0;
	const char *current = cstr;
	while (current != NULL && *current != NULL_CHAR) {
		len++;
		current++;
	}

	// Likely unnecessary but an extra precaution to prevent a second null terminator from being
	// spliced into the middle of a Str
	if (insert_count > len) {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}

	Vec_splice(self, index, delete_count, cstr, insert_count);
}

void Str_append(Str *self, const char *cstr)
{
	while (*cstr != NULL_CHAR) {
		Vec_splice(self, Str_length(self), 0, cstr, 1);
		cstr++;
	}
}

char Str_get(const Str *self, size_t index)
{
	// Extra check necessary to make sure caller doesn't attempt to modify null terminator
	if (index >= Str_length(self)) {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}
	
	char out;
	Vec_get(self, index, &out);
	return out;
}

void Str_set(Str *self, size_t index, const char value)
{
	if (index > Str_length(self)) {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	} else if (index == Str_length(self)) {
		Vec_splice(self, Str_length(self), 0, &value, 1);
	} else {
		Vec_set(self, index, &value);
	}
}
