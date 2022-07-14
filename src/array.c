#include <stdlib.h>
#include <memory.h>
#include <rock.h>

#include "private/array.h"
#include "test/cmocka.h"

bool rock_array_init(struct rock_array *object, const size_t size,
                     const size_t capacity) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!size) {
        rock_error = ROCK_ARRAY_ERROR_SIZE_IS_ZERO;
        return false;
    }
    (*object) = (struct rock_array) {0};
    object->size = size;
    if (!rock_array_set_capacity(object, capacity)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    return true;
}

bool rock_array_invalidate(struct rock_array *object,
                           void (*on_destroy)(void *)) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (on_destroy) {
        for (size_t i = 0; i < object->capacity; i++) {
            on_destroy(rock_array_address(object, i));
        }
    }
    free(object->data);
    (*object) = (struct rock_array) {0};
    return true;
}

bool rock_array_capacity(struct rock_array *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->capacity;
    return true;
}

bool rock_array_set_capacity(struct rock_array *object, const size_t capacity) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    rock_required_true(object->size > 0);
    size_t new;
    if (capacity <= object->capacity) {
        new = object->size * capacity;
    } else if (!rock_multiply_size_t(capacity,
                                     object->size,
                                     &new)) {
        rock_required_true(ROCK_ERROR_OVERFLOW_OCCURRED == rock_error);
        rock_error = ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    if (!new) {
        const size_t size = object->size;
        rock_required_true(rock_array_invalidate(
                object, NULL));
        object->size = size;
        return true;
    }
    void *data = object->data
                 ? realloc(object->data, new)
                 : calloc(1, new);
    if (!data) {
        rock_error = ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    const size_t current = object->size * object->capacity;
    if (current < new) {
        memset(((unsigned char *) data) + current, 0, new - current);
    }
    object->data = data;
    object->capacity = capacity;
    return true;
}

bool rock_array_count(struct rock_array *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->count;
    return true;
}

bool rock_array_size(struct rock_array *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->size;
    return true;
}

bool rock_array_shrink(struct rock_array *object) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    rock_required_true(rock_array_set_capacity(
            object, object->count));
    return true;
}

bool rock_array_adjust_capacity(struct rock_array *object, const size_t count) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    if (SIZE_MAX == object->capacity) {
        rock_error = ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    size_t capacity = 0;
    /* increase capacity by times and a half and then use the next even
     * number */
    for (size_t b = object->capacity; count + object->count > b;) {
        size_t i = b >> 1;
        /* even numbers are desired */
        if ((b & 1) ^ (i & 1)) {
            i += 1;
        }
        if (!rock_add_size_t(b, i, &b)) {
            rock_required_true(ROCK_ERROR_OVERFLOW_OCCURRED == rock_error);
            b = SIZE_MAX;
        } else if (!b) {
            b = 2;
        }
        capacity = b;
    }
    size_t out;
    if (!capacity && !rock_add_size_t(count + object->count,
                                      object->capacity,
                                      &out)) {
        rock_required_true(ROCK_ERROR_OVERFLOW_OCCURRED == rock_error);
        capacity = SIZE_MAX;
    }
    /* apply the calculated capacity increase */
    if (capacity > object->capacity
        && !rock_array_set_capacity(object,
                                    capacity)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    return true;
}

bool rock_array_add(struct rock_array *object, void *item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    void *items[] = {item};
    if (!rock_array_add_all(object, 1, items)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    return true;
}

bool rock_array_add_all(struct rock_array *object, const size_t count,
                        void *items[]) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    if (!items) {
        rock_error = ROCK_ARRAY_ERROR_ITEMS_IS_NULL;
        return false;
    }
    if (!rock_array_adjust_capacity(object, count)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    object->count += count;
    for (size_t i = 0, at = object->count - count; i < count; i++, at++) {
        rock_required_true(rock_array_set(object, at, items[i]));
    }
    return true;
}

bool rock_array_remove_last(struct rock_array *object) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!object->count) {
        rock_error = ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
        return false;
    }
    object->count -= 1;
    return true;
}

void *rock_array_address(struct rock_array *object, const size_t at) {
    rock_required(object);
    rock_required(object->data);
    return ((unsigned char *) object->data) + (object->size * at);
}

bool rock_array_insert(struct rock_array *object, const size_t at, void *item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    void *items[] = {item};
    if (!rock_array_insert_all(object, at, 1, items)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    return true;
}

bool rock_array_insert_all(struct rock_array *object, const size_t at,
                           const size_t count, void *items[]) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    if (!items) {
        rock_error = ROCK_ARRAY_ERROR_ITEMS_IS_NULL;
        return false;
    }
    if (!rock_array_adjust_capacity(object, count)) {
        rock_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                           == rock_error);
        return false;
    }
    memcpy(rock_array_address(object, count + at),
           rock_array_address(object, at),
           object->size * (object->count - at));
    object->count += count;
    for (size_t i = 0; i < count; i++) {
        rock_required_true(rock_array_set(object, at + i, items[i]));
    }
    return true;
}

bool rock_array_remove(struct rock_array *object, const size_t at) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    rock_required_true(rock_array_remove_all(object, at, 1));
    return true;
}

bool rock_array_remove_all(struct rock_array *object, const size_t at,
                           const size_t count) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    size_t max;
    if (!rock_add_size_t(at, count, &max)) {
        rock_required_true(ROCK_ERROR_OVERFLOW_OCCURRED == rock_error);
        max = SIZE_MAX;
    }
    if (max >= object->count) {
        object->count = at;
    } else {
        const size_t remainder = object->count - count - at;
        memmove(rock_array_address(object, at),
                rock_array_address(object, at + count),
                object->size * remainder);
        object->count -= count;
    }
    return true;
}

bool rock_array_get(struct rock_array *object, const size_t at, void **out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    *out = rock_array_address(object, at);
    return true;
}

bool rock_array_set(struct rock_array *object, const size_t at,
                    const void *item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->count) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    void *address = rock_array_address(object, at);
    if (item) {
        memcpy(address, item, object->size);
    } else {
        memset(address, 0, object->size);
    }
    return true;
}
