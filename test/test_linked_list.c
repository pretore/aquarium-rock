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
            rock_linked_list_node_init(NULL),
            ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_node_init(void **state) {
    struct rock_linked_list_node node;
    assert_int_equal(rock_linked_list_node_init(&node), 0);
    assert_ptr_equal(node.next, &node);
    assert_ptr_equal(node.prev, &node);
}

static void check_insert_after_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_insert_after(NULL, (void *) 1),
            ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_insert_after_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_insert_after((void *) 1, NULL),
            ROCK_LINKED_LIST_ERROR_NODE_IS_NULL);
}

static void check_insert_after(void **state) {
    struct rock_linked_list_node A, B;
    assert_int_equal(rock_linked_list_node_init(&A), 0);
    assert_int_equal(rock_linked_list_node_init(&B), 0);
    assert_int_equal(rock_linked_list_insert_after(&A, &B), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &B);
    assert_ptr_equal(B.next, &A);
    assert_ptr_equal(B.prev, &A);
    struct rock_linked_list_node C;
    assert_int_equal(rock_linked_list_node_init(&C), 0);
    assert_int_equal(rock_linked_list_insert_after(&A, &C), 0);
    assert_ptr_equal(A.next, &C);
    assert_ptr_equal(A.prev, &B);
    assert_ptr_equal(C.prev, &A);
    assert_ptr_equal(C.next, &B);
    assert_ptr_equal(B.next, &A);
    assert_ptr_equal(B.prev, &C);
}

static void check_insert_before_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_insert_before(NULL, (void *) 1),
            ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_insert_before_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_insert_before((void *) 1, NULL),
            ROCK_LINKED_LIST_ERROR_NODE_IS_NULL);
}

static void check_insert_before(void **state) {
    struct rock_linked_list_node A, B;
    assert_int_equal(rock_linked_list_node_init(&A), 0);
    assert_int_equal(rock_linked_list_node_init(&B), 0);
    assert_int_equal(rock_linked_list_insert_before(&A, &B), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &B);
    assert_ptr_equal(B.next, &A);
    assert_ptr_equal(B.prev, &A);
    struct rock_linked_list_node C;
    assert_int_equal(rock_linked_list_node_init(&C), 0);
    assert_int_equal(rock_linked_list_insert_before(&A, &C), 0);
    assert_ptr_equal(A.prev, &C);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(B.next, &C);
    assert_ptr_equal(B.prev, &A);
    assert_ptr_equal(C.next, &A);
    assert_ptr_equal(C.prev, &B);
}

static void check_remove_error_on_object_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_remove(NULL),
            ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL);
}

static void check_remove(void **state) {
    struct rock_linked_list_node A, B, C;
    assert_int_equal(rock_linked_list_node_init(&A), 0);
    assert_int_equal(rock_linked_list_node_init(&B), 0);
    assert_int_equal(rock_linked_list_node_init(&C), 0);
    assert_int_equal(rock_linked_list_insert_after(&A, &B), 0);
    assert_int_equal(rock_linked_list_insert_after(&B, &C), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &C);
    assert_ptr_equal(B.next, &C);
    assert_ptr_equal(B.prev, &A);
    assert_ptr_equal(C.next, &A);
    assert_ptr_equal(C.prev, &B);
    assert_int_equal(rock_linked_list_remove(&C), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &B);
    assert_ptr_equal(B.next, &A);
    assert_ptr_equal(B.prev, &A);
    assert_ptr_equal(C.next, &C);
    assert_ptr_equal(C.prev, &C);
    assert_int_equal(rock_linked_list_remove(&A), 0);
    assert_ptr_equal(A.next, &A);
    assert_ptr_equal(A.prev, &A);
    assert_ptr_equal(B.next, &B);
    assert_ptr_equal(B.prev, &B);
    assert_ptr_equal(C.next, &C);
    assert_ptr_equal(C.prev, &C);
}

static void check_next_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_next(NULL, (void *) 1),
            ROCK_LINKED_LIST_ERROR_NODE_IS_NULL);
}

static void check_next_error_on_out_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_next((void *) 1, NULL),
            ROCK_LINKED_LIST_ERROR_OUT_IS_NULL);
}

static void check_next(void **state) {
    struct rock_linked_list_node A, B, C;
    assert_int_equal(rock_linked_list_node_init(&A), 0);
    assert_int_equal(rock_linked_list_node_init(&B), 0);
    assert_int_equal(rock_linked_list_node_init(&C), 0);
    assert_int_equal(rock_linked_list_insert_after(&A, &B), 0);
    assert_int_equal(rock_linked_list_insert_after(&B, &C), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &C);
    assert_ptr_equal(B.next, &C);
    assert_ptr_equal(B.prev, &A);
    assert_ptr_equal(C.next, &A);
    assert_ptr_equal(C.prev, &B);
    struct rock_linked_list_node *out;
    assert_int_equal(rock_linked_list_next(&A, &out), 0);
    assert_ptr_equal(out, &B);
    assert_int_equal(rock_linked_list_next(&B, &out), 0);
    assert_ptr_equal(out, &C);
    assert_int_equal(rock_linked_list_next(&C, &out), 0);
    assert_ptr_equal(out, &A);
}

static void check_prev_error_on_node_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_prev(NULL, (void *) 1),
            ROCK_LINKED_LIST_ERROR_NODE_IS_NULL);
}

static void check_prev_error_on_out_is_null(void **state) {
    assert_int_equal(
            rock_linked_list_prev((void *) 1, NULL),
            ROCK_LINKED_LIST_ERROR_OUT_IS_NULL);
}

static void check_prev(void **state) {
    struct rock_linked_list_node A, B, C;
    assert_int_equal(rock_linked_list_node_init(&A), 0);
    assert_int_equal(rock_linked_list_node_init(&B), 0);
    assert_int_equal(rock_linked_list_node_init(&C), 0);
    assert_int_equal(rock_linked_list_insert_after(&A, &B), 0);
    assert_int_equal(rock_linked_list_insert_after(&B, &C), 0);
    assert_ptr_equal(A.next, &B);
    assert_ptr_equal(A.prev, &C);
    assert_ptr_equal(B.next, &C);
    assert_ptr_equal(B.prev, &A);
    assert_ptr_equal(C.next, &A);
    assert_ptr_equal(C.prev, &B);
    struct rock_linked_list_node *out;
    assert_int_equal(rock_linked_list_prev(&A, &out), 0);
    assert_ptr_equal(out, &C);
    assert_int_equal(rock_linked_list_prev(&C, &out), 0);
    assert_ptr_equal(out, &B);
    assert_int_equal(rock_linked_list_prev(&B, &out), 0);
    assert_ptr_equal(out, &A);
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_init_error_on_object_is_null),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_insert_after_error_on_object_is_null),
            cmocka_unit_test(check_insert_after_error_on_node_is_null),
            cmocka_unit_test(check_insert_after),
            cmocka_unit_test(check_insert_before_error_on_object_is_null),
            cmocka_unit_test(check_insert_before_error_on_node_is_null),
            cmocka_unit_test(check_insert_before),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_next_error_on_node_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_node_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
