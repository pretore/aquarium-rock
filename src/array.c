#include <stdlib.h>
#include <memory.h>
#include <seagrass.h>
#include <rock.h>

#include "private/array.h"
#include "test/cmocka.h"

bool rock_array_init(struct rock_array *const object, const size_t size,
                     const uintmax_t capacity) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!size) {
        rock_error = ROCK_ARRAY_ERROR_SIZE_IS_ZERO;
        return false;
    }
    *object = (struct rock_array) {0};
    object->size = size;
    const bool result = rock_array_set_capacity(object, capacity);
    if (!result) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
    }
    return result;
}

bool rock_array_invalidate(struct rock_array *const object,
                           void (*const on_destroy)(void *)) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (on_destroy) {
        for (uintmax_t i = 0; i < object->length; i++) {
            on_destroy(rock_array_address(object, i));
        }
    }
    free(object->data);
    *object = (struct rock_array) {0};
    return true;
}

bool rock_array_capacity(const struct rock_array *const object,
                         uintmax_t *const out) {
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

bool rock_array_set_capacity(struct rock_array *const object,
                             const uintmax_t capacity) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(object->size > 0);
    uintmax_t new;
    if (capacity <= object->capacity) {
        new = object->size * capacity;
    } else if (!seagrass_uintmax_t_multiply(capacity,
                                            object->size,
                                            &new)) {
        seagrass_required_true(SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                               == seagrass_error);
        rock_error = ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    if (!new) {
        const size_t size = object->size;
        seagrass_required_true(rock_array_invalidate(
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

bool rock_array_get_length(const struct rock_array *const object,
                           uintmax_t *const out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->length;
    return true;
}

bool rock_array_set_length(struct rock_array *const object,
                           const uintmax_t length) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (length > object->capacity
        && !rock_array_adjust_capacity(object, length)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    object->length = length;
    return true;
}

bool rock_array_size(const struct rock_array *const object,
                     size_t *const out) {
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

bool rock_array_shrink(struct rock_array *const object) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(rock_array_set_capacity(
            object, object->length));
    return true;
}

bool rock_array_adjust_capacity(struct rock_array *const object,
                                const uintmax_t count) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    uintmax_t limit;
    if (!seagrass_uintmax_t_add(count, object->length, &limit)) {
        seagrass_required_true(SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                               == seagrass_error);
        rock_error = ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    uintmax_t capacity = object->capacity;
    while (limit > capacity) {
        seagrass_required_true(seagrass_uintmax_t_times_and_a_half_even(
                capacity, &capacity));
    }
    /* apply the calculated capacity increase */
    if (capacity > object->capacity
        && !rock_array_set_capacity(object, capacity)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    return true;
}

bool rock_array_add(struct rock_array *const object,
                    const void *const item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    const void *items[] = {item};
    if (!rock_array_add_all(object, 1, items)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    return true;
}

bool rock_array_add_all(struct rock_array *const object,
                        const uintmax_t count,
                        const void **const items) {
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
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    object->length += count;
    for (uintmax_t i = 0, at = object->length - count; i < count; i++, at++) {
        seagrass_required_true(rock_array_set(object, at, items[i]));
    }
    return true;
}

bool rock_array_remove_last(struct rock_array *const object) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!object->length) {
        rock_error = ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
        return false;
    }
    object->length -= 1;
    return true;
}

void *rock_array_address(const struct rock_array *const object,
                         const uintmax_t at) {
    seagrass_required(object);
    seagrass_required(object->data);
    return ((unsigned char *) object->data) + (object->size * at);
}

bool rock_array_insert(struct rock_array *const object,
                       const uintmax_t at,
                       const void *const item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    const void *items[] = {item};
    if (!rock_array_insert_all(object, at, 1, items)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    return true;
}

bool rock_array_insert_all(struct rock_array *const object,
                           const uintmax_t at,
                           const uintmax_t count,
                           const void **items) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
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
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == rock_error);
        return false;
    }
    memcpy(rock_array_address(object, count + at),
           rock_array_address(object, at),
           object->size * (object->length - at));
    object->length += count;
    for (uintmax_t i = 0; i < count; i++) {
        seagrass_required_true(rock_array_set(object, at + i, items[i]));
    }
    return true;
}

bool rock_array_remove(struct rock_array *const object, const uintmax_t at) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    seagrass_required_true(rock_array_remove_all(object, at, 1));
    return true;
}

bool rock_array_remove_all(struct rock_array *const object, const uintmax_t at,
                           const uintmax_t count) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    if (!count) {
        rock_error = ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
        return false;
    }
    uintmax_t max;
    if (!seagrass_uintmax_t_add(at, count, &max)) {
        seagrass_required_true(SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                               == seagrass_error);
        max = UINTMAX_MAX;
    }
    if (max >= object->length) {
        object->length = at;
    } else {
        const uintmax_t remainder = object->length - count - at;
        memmove(rock_array_address(object, at),
                rock_array_address(object, at + count),
                object->size * remainder);
        object->length -= count;
    }
    return true;
}

bool rock_array_get(const struct rock_array *const object,
                    const uintmax_t at,
                    void **const out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
        rock_error = ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
        return false;
    }
    *out = rock_array_address(object, at);
    return true;
}

bool rock_array_set(struct rock_array *const object, const uintmax_t at,
                    const void *const item) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (at >= object->length) {
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

bool rock_array_first(const struct rock_array *const object,
                      void **const out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->length) {
        rock_error = ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
        return false;
    }
    *out = object->data;
    return true;
}

bool rock_array_last(const struct rock_array *const object,
                     void **const out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->length) {
        rock_error = ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
        return false;
    }
    *out = rock_array_address(object, object->length - 1);
    return true;
}

bool rock_array_next(const struct rock_array *const object,
                     const void *const item, void **const out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        rock_error = ROCK_ARRAY_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->length) {
        rock_error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
        return false;
    }
    const void *begin = object->data;
    const void *end = rock_array_address(object, object->length - 1);
    if (begin > item || end < item) {
        rock_error = ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS;
        return false;
    }
    uintmax_t at;
    seagrass_required_true(seagrass_uintmax_t_subtract(
            (uintptr_t) item, (uintptr_t) begin, &at));
    seagrass_required_true(seagrass_uintmax_t_divide(
            at, object->size, &at, NULL));
    if (!seagrass_uintmax_t_add(1, at, &at)) {
        seagrass_required_true(SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                               == seagrass_error);
        rock_error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
        return false;
    }
    if (!rock_array_get(object, at, out)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS
                               == rock_error);
        rock_error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}

bool rock_array_prev(const struct rock_array *const object,
                     const void *item, void **out) {
    if (!object) {
        rock_error = ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        rock_error = ROCK_ARRAY_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_ARRAY_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->length) {
        rock_error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
        return false;
    }
    const void *begin = object->data;
    const void *end = rock_array_address(object, object->length - 1);
    if (begin > item || end < item) {
        rock_error = ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS;
        return false;
    }
    uintmax_t at;
    seagrass_required_true(seagrass_uintmax_t_subtract(
            (uintptr_t) item, (uintptr_t) begin, &at));
    seagrass_required_true(seagrass_uintmax_t_divide(
            at, object->size, &at, NULL));
    if (!rock_array_get(object, at - 1, out)) {
        seagrass_required_true(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS
                               == rock_error);
        rock_error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}
