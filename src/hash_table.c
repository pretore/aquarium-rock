#include <stdlib.h>
#include <math.h>
#include <seagrass.h>
#include <rock.h>

#include "private/hash_table.h"
#include "test/cmocka.h"

bool rock_hash_table_init(struct rock_hash_table *object,
                          float load_factor,
                          size_t (*hash_code)(const void *),
                          bool (*is_equal)(const void *, const void *)) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!hash_code) {
        rock_error = ROCK_HASH_TABLE_ERROR_HASH_CODE_IS_NULL;
        return false;
    }
    if (!is_equal) {
        rock_error = ROCK_HASH_TABLE_ERROR_IS_EQUAL_IS_NULL;
        return false;
    }
    float lf = fabsf(load_factor);
    bool result;
    seagrass_required_true(seagrass_float_is_greater_than(
            lf, ROCK_HASH_TABLE_LOAD_FACTOR_MAXIMUM, &result));
    if (result) {
        rock_error = ROCK_HASH_TABLE_ERROR_LOAD_FACTOR_IS_INVALID;
        return false;
    }
    seagrass_required_true(seagrass_float_is_equal(
            ROCK_HASH_TABLE_LOAD_FACTOR_UNSPECIFIED, lf, &result));
    if (result) {
        lf = ROCK_HASH_TABLE_LOAD_FACTOR_DEFAULT;
    }
    (*object) = (struct rock_hash_table) {0};
    seagrass_required_true(rock_array_init(
            &object->array, sizeof(struct rock_hash_table_entry), 0));
    object->lf = lf;
    object->hash_code = hash_code;
    object->is_equal = is_equal;
    return true;
}

static _Thread_local void (*on_destroy_callback)(struct rock_hash_table_entry *);

static void rock_hash_table_on_destroy(void *item) {
    struct rock_hash_table_entry *entry = (struct rock_hash_table_entry *) item;
    if (!entry->key.data) {
        return;
    }
    on_destroy_callback(entry);
}

bool
rock_hash_table_invalidate(struct rock_hash_table *object,
                           void (*on_destroy)(struct rock_hash_table_entry *)) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (on_destroy) {
        on_destroy_callback = on_destroy;
    }
    seagrass_required_true(rock_array_invalidate(
            &object->array,
            on_destroy ? rock_hash_table_on_destroy : NULL));
    (*object) = (struct rock_hash_table) {0};
    return true;
}

bool rock_hash_table_count(struct rock_hash_table *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->count;
    return true;
}

bool rock_hash_table_rebuild_needed(struct rock_hash_table *object,
                                    bool *out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    size_t limit;
    seagrass_required_true(seagrass_float_to_size_t(
            (float) length * 0.1f,
            SEAGRASS_FLOAT_ROUNDING_MODE_UPWARD,
            &limit));
    *out = object->deleted > limit && object->count < object->deleted;
    return true;
}

bool rock_hash_table_rebuild(struct rock_hash_table *object) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    for (size_t i = 0; i < length; i++) {
        struct rock_hash_table_entry *entry;
        seagrass_required_true(rock_array_get(
                &object->array, i, (void **) &entry));
        if (entry->key.data) {
            do {
                const size_t at =
                        object->hash_code(entry->key.data) % length;
                if (at < i) {
                    seagrass_required_true(rock_hash_table_insert(
                            object, entry, true));
                    /* clear out duplicate entry */
                    *entry = ROCK_HASH_TABLE_ENTRY_EMPTY;
                    break;
                } else if (at == i) {
                    break;
                } else {
                    struct rock_hash_table_entry *other;
                    seagrass_required_true(rock_array_get(
                            &object->array, at, (void **) &other));
                    rock_hash_table_entry_swap(entry, other);
                }
            } while (entry->key.data);
        }
        /* remove deleted entry */
        if (rock_hash_table_entry_equals(entry,
                                         &ROCK_HASH_TABLE_ENTRY_DELETED)) {
            *entry = ROCK_HASH_TABLE_ENTRY_EMPTY;
            object->deleted -= 1;
        }
    }
    return true;
}

