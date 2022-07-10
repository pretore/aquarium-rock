#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <rock.h>

#include "test/cmocka.h"

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_compare_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_init((void *)1, NULL));
    assert_int_equal(ROCK_TREE_MAP_ERROR_COMPARE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
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
                                   (void *)1));
    size_t count;
    assert_true(rock_tree_map_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    object.tree.count = 192312;
    assert_true(rock_tree_map_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_map_add(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static int tree_map_compare(const struct rock_tree_map_entry_key *a,
                            const struct rock_tree_map_entry_key *b) {
    return rock_compare_size_t((const void *)*(size_t *)a,
                               (const void *)*(size_t *)b);
}

static void check_add(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)1, NULL));
    assert_int_equal(1, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)100, (void *)127));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_key_already_exists(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)1, NULL));
    assert_int_equal(1, object.tree.count);
    assert_false(rock_tree_map_add(&object, (void *)1, (void *)127));
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_false(rock_tree_map_remove(&object, (void *)1));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)191, NULL));
    assert_int_equal(1, object.tree.count);
    assert_true(rock_tree_map_remove(&object, (void *)191));
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_false(rock_tree_map_set(&object, (void *)1, (void *)12));
    assert_int_equal(ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)100, (void *)30));
    assert_int_equal(1, object.tree.count);
    size_t value;
    assert_true(rock_tree_map_get(&object, (void *)100, (void **)&value));
    assert_int_equal(30, value);
    assert_true(rock_tree_map_set(&object, (void *)100, NULL));
    assert_true(rock_tree_map_get(&object, (void *)100, (void **)&value));
    assert_int_equal(0, value);
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)100, (void *)12));
    assert_int_equal(1, object.tree.count);
    size_t value;
    assert_true(rock_tree_map_get(&object, (void *)100, (void **)&value));
    assert_int_equal(12, value);
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)72, (void *)12));
    assert_int_equal(1, object.tree.count);
    bool result;
    assert_true(rock_tree_map_contains(&object, (void *)11, &result));
    assert_false(result);
    assert_true(rock_tree_map_contains(&object, (void *)72, &result));
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    struct rock_tree_map_entry *entry;
    assert_false(rock_tree_map_first(&object, &entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)123, NULL));
    assert_int_equal(1, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_first(&object, &entry));
    assert_int_equal(123, (size_t)entry->key.data);
    assert_null(entry->value.data);
    assert_true(rock_tree_map_add(&object, (void *)77, (void*)1));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_first(&object, &entry));
    assert_int_equal(77, (size_t)entry->key.data);
    assert_int_equal(1, (size_t)entry->value.data);
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
                                   tree_map_compare));
    struct rock_tree_map_entry *entry;
    assert_false(rock_tree_map_last(&object, &entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)77, NULL));
    assert_int_equal(1, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_last(&object, &entry));
    assert_int_equal(77, (size_t)entry->key.data);
    assert_null(entry->value.data);
    assert_true(rock_tree_map_add(&object, (void *)123, (void*)1));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_map_last(&object, &entry));
    assert_int_equal(123, (size_t)entry->key.data);
    assert_int_equal(1, (size_t)entry->value.data);
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)77, NULL));
    assert_int_equal(1, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_first(&object, &entry));
    assert_false(rock_tree_map_next(entry, &entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)77, NULL));
    assert_true(rock_tree_map_add(&object, (void *)123, (void*)1));
    assert_int_equal(2, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_first(&object, &entry));
    assert_int_equal(77, (size_t)entry->key.data);
    assert_int_equal(0, (size_t)entry->value.data);
    assert_true(rock_tree_map_next(entry, &entry));
    assert_int_equal(123, (size_t)entry->key.data);
    assert_int_equal(1, (size_t)entry->value.data);
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
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)77, NULL));
    assert_int_equal(1, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_first(&object, &entry));
    assert_false(rock_tree_map_prev(entry, &entry));
    assert_int_equal(ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_map object = {};
    assert_true(rock_tree_map_init(&object,
                                   tree_map_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_map_add(&object, (void *)77, NULL));
    assert_true(rock_tree_map_add(&object, (void *)123, (void*)1));
    assert_int_equal(2, object.tree.count);
    struct rock_tree_map_entry *entry;
    assert_true(rock_tree_map_last(&object, &entry));
    assert_int_equal(123, (size_t)entry->key.data);
    assert_int_equal(1, (size_t)entry->value.data);
    assert_true(rock_tree_map_prev(entry, &entry));
    assert_int_equal(77, (size_t)entry->key.data);
    assert_int_equal(0, (size_t)entry->value.data);
    assert_true(rock_tree_map_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_compare_is_null),
            cmocka_unit_test(check_invalidate_on_object_is_null),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
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
