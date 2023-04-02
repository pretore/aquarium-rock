#include <stdlib.h>
#include <string.h>
#include <seagrass.h>
#include <rock.h>

#include "private/array.h"

#ifdef TEST
#include <test/cmocka.h>
#endif

int rock_array_init(struct rock_array *const object,
                    const size_t size,
                    const uintmax_t capacity) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!size) {
        return ROCK_ARRAY_ERROR_SIZE_IS_ZERO;
    }
    *object = (struct rock_array) {
        .size = size,
    };
    int error;
    if ((error = rock_array_set_capacity(object, capacity))) {
        seagrass_required_true(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                               == error);
    }
    return error;
}

int rock_array_invalidate(struct rock_array *const object,
                          void (*const on_destroy)(void *)) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (on_destroy) {
        for (uintmax_t i = 0; i < object->length; i++) {
            on_destroy(rock_array_address(object, i));
        }
    }
    free(object->data);
    *object = (struct rock_array) {0};
    return 0;
}

int rock_array_capacity(const struct rock_array *const object,
                        uintmax_t *const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    *out = object->capacity;
    return 0;
}

int rock_array_set_capacity(struct rock_array *const object,
                            const uintmax_t capacity) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    seagrass_required_true(object->size > 0);
    int error;
    uintmax_t new;
    if (capacity <= object->capacity) {
        new = object->size * capacity;
    } else if ((error = seagrass_uintmax_t_multiply(capacity,
                                                    object->size,
                                                    &new))) {
        seagrass_required_true(
                SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                == error);
        return ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    if (!new) {
        const size_t size = object->size;
        rock_array_invalidate(object, NULL);
        object->size = size;
        return 0;
    }
    void *data = object->data
                 ? realloc(object->data, new)
                 : calloc(1, new);
    if (!data) {
        return ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    const size_t current = object->size * object->capacity;
    if (current < new) {
        memset(((unsigned char *) data) + current, 0, new - current);
    }
    object->data = data;
    object->capacity = capacity;
    return 0;
}

int rock_array_get_length(const struct rock_array *const object,
                          uintmax_t *const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    *out = object->length;
    return 0;
}

int rock_array_set_length(struct rock_array *const object,
                          const uintmax_t length) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    int error;
    if (length > object->capacity
        && (error = rock_array_adjust_capacity(object, length))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
        return error;
    }
    object->length = length;
    return 0;
}

int rock_array_size(const struct rock_array *const object,
                    size_t *const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    *out = object->size;
    return 0;
}

int rock_array_shrink(struct rock_array *const object) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    seagrass_required_true(!rock_array_set_capacity(
            object, object->length));
    return 0;
}

int rock_array_adjust_capacity(struct rock_array *const object,
                               const uintmax_t count) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!count) {
        return ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
    }
    int error;
    uintmax_t limit;
    if ((error = seagrass_uintmax_t_add(count, object->length, &limit))) {
        seagrass_required_true(
                SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                == error);
        return ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    uintmax_t capacity = object->capacity;
    while (limit > capacity) {
        seagrass_required_true(!seagrass_uintmax_t_times_and_a_half_even(
                capacity, &capacity));
    }
    /* apply the calculated capacity increase */
    if (capacity > object->capacity
        && (error = rock_array_set_capacity(object, capacity))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
    }
    return error;
}

int rock_array_add(struct rock_array *const object,
                   const void *const item) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    int error;
    const void *items[] = {item};
    if ((error = rock_array_add_all(object, 1, items))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
    }
    return error;
}

int rock_array_add_all(struct rock_array *const object,
                       const uintmax_t count,
                       const void **const items) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!count) {
        return ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
    }
    if (!items) {
        return ROCK_ARRAY_ERROR_ITEMS_IS_NULL;
    }
    int error;
    if ((error = rock_array_adjust_capacity(object, count))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
        return error;
    }
    object->length += count;
    for (uintmax_t i = 0, at = object->length - count; i < count; i++, at++) {
        seagrass_required_true(!rock_array_set(object, at, items[i]));
    }
    return 0;
}

int rock_array_remove_last(struct rock_array *const object) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
    }
    object->length -= 1;
    return 0;
}

void *rock_array_address(const struct rock_array *const object,
                         const uintmax_t at) {
    seagrass_required(object);
    seagrass_required(object->data);
    return ((unsigned char *) object->data) + (object->size * at);
}

