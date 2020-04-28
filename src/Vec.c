#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

static void _ensure_capacity(Vec *self, size_t n);

/* Constructor / Destructor */

Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

/* Operations */

void Vec_get(const Vec *self, size_t index, void *out)
{
	if (index >= 0 && index < self->length) {
  		memcpy(out, Vec_ref(self, index), self->item_size);
	} else {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}
}

void Vec_set(Vec *self, size_t index, const void *value)
{
	if (index >= 0 && index < self->length) {
		Vec_splice(self, index, 1, value, 1);
	} else if (index == self->length) {
		Vec_splice(self, index, 0, value, 1);
	} else {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}
}

bool Vec_equals(const Vec *self, const Vec *other)
{
	if (self->length != other->length || self->item_size != other->item_size) {
		return false;
	}	
	return memcmp(self->buffer, other->buffer, self->length * self->item_size) == 0;
}

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count)
{
	// Inserting beyond Vec length only occurs when index>self->length, which this also covers
	if (delete_count < 0 || insert_count < 0 || index+delete_count > self->length) {
        	fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        	exit(EXIT_FAILURE);
	}

	size_t oldLength = self->length;
	size_t newLength = self->length-delete_count+insert_count;

	if (newLength > self->length) {
		_ensure_capacity(self, newLength);
		self->length = newLength; // Necessary to update to higher length for Vec_ref usage later
	}

	// Either moving the last part (block) of the array left to cover deleted items or
	// moving it right to make space for items to be inserted
	size_t blockStart = index+delete_count;
	size_t blockLength = (oldLength-blockStart)*self->item_size;
	size_t shift = insert_count-delete_count;
	if (blockLength > 0) {
		memcpy(Vec_ref(self, blockStart+shift), Vec_ref(self, blockStart), blockLength);
	}

	// Insert from the items array
	memcpy(Vec_ref(self, index), items, insert_count*self->item_size);
	self->length = newLength;
}

/* Helpers */

static void _ensure_capacity(Vec *self, size_t n) 
{
    if (n > self->capacity) {
        size_t new_capacity = n * 2;
        self->buffer = realloc(self->buffer, new_capacity * self->item_size);
        OOM_GUARD(self->buffer, __FILE__, __LINE__);
        self->capacity = new_capacity;
    }
}
