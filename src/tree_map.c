#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

bool
rock_tree_map_init(struct rock_tree_map *object,
                   int (*compare)(const struct rock_tree_map_entry_key *,
                                  const struct rock_tree_map_entry_key *)) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
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
    return true;
}

bool
rock_tree_map_invalidate(struct rock_tree_map *object,
                         void (*on_destroy)(struct rock_tree_map_entry *)) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_invalidate(
            &object->tree,
            (void (*)(void *)) on_destroy));
    return true;
}

bool rock_tree_map_count(struct rock_tree_map *object, size_t *out) {
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

bool rock_tree_map_add(struct rock_tree_map *object,
                       const void *key,
                       void *value) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    void *insertion_point;
    if (rock_red_black_tree_find(&object->tree,
                                 NULL,
                                 &key,
                                 &insertion_point)) {
        rock_error = ROCK_TREE_MAP_ERROR_KEY_ALREADY_EXISTS;
        return false;
    }
    seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                           == rock_error);
    struct rock_tree_map_entry *entry;
    if (!rock_red_black_tree_node(sizeof(*entry),
                                  (void **) &entry)) {
        seagrass_required_true(
                ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED
                == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    entry->key.data = key;
    entry->value.data = value;
    seagrass_required_true(rock_red_black_tree_insert(
            &object->tree,
            insertion_point,
            entry));
    return true;
}

bool rock_tree_map_remove(struct rock_tree_map *object,
                          const void *key) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    struct rock_tree_map_entry *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  &key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_delete(
            &object->tree,
            entry));
    return true;
}

bool rock_tree_map_set(struct rock_tree_map *object,
                       const void *key,
                       void *value) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    struct rock_tree_map_entry *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  &key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    entry->value.data = value;
    return true;
}

bool rock_tree_map_get(struct rock_tree_map *object,
                       const void *key,
                       void **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_tree_map_entry *entry;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  &key,
                                  (void **) &entry)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND;
        return false;
    }
    *out = entry->value.data;
    return true;
}

bool rock_tree_map_contains(struct rock_tree_map *object,
                            const void *key,
                            bool *out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_tree_map_entry *entry;
    *out = rock_red_black_tree_find(&object->tree,
                                    NULL,
                                    &key,
                                    (void **) &entry);
    if (!*out) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
    }
    return true;
}

bool rock_tree_map_first(struct rock_tree_map *object,
                         struct rock_tree_map_entry **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!rock_red_black_tree_first(&object->tree, (void **) out)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY;
        return false;
    }
    return true;
}

bool rock_tree_map_last(struct rock_tree_map *object,
                        struct rock_tree_map_entry **out) {
    if (!object) {
        rock_error = ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!rock_red_black_tree_last(&object->tree, (void **) out)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY;
        return false;
    }
    return true;
}

bool rock_tree_map_next(struct rock_tree_map_entry *entry,
                        struct rock_tree_map_entry **out) {
    if (!entry) {
        rock_error = ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!rock_red_black_tree_next(entry, (void **) out)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}

bool rock_tree_map_prev(struct rock_tree_map_entry *entry,
                        struct rock_tree_map_entry **out) {
    if (!entry) {
        rock_error = ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_MAP_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!rock_red_black_tree_prev(entry, (void **) out)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE;
        return false;
    }
    return true;
}

