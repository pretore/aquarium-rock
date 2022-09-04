#include <stdlib.h>
#include <string.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

bool rock_tree_map_init(struct rock_tree_map *object,
                        const size_t key,
                        const size_t value,
                        int (*compare)(const void *first,
                                       const void *second)) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_SIZE_IS_ZERO;
        return false;
    }
    if (!value) {
        rock_error = ROCK_TREE_MAP_ERROR_VALUE_SIZE_IS_ZERO;
        return false;
    }
    if (!compare) {
        rock_error = ROCK_TREE_MAP_ERROR_COMPARE_IS_NULL;
        return false;
    }
    (*object) = (struct rock_tree_map) {0};
    seagrass_required_true(rock_red_black_tree_init(
            &object->tree,
            (int (*)(const void *, const void *)) compare));
    object->key = key;
    object->value = value;
    seagrass_required_true(seagrass_uintmax_t_add(
            object->key, object->value, &object->entry));
    seagrass_required_true(SIZE_MAX >= object->entry);
    return true;
}

static _Thread_local struct rock_tree_map *instance;
static _Thread_local void (*on_destroy_callback)(void *key, void *value);

static void rock_tree_map_on_destroy(void *entry) {
    void *key = entry;
    void *value = (void *)((char*)key + instance->key);
    on_destroy_callback(key, value);
}

bool rock_tree_map_invalidate(struct rock_tree_map *object,
                              void (*on_destroy)(void *key, void *value)) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (on_destroy) {
        instance = object;
        on_destroy_callback = on_destroy;
    }
    seagrass_required_true(rock_red_black_tree_invalidate(
            &object->tree, on_destroy ? rock_tree_map_on_destroy : NULL));
    return true;
}

bool rock_tree_map_count(struct rock_tree_map *object, uintmax_t *out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_count(
            &object->tree, out));
    return true;
}

bool rock_tree_map_key_size(struct rock_tree_map *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->key;
    return true;
}

bool rock_tree_map_value_size(struct rock_tree_map *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->value;
    return true;
}

bool rock_tree_map_add(struct rock_tree_map *object, const void *key,
                       void *value) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_IS_NULL;
        return false;
    }
    if (!value) {
        rock_error = ROCK_TREE_MAP_ERROR_VALUE_IS_NULL;
        return false;
    }
    void *insertion_point;
    if (rock_red_black_tree_find(&object->tree,
                                 NULL,
                                 key,
                                 &insertion_point)) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_ALREADY_EXISTS;
        return false;
    }
    seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                           == rock_error);
    void *entry;
    if (!rock_red_black_tree_node(object->entry, (void **) &entry)) {
        seagrass_required_true(
                ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED
                == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    memcpy(entry, key, object->key);
    void *dest = (char *)entry + object->key;
    memcpy(dest, value, object->value);
    seagrass_required_true(rock_red_black_tree_insert(
            &object->tree, insertion_point, entry));
    return true;
}

bool rock_tree_map_remove(struct rock_tree_map *object, const void *key) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_IS_NULL;
        return false;
    }
    char *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_delete(
            &object->tree, entry));
    return true;
}

bool rock_tree_map_set(struct rock_tree_map *object, const void *key,
                       void *value) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_IS_NULL;
        return false;
    }
    if (!value) {
        rock_error = ROCK_TREE_MAP_ERROR_VALUE_IS_NULL;
        return false;
    }
    void *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    void *dest = (char *) entry + object->key;
    memcpy(dest, value, object->value);
    return true;
}

bool rock_tree_map_get(struct rock_tree_map *object, const void *key,
                       void **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!key) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    void *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    *out = (char *)entry + object->key;
    return true;
}

bool rock_tree_map_contains(struct rock_tree_map *object, const void *key,
                            bool *out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    void *entry;
    *out = rock_red_black_tree_find(&object->tree,
                                    NULL,
                                    key,
                                    (void **) &entry);
    if (!*out) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
    }
    return true;
}

bool rock_tree_map_first(struct rock_tree_map *object, void **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_first(&object->tree, (void **) out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY;
    }
    return result;
}

bool rock_tree_map_last(struct rock_tree_map *object, void **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_last(&object->tree, (void **) out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY;
    }
    return result;
}

bool rock_tree_map_next(void *entry, void **out) {
    if (!entry) {
        rock_error = ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_next(entry, (void **) out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE;
    }
    return result;
}

bool rock_tree_map_prev(void *entry, void **out) {
    if (!entry) {
        rock_error = ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_prev(entry, (void **) out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE;
    }
    return result;
}

