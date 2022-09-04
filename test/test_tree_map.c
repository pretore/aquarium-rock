#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <time.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

static void check_invalidate_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init(NULL,
                                    sizeof(void *),
                                    sizeof(void *),
                                    (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_key_size_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init((void *)1,
                                    0,
                                    sizeof(void *),
                                    (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_SIZE_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_value_size_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init((void *)1,
                                    sizeof(void *),
                                    0,
                                    (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_VALUE_SIZE_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_compare_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init((void *)1,
                                    sizeof(void *),
                                    sizeof(void *),
                                    NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_COMPARE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_count(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_count((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object;
    assert_true(rock_tree_map_init(&object,
                                   sizeof(void *),
                                   sizeof(void *),
                                   (void *)1));
    uintmax_t count;
    assert_true(rock_tree_map_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    object.tree.count = 192312;
    assert_true(rock_tree_map_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    rock_error = ROCK_ERROR_NONE;
}

static int tree_map_compare(const void *a, const void *b) {
    return seagrass_uintmax_t_ptr_compare(a, b);
}

static void check_key_size_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_key_size(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_key_size_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_key_size((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_key_size(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object;
    uintmax_t key;
    if (!(rand() % UINTMAX_MAX)) {
        key = sizeof(uintmax_t);
    }
    assert_true(rock_tree_map_init(&object,
                                   key,
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t out;
    assert_true(rock_tree_map_key_size(&object, &out));
    assert_int_equal(out, key);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_value_size_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_value_size(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_value_size_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_value_size((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_value_size(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object;
    uintmax_t value;
    if (!(rand() % UINTMAX_MAX)) {
        value = sizeof(uintmax_t);
    }
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   value,
                                   tree_map_compare));
    uintmax_t out;
    assert_true(rock_tree_map_value_size(&object, &out));
    assert_int_equal(out, value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_add(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    key += 1;
    value += 1;
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_key_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    value += 1;
    assert_false(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_ALREADY_EXISTS, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_remove(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_key_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    assert_false(rock_tree_map_remove(&object, &key));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    assert_true(rock_tree_map_remove(&object, &key));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_set(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_key_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_false(rock_tree_map_set(&object, &key, &value));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    uintmax_t *out = NULL;
    assert_true(rock_tree_map_get(&object, &key, (void **)&out));
    assert_int_equal(*out, value);
    value = 0;
    assert_true(rock_tree_map_set(&object, &key, &value));
    assert_true(rock_tree_map_get(&object, &key, (void **)&out));
    assert_int_equal(*out, 0);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_get(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_get((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    uintmax_t *out;
    assert_true(rock_tree_map_get(&object, &key, (void **)&out));
    assert_int_equal(*out, value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_contains(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_contains((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = (rand() % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    bool result;
    key += 1;
    assert_true(rock_tree_map_contains(&object, &key, &result));
    assert_false(result);
    key -= 1;
    assert_true(rock_tree_map_contains(&object, &key, &result));
    assert_true(result);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_first(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_first((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_empty_tree_map(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    struct {
        uintmax_t key;
        uintmax_t value;
    } entry;
    assert_false(rock_tree_map_first(&object, (void **)&entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_first(&object, (void **)&entry));
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    key -= 1;
    value += 1;
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_first(&object, (void **)&entry));
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_last(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_last((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_empty_tree_map(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_false(rock_tree_map_last(&object, (void **)&entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_last(&object, (void **)&entry));
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    key += 1;
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_last(&object, (void **)&entry));
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_entry_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_next(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_next((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_first(&object, (void **)&entry));
    assert_false(rock_tree_map_next(entry, (void **)&entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    key += 1;
    value += 1;
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(2, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_first(&object, (void **)&entry));
    assert_int_equal(entry->key, key - 1);
    assert_int_equal(entry->value, value - 1);
    assert_true(rock_tree_map_next(entry, (void **)&entry));
    assert_int_equal(entry->key, key);
    assert_int_equal(entry->value, value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_entry_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_prev(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_prev((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(1, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_first(&object, (void **)&entry));
    assert_false(rock_tree_map_prev(entry, (void **)&entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   sizeof(uintmax_t),
                                   sizeof(uintmax_t),
                                   tree_map_compare));
    uintmax_t key = ((1 + rand()) % UINTMAX_MAX);
    uintmax_t value = (rand() % UINTMAX_MAX);
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, &key, &value));
    key += 112;
    value += 1;
    assert_true(rock_tree_map_add(&object, &key, &value));
    assert_int_equal(2, object.tree.count);
    struct {
        uintmax_t key;
        uintmax_t value;
    } *entry;
    assert_true(rock_tree_map_last(&object, (void **)&entry));
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    assert_true(rock_tree_map_prev(entry, (void **)&entry));
    key -= 112;
    value -= 1;
    assert_int_equal(key, entry->key);
    assert_int_equal(value, entry->value);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_key_size_is_zero),
            cmocka_unit_test(check_init_error_on_value_size_is_zero),
            cmocka_unit_test(check_init_error_on_compare_is_null),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_key_size_error_on_object_is_null),
            cmocka_unit_test(check_key_size_error_on_out_is_null),
            cmocka_unit_test(check_key_size),
            cmocka_unit_test(check_value_size_error_on_object_is_null),
            cmocka_unit_test(check_value_size_error_on_out_is_null),
            cmocka_unit_test(check_value_size),
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_on_key_already_exists),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_key_not_found),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_set_error_on_object_is_null),
            cmocka_unit_test(check_set_error_on_key_not_found),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_get_error_on_object_is_null),
            cmocka_unit_test(check_get_error_on_out_is_null),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_contains_error_on_object_is_null),
            cmocka_unit_test(check_contains_error_on_out_is_null),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_empty_tree_map),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_out_is_null),
            cmocka_unit_test(check_last_error_on_empty_tree_map),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_entry_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_entry_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