void rock_hash_table_entry_swap(struct rock_hash_table_entry *restrict a,
                                struct rock_hash_table_entry *restrict b) {
    seagrass_required_true(a != b);
    seagrass_required(a);
    seagrass_required(b);
    struct rock_hash_table_entry tmp = *a;
    *a = *b;
    *b = tmp;
}

bool
rock_hash_table_entry_equals(const struct rock_hash_table_entry *restrict a,
                             const struct rock_hash_table_entry *restrict b) {
    seagrass_required_true(a != b);
    seagrass_required(a);
    seagrass_required(b);
    return a->key.data == b->key.data
           && a->value.data == b->value.data;
}

bool rock_hash_table_insert(struct rock_hash_table *object,
                            struct rock_hash_table_entry *entry,
                            bool ignore_duplicates) {
    seagrass_required(object);
    seagrass_required(entry);
    seagrass_required(entry->key.data);

    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    const size_t at = object->hash_code(entry->key.data) % length;

    for (size_t i = 0; i < length; i++) {
        struct rock_hash_table_entry *dst;
        seagrass_required_true(rock_array_get(&object->array,
                                              (at + i) % length,
                                              (void **) &dst));
        if (!dst->key.data) {
            *dst = *entry;
            return true;
        }
        if (!ignore_duplicates
            && object->is_equal(entry->key.data, dst->key.data)) {
            rock_error = ROCK_HASH_TABLE_ERROR_KEY_ALREADY_EXISTS;
            return false;
        }
    }
    rock_error = ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_FULL;
    return false;
}

bool rock_hash_table_add(struct rock_hash_table *object,
                         const void *key,
                         void *value) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_IS_NULL;
        return false;
    }
    /* check if we need to resize the hash table */
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    size_t used;
    if (!seagrass_size_t_add(1, object->count, &used)) {
        seagrass_required_true(SEAGRASS_SIZE_T_ERROR_RESULT_IS_INCONSISTENT
                               == seagrass_error);
        rock_error = ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    size_t limit;
    const size_t current = length;
    do {
        seagrass_required_true(seagrass_float_to_size_t(
                (float) length * object->lf,
                SEAGRASS_FLOAT_ROUNDING_MODE_UPWARD,
                &limit));
        if (limit > used) {
            break;
        }
        if (!seagrass_size_t_add(used, length, &length)) {
            seagrass_required_true(SEAGRASS_SIZE_T_ERROR_RESULT_IS_INCONSISTENT
                                   == seagrass_error);
            if (length < SIZE_MAX) {
                length = SIZE_MAX;
                continue;
            }
            rock_error = ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED;
            return false;
        }
    } while (true);

    /* resize and rebuild */
    if (current < length) {
        if (!rock_array_set_length(&object->array, length)) {
            rock_error = ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED;
            return false;
        }
        seagrass_required_true(rock_hash_table_rebuild(object));
    }

    struct rock_hash_table_entry entry = {
            .key.data = key,
            .value.data = value
    };
    if (!rock_hash_table_insert(object, &entry, false)) {
        seagrass_required_true(ROCK_HASH_TABLE_ERROR_KEY_ALREADY_EXISTS
                               == rock_error);
        return false;
    }
    object->count = used;
    return true;
}

bool rock_hash_table_entry_find(struct rock_hash_table *object,
                                const void *key,
                                struct rock_hash_table_entry **out) {
    seagrass_required(object);
    seagrass_required(key);
    seagrass_required(out);
    if (!object->count) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND;
        return false;
    }
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    size_t at = object->hash_code(key) % length;
    size_t i;
    struct rock_hash_table_entry *entry;
    for (i = 0; i < length; i++) {
        seagrass_required_true(rock_array_get(&object->array,
                                              (at + i) % length,
                                              (void **) &entry));
        if (rock_hash_table_entry_equals(entry,
                                         &ROCK_HASH_TABLE_ENTRY_DELETED)) {
            continue;
        }
        if (!entry->key.data
            || object->is_equal(key, entry->key.data)) {
            break;
        }
    }
    if (!entry->key.data || length == i) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND;
        return false;
    }
    *out = entry;
    return true;
}

