#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <rock.h>

#include "test/cmocka.h"

static void check_node_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_node_init(NULL));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object = {
            .next = (void *) ~0
    };
    assert_true(rock_forward_list_node_init(&object));
    assert_null(object.next);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_insert(NULL, (void *)1));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_insert((void *)1, NULL));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object;
    assert_true(rock_forward_list_node_init(&object));
    assert_null(object.next);
    struct rock_forward_list_node node;
    assert_true(rock_forward_list_node_init(&node));
    assert_null(object.next);
    assert_true(rock_forward_list_insert(&object, &node));
    assert_ptr_equal(object.next, &node);
    assert_null(node.next);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_remove(NULL, (void *)1));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_remove((void *)1, NULL));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_node_is_not_next_of_object(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object;
    assert_true(rock_forward_list_node_init(&object));
    assert_null(object.next);
    struct rock_forward_list_node node;
    assert_true(rock_forward_list_node_init(&node));
    assert_false(rock_forward_list_remove(&object, &node));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object;
    assert_true(rock_forward_list_node_init(&object));
    struct rock_forward_list_node node;
    assert_true(rock_forward_list_node_init(&node));
    assert_true(rock_forward_list_insert(&object, &node));
    assert_non_null(object.next);
    assert_true(rock_forward_list_remove(&object, &node));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_next(NULL, (void *)1));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_forward_list_next((void *)1, NULL));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object;
    assert_true(rock_forward_list_node_init(&object));
    struct rock_forward_list_node *out;
    assert_false(rock_forward_list_next(&object, &out));
    assert_int_equal(ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_forward_list_node object;
    assert_true(rock_forward_list_node_init(&object));
    struct rock_forward_list_node node;
    assert_true(rock_forward_list_node_init(&node));
    assert_true(rock_forward_list_insert(&object, &node));
    struct rock_forward_list_node *out;
    assert_true(rock_forward_list_next(&object, &out));
    assert_ptr_equal(&node, out);
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_init_error_on_object_is_null),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_insert_error_on_object_is_null),
            cmocka_unit_test(check_insert_error_on_node_is_null),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_node_is_null),
            cmocka_unit_test(check_remove_error_on_node_is_not_next_of_object),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_next_error_on_node_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
