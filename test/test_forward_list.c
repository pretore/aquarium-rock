#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <rock.h>

#include <test/cmocka.h>

static void check_node_init_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_node_init(NULL),
            ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_node_init(void **state) {
    struct rock_forward_list_node object = {
            .next = (void *) ~0
    };
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    assert_null(object.next);
}

static void check_insert_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_insert(NULL, (void *) 1),
            ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_insert_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_insert((void *) 1, NULL),
            ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL);
}

static void check_insert(void **state) {
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    assert_null(object.next);
    struct rock_forward_list_node node;
    assert_int_equal(rock_forward_list_node_init(&node), 0);
    assert_null(object.next);
    assert_int_equal(rock_forward_list_insert(&object, &node), 0);
    assert_ptr_equal(object.next, &node);
    assert_null(node.next);
}

static void check_insert_case_prepend(void **state) {
    struct rock_forward_list_node node[2];
    assert_int_equal(rock_forward_list_node_init(&node[0]), 0);
    assert_int_equal(rock_forward_list_node_init(&node[1]), 0);
    assert_int_equal(rock_forward_list_insert(&node[0], &node[1]), 0);
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    assert_int_equal(rock_forward_list_insert(&object, &node[0]), 0);
    assert_ptr_equal(object.next, &node[0]);
    assert_ptr_equal(node[0].next, &node[1]);
    assert_null(node[1].next);
}

static void check_remove_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_remove(NULL, (void *) 1),
            ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_remove_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_remove((void *) 1, NULL),
            ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL);
}

static void check_remove_error_on_node_is_not_next_of_object(void **state) {
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    assert_null(object.next);
    struct rock_forward_list_node node;
    assert_int_equal(rock_forward_list_node_init(&node), 0);
    assert_int_equal(
            rock_forward_list_remove(&object, &node),
            ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT);
}

static void check_remove(void **state) {
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    struct rock_forward_list_node node;
    assert_int_equal(rock_forward_list_node_init(&node), 0);
    assert_int_equal(rock_forward_list_insert(&object, &node), 0);
    assert_non_null(object.next);
    assert_int_equal(rock_forward_list_remove(&object, &node), 0);
}

static void check_next_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_next(NULL, (void *) 1),
            ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL);
}

static void check_next_error_on_out_is_null(void **state) {
    assert_int_equal(
            rock_forward_list_next((void *)1, NULL),
            ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL);
}

static void check_next_error_on_end_of_sequence(void **state) {
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    struct rock_forward_list_node *out;
    assert_int_equal(
            rock_forward_list_next(&object, &out),
            ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE);
}

static void check_next(void **state) {
    struct rock_forward_list_node object;
    assert_int_equal(rock_forward_list_node_init(&object), 0);
    struct rock_forward_list_node node;
    assert_int_equal(rock_forward_list_node_init(&node), 0);
    assert_int_equal(rock_forward_list_insert(&object, &node), 0);
    struct rock_forward_list_node *out;
    assert_int_equal(rock_forward_list_next(&object, &out), 0);
    assert_ptr_equal(&node, out);
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_init_error_on_object_is_null),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_insert_error_on_object_is_null),
            cmocka_unit_test(check_insert_error_on_node_is_null),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_insert_case_prepend),
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