bool rock_hash_table_remove(struct rock_hash_table *object,
                            const void *key) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *entry;
    if (!rock_hash_table_entry_find(object, key, &entry)) {
        seagrass_required_true(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND
                               == rock_error);
        return false;
    }
    /* delete entry */
    *entry = ROCK_HASH_TABLE_ENTRY_DELETED;
    object->count -= 1;
    object->deleted += 1;
    return true;
}

bool rock_hash_table_set(struct rock_hash_table *object,
                         const void *key,
                         void *value) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *entry;
    if (!rock_hash_table_entry_find(object, key, &entry)) {
        seagrass_required_true(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND
                               == rock_error);
        return false;
    }
    entry->value.data = value;
    return true;
}

bool rock_hash_table_get(struct rock_hash_table *object,
                         const void *key,
                         void **out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *entry;
    if (!rock_hash_table_entry_find(object, key, &entry)) {
        seagrass_required_true(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND
                               == rock_error);
        return false;
    }
    *out = entry->value.data;
    return true;
}

bool rock_hash_table_contains(struct rock_hash_table *object,
                              const void *key,
                              bool *out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_HASH_TABLE_ERROR_KEY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *entry;
    *out = rock_hash_table_entry_find(object, key, &entry);
    if (!*out) {
        seagrass_required_true(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND
                               == rock_error);
    }
    return true;
}

bool rock_hash_table_first(struct rock_hash_table *object,
                           struct rock_hash_table_entry **out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->count) {
        rock_error = ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY;
        return false;
    }
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    struct rock_hash_table_entry *entry;
    for (size_t i = 0; i < length; i++) {
        seagrass_required_true(rock_array_get(
                &object->array, i, (void **) &entry));
        if (entry->key.data) {
            *out = entry;
            return true;
        }
    }
    /* inconsistent state; abort */
    seagrass_required_true(false);
    return false;
}

bool rock_hash_table_last(struct rock_hash_table *object,
                          struct rock_hash_table_entry **out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->count) {
        rock_error = ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY;
        return false;
    }
    size_t length;
    seagrass_required_true(rock_array_get_length(
            &object->array, &length));
    struct rock_hash_table_entry *entry;
    for (size_t i = length; i; i--) {
        seagrass_required_true(rock_array_get(
                &object->array, i - 1, (void **) &entry));
        if (entry->key.data) {
            *out = entry;
            return true;
        }
    }
    /* inconsistent state; abort */
    seagrass_required_true(false);
    return false;
}

bool rock_hash_table_next(struct rock_hash_table *object,
                          struct rock_hash_table_entry *entry,
                          struct rock_hash_table_entry **out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!entry) {
        rock_error = ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *i = entry;
    do {
        if (!rock_array_next(&object->array, i, (void **) &i)) {
            seagrass_required_true(
                    ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS == rock_error
                    || ROCK_ARRAY_ERROR_END_OF_SEQUENCE == rock_error);
            if (ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS == rock_error) {
                rock_error = ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND;
            } else if (ROCK_ARRAY_ERROR_END_OF_SEQUENCE == rock_error) {
                rock_error = ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE;
            }
            return false;
        }
        if (i->key.data) {
            *out = i;
            return true;
        }
    } while (true);
}

bool rock_hash_table_prev(struct rock_hash_table *object,
                          struct rock_hash_table_entry *entry,
                          struct rock_hash_table_entry **out) {
    if (!object) {
        rock_error = ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!entry) {
        rock_error = ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_HASH_TABLE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_hash_table_entry *i = entry;
    do {
        if (!rock_array_prev(&object->array, i, (void **) &i)) {
            seagrass_required_true(
                    ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS == rock_error
                    || ROCK_ARRAY_ERROR_END_OF_SEQUENCE == rock_error);
            if (ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS == rock_error) {
                rock_error = ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND;
            } else if (ROCK_ARRAY_ERROR_END_OF_SEQUENCE == rock_error) {
                rock_error = ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE;
            }
            return false;
        }
        if (i->key.data) {
            *out = i;
            return true;
        }
    } while (true);
}