int rock_array_insert(struct rock_array *const object,
                      const uintmax_t at,
                      const void *const item) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    int error;
    const void *items[] = {item};
    if ((error = rock_array_insert_all(object, at, 1, items))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
    }
    return error;
}

int rock_array_insert_all(struct rock_array *const object,
                          const uintmax_t at,
                          const uintmax_t count,
                          const void **const items) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    if (!count) {
        return ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
    }
    if (!items) {
        return ROCK_ARRAY_ERROR_ITEMS_IS_NULL;
    }
    int error;
    if ((error = rock_array_adjust_capacity(object, count))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED
                == error);
        return error;
    }
    memcpy(rock_array_address(object, count + at),
           rock_array_address(object, at),
           object->size * (object->length - at));
    object->length += count;
    for (uintmax_t i = 0; i < count; i++) {
        seagrass_required_true(!rock_array_set(object, at + i, items[i]));
    }
    return 0;
}

int rock_array_remove(struct rock_array *const object,
                      const uintmax_t at) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    seagrass_required_true(!rock_array_remove_all(object, at, 1));
    return 0;
}

int rock_array_remove_all(struct rock_array *const object,
                          const uintmax_t at,
                          const uintmax_t count) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    if (!count) {
        return ROCK_ARRAY_ERROR_COUNT_IS_ZERO;
    }
    int error;
    uintmax_t max;
    if ((error = seagrass_uintmax_t_add(at, count, &max))) {
        seagrass_required_true(
                SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                == error);
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
    return 0;
}

int rock_array_get(const struct rock_array *const object,
                   const uintmax_t at,
                   void **const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    *out = rock_array_address(object, at);
    return 0;
}

int rock_array_set(struct rock_array *const object,
                   const uintmax_t at,
                   const void *const item) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (at >= object->length) {
        return ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS;
    }
    void *address = rock_array_address(object, at);
    if (item) {
        memcpy(address, item, object->size);
    } else {
        memset(address, 0, object->size);
    }
    return 0;
}

int rock_array_first(const struct rock_array *const object,
                     void **const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
    }
    *out = object->data;
    return 0;
}

int rock_array_last(const struct rock_array *const object,
                    void **const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY;
    }
    *out = rock_array_address(object, object->length - 1);
    return 0;
}

int rock_array_next(const struct rock_array *const object,
                    const void *const item,
                    void **const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!item) {
        return ROCK_ARRAY_ERROR_ITEM_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
    }
    int error;
    uintmax_t at;
    if ((error = rock_array_at(object, item, &at))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS
                == error);
        return error;
    }
    if ((error = seagrass_uintmax_t_add(1, at, &at))) {
        seagrass_required_true(
                SEAGRASS_UINTMAX_T_ERROR_RESULT_IS_INCONSISTENT
                == error);
        return ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
    }
    if ((error = rock_array_get(object, at, out))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS
                == error);
        return ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
    }
    return 0;
}

int rock_array_prev(const struct rock_array *const object,
                    const void *const item,
                    void **const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!item) {
        return ROCK_ARRAY_ERROR_ITEM_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
    }
    int error;
    uintmax_t at;
    if ((error = rock_array_at(object, item, &at))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS
                == error);
        return error;
    }
    if ((error = rock_array_get(object, at - 1, out))) {
        seagrass_required_true(
                ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS
                == error);
        error = ROCK_ARRAY_ERROR_END_OF_SEQUENCE;
    }
    return error;
}

int rock_array_at(const struct rock_array *const object,
                  const void *const item,
                  uintmax_t *const out) {
    if (!object) {
        return ROCK_ARRAY_ERROR_OBJECT_IS_NULL;
    }
    if (!item) {
        return ROCK_ARRAY_ERROR_ITEM_IS_NULL;
    }
    if (!out) {
        return ROCK_ARRAY_ERROR_OUT_IS_NULL;
    }
    if (!object->length) {
        return ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS;
    }
    const void *begin = object->data;
    const void *end = rock_array_address(object, object->length - 1);
    if (begin > item || end < item) {
        return ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS;
    }
    uintmax_t address;
    seagrass_required_true(!seagrass_uintmax_t_subtract(
            (uintptr_t) item, (uintptr_t) begin, &address));
    uintmax_t qr[2];
    seagrass_required_true(!seagrass_uintmax_t_divide(
            address, object->size, &qr[0], &qr[1]));
    seagrass_required_true(!qr[1]);
    *out = qr[0];
    return 0;
}
