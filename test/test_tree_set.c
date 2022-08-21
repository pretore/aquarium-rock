#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_init(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_compare_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_init((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_COMPARE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_count(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_count((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object;
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    size_t count;
    assert_true(rock_tree_set_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    object.tree.count = 192312;
    assert_true(rock_tree_set_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_add(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    assert_int_equal(0, object.tree.count);
    assert_null(object.tree.root);
    assert_true(rock_tree_set_add(&object, check_add));
    assert_int_equal(1, object.tree.count);
    assert_non_null(object.tree.root);
    assert_true(rock_tree_set_add(&object, &rock_error));
    assert_int_equal(2, object.tree.count);
    assert_true(rock_tree_set_invalidate(&object,
                                         NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_item_already_exists(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    assert_int_equal(0, object.tree.count);
    assert_null(object.tree.root);
    assert_true(rock_tree_set_add(&object, &rock_error));
    assert_int_equal(1, object.tree.count);
    assert_non_null(object.tree.root);
    assert_false(rock_tree_set_add(&object, &rock_error));
    assert_int_equal(ROCK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS, rock_error);
    assert_true(rock_tree_set_invalidate(&object,
                                         NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_remove(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_item_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    assert_false(rock_tree_set_remove(&object, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_ITEM_NOT_FOUND, rock_error);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_set_add(&object, (void *)1));
    assert_int_equal(1, object.tree.count);
    assert_true(rock_tree_set_remove(&object, (void *)1));
    assert_int_equal(0, object.tree.count);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_contains(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_contains((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   seagrass_void_ptr_compare));
    bool result;
    assert_true(rock_tree_set_contains(&object, (void *)1, &result));
    assert_false(result);
    assert_true(rock_tree_set_add(&object, (void *)1));
    assert_true(rock_tree_set_contains(&object, (void *)1, &result));
    assert_true(result);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_first(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_first((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_empty_tree_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    size_t first;
    assert_false(rock_tree_set_first(&object, (void **)&first));
    assert_int_equal(ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY, rock_error);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)100));
    size_t* first;
    assert_true(rock_tree_set_first(&object, (void **)&first));
    assert_int_equal(*first, 100);
    assert_true(rock_tree_set_add(&object, (void*)10));
    assert_true(rock_tree_set_first(&object, (void **)&first));
    assert_int_equal(*first, 10);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_last(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_last((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_empty_tree_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    size_t first;
    assert_false(rock_tree_set_last(&object, (void **)&first));
    assert_int_equal(ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY, rock_error);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)10));
    size_t* last;
    assert_true(rock_tree_set_last(&object, (void **)&last));
    assert_int_equal(*last, 10);
    assert_true(rock_tree_set_add(&object, (void*)100));
    assert_true(rock_tree_set_last(&object, (void **)&last));
    assert_int_equal(*last, 100);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_next(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_ITEM_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_next((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)10));
    size_t* last;
    assert_true(rock_tree_set_last(&object, (void **)&last));
    assert_false(rock_tree_set_next(last, (void **)&last));
    assert_int_equal(ROCK_TREE_SET_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)10));
    size_t* last;
    assert_true(rock_tree_set_last(&object, (void **)&last));
    assert_true(rock_tree_set_add(&object, (void*)100));
    assert_true(rock_tree_set_next(last, (void **)&last));
    assert_int_equal(*last, 100);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_prev(NULL, (void *)1));
    assert_int_equal(ROCK_TREE_SET_ERROR_ITEM_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_tree_set_prev((void *)1, NULL));
    assert_int_equal(ROCK_TREE_SET_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)10));
    size_t* first;
    assert_true(rock_tree_set_first(&object, (void **)&first));
    assert_false(rock_tree_set_prev(first, (void **)&first));
    assert_int_equal(ROCK_TREE_SET_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_tree_set object = {};
    assert_true(rock_tree_set_init(&object,
                                   (int (*)(const void *, const void *))seagrass_size_t_compare));
    assert_true(rock_tree_set_add(&object, (void*)100));
    size_t* first;
    assert_true(rock_tree_set_first(&object, (void **)&first));
    assert_true(rock_tree_set_add(&object, (void*)10));
    assert_true(rock_tree_set_prev(first, (void **)&first));
    assert_int_equal(*first, 10);
    assert_true(rock_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_compare_is_null),
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_on_item_already_exists),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_item_not_found),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_contains_error_on_object_is_null),
            cmocka_unit_test(check_contains_error_on_out_is_null),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_empty_tree_set),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_out_is_null),
            cmocka_unit_test(check_last_error_on_empty_tree_set),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_item_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_item_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
