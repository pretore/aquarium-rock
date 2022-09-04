#include <stdlib.h>
#include <memory.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

static _Thread_local struct rock_tree_set *this;

static int rock_tree_set_compare(const void *a, const void *b) {
    return this->compare(*(void **) a, *(void **) b);
}

bool rock_tree_set_init(struct rock_tree_set *object,
                        const size_t size,
                        int (*compare)(const void *,
                                       const void *)) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!size) {
        rock_error = ROCK_TREE_SET_ERROR_SIZE_IS_ZERO;
        return false;
    }
    if (!compare) {
        rock_error = ROCK_TREE_SET_ERROR_COMPARE_IS_NULL;
        return false;
    }
    (*object) = (struct rock_tree_set) {0};
    seagrass_required_true(rock_red_black_tree_init(
            &object->tree,
            rock_tree_set_compare));
    object->compare = compare;
    object->size = size;
    return true;
}

bool rock_tree_set_invalidate(struct rock_tree_set *object,
                              void (*on_destroy)(void *)) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_invalidate(
            &object->tree, on_destroy));
    (*object) = (struct rock_tree_set) {0};
    return true;
}

bool rock_tree_set_size(struct rock_tree_set *object, size_t *out) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->size;
    return true;
}

bool rock_tree_set_count(struct rock_tree_set *object, uintmax_t *out) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_count(
            &object->tree, out));
    return true;
}

bool rock_tree_set_add(struct rock_tree_set *object, void *item) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_IS_NULL;
        return false;
    }
    this = object;
    void *insertion_point;
    if (rock_red_black_tree_find(&object->tree,
                                 NULL,
                                 item,
                                 &insertion_point)) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS;
        return false;
    }
    seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                           == rock_error);
    void *node;
    if (!rock_red_black_tree_node(object->size, &node)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED
                                == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    memcpy(node, item, object->size);
    seagrass_required_true(rock_red_black_tree_insert(
            &object->tree, insertion_point, node));
    return true;
}

bool rock_tree_set_remove(struct rock_tree_set *object, void *item) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_IS_NULL;
        return false;
    }
    this = object;
    void *node;
    if (!rock_red_black_tree_find(&object->tree,
                                  NULL,
                                  item,
                                  &node)) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_ITEM_NOT_FOUND;
        return false;
    }
    seagrass_required_true(rock_red_black_tree_delete(
            &object->tree, node));
    return true;
}

bool rock_tree_set_contains(struct rock_tree_set *object, void *item,
                            bool *out) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    this = object;
    void *at;
    *out = rock_red_black_tree_find(&object->tree,
                                    NULL,
                                    item,
                                    &at);
    if (!*out) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND
                               == rock_error);
    }
    return true;
}

bool rock_tree_set_first(struct rock_tree_set *object, void **out) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_first(&object->tree, out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY;
    }
    return result;
}

bool rock_tree_set_last(struct rock_tree_set *object, void **out) {
    if (!object) {
        rock_error = ROCK_TREE_SET_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_last(&object->tree, out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY
                               == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY;
    }
    return result;
}

bool rock_tree_set_next(void *item, void **out) {
    if (!item) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_next(item, out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_END_OF_SEQUENCE;
    }
    return result;
}

bool rock_tree_set_prev(void *item, void **out) {
    if (!item) {
        rock_error = ROCK_TREE_SET_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_TREE_SET_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = rock_red_black_tree_prev(item, out);
    if (!result) {
        seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                               == rock_error);
        rock_error = ROCK_TREE_SET_ERROR_END_OF_SEQUENCE;
    }
    return result;
}
