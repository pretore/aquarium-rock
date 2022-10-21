#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <time.h>
#include <seagrass.h>
#include <rock.h>
#include <stdio.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"
#include "test/red_black_tree.h"

static void check_node_init_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_init(NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct entry {
        int i;
        struct rock_red_black_tree_node node;
        int p;
    } e;
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_null(e.node.parent);
    assert_null(e.node.left);
    assert_null(e.node.right);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_parent_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_set_parent(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node parent;
    assert_true(rock_red_black_tree_node_init(&parent));
    node.parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(rock_red_black_tree_node_set_parent(&node, &parent));
    assert_ptr_equal((void *) ((uintmax_t) node.parent & ~1), &parent);
    assert_int_equal(((uintmax_t) node.parent & 1),
                     ROCK_RED_BLACK_TREE_COLOR_RED);
    node.parent = (void *) ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_set_parent(&node, &parent));
    assert_ptr_equal((void *) ((uintmax_t) node.parent & ~1), &parent);
    assert_int_equal(((uintmax_t) node.parent & 1),
                     ROCK_RED_BLACK_TREE_COLOR_BLACK);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_parent_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_parent(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_parent_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_parent((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node parent;
    assert_true(rock_red_black_tree_node_init(&parent));
    struct rock_red_black_tree_node *out;
    assert_true(rock_red_black_tree_node_get_parent(&node, &out));
    assert_null(out);
    assert_true(rock_red_black_tree_node_set_parent(&node, &parent));
    assert_true(rock_red_black_tree_node_get_parent(&node, &out));
    assert_int_equal(&parent, out);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_color_black_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(rock_red_black_tree_node_get_color(NULL, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_color_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_color((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_color(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    bool color;
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    node.parent = (void *) ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    node.parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_color_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_set_color(
            NULL, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_color(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    bool color;
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    assert_true(rock_red_black_tree_node_set_color(
            &node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    assert_true(rock_red_black_tree_node_set_color(
            &node, ROCK_RED_BLACK_TREE_COLOR_RED));
    assert_true(rock_red_black_tree_node_get_color(&node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_left_error_on_null_node(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_left(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_left_error_on_null_argument_ptr(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_left((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_left(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node *out;
    assert_true(rock_red_black_tree_node_get_left(&node, &out));
    assert_null(out);
    uintptr_t left = (rand() % UINTPTR_MAX);
    node.left = (struct rock_red_black_tree_node *) left;
    assert_true(rock_red_black_tree_node_get_left(&node, &out));
    assert_ptr_equal(out, (void *) left);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_left_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_set_left(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_left(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node left;
    struct rock_red_black_tree_node *out;
    assert_true(rock_red_black_tree_node_get_left(&node, &out));
    assert_null(out);
    assert_true(rock_red_black_tree_node_set_left(&node, &left));
    assert_true(rock_red_black_tree_node_get_left(&node, &out));
    assert_ptr_equal(&left, out);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_right_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_right(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_right_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_get_right((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_get_right(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node *out;
    assert_true(rock_red_black_tree_node_get_right(&node, &out));
    assert_null(out);
    uintptr_t right = (rand() % UINTPTR_MAX);
    node.right = (struct rock_red_black_tree_node *) right;
    assert_true(rock_red_black_tree_node_get_right(&node, &out));
    assert_ptr_equal(out, (void *) right);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_right_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_set_right(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_right(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node node;
    assert_true(rock_red_black_tree_node_init(&node));
    struct rock_red_black_tree_node right;
    struct rock_red_black_tree_node *out;
    assert_true(rock_red_black_tree_node_get_right(&node, &out));
    assert_null(out);
    assert_true(rock_red_black_tree_node_set_right(&node, &right));
    assert_true(rock_red_black_tree_node_get_right(&node, &out));
    assert_ptr_equal(&right, out);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_error_on_Y_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_rotate_left(NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_error_on_Y_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node Y;
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_false(rock_red_black_tree_rotate_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_error_on_Y_is_not_right_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_false(rock_red_black_tree_rotate_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *           p
     *          /
     *         X
     *        / \
     *       a   Y
     *          / \
     *         b   c
     */
    assert_true(rock_red_black_tree_node_set_left(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *           p
     *          /
     *         Y
     *        / \
     *       X   c
     *      / \
     *     a   b
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *       p
     *        \
     *         X
     *        / \
     *       a   Y
     *          / \
     *         b   c
     */
    assert_true(rock_red_black_tree_node_set_right(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *       p
     *        \
     *         Y
     *        / \
     *       X   c
     *      / \
     *     a   b
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *         X
     *        / \
     *       a   Y
     *          / \
     *         b   c
     */
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *         Y
     *        / \
     *       X   c
     *      / \
     *     a   b
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(&Y, &root_));
    assert_null(root_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *           p
     *          /
     *         X
     *        / \
     *       a   Y
     *            \
     *             c
     */
    assert_true(rock_red_black_tree_node_set_left(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *           p
     *          /
     *         Y
     *        / \
     *       X   c
     *      /
     *     a
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *       p
     *        \
     *         X
     *        / \
     *       a   Y
     *            \
     *             c
     */
    assert_true(rock_red_black_tree_node_set_right(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *       p
     *        \
     *         Y
     *        / \
     *       X   c
     *      /
     *     a
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *         X
     *        / \
     *       a   Y
     *            \
     *             c
     */
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_rotate_left(&Y));
    /*
     *         Y
     *        / \
     *       X   c
     *      /
     *     a
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(&Y, &root_));
    assert_null(root_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_error_on_Y_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_rotate_right(NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_error_on_Y_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node Y;
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_false(rock_red_black_tree_rotate_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_error_on_Y_is_not_left_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_false(rock_red_black_tree_rotate_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *           p
     *          /
     *         X
     *        / \
     *       Y   c
     *      / \
     *     a   b
     */
    assert_true(rock_red_black_tree_node_set_left(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *           p
     *          /
     *         Y
     *        / \
     *       a   X
     *          / \
     *         b   c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *       p
     *        \
     *         X
     *        / \
     *       Y   c
     *      / \
     *     a   b
     */
    assert_true(rock_red_black_tree_node_set_right(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *       p
     *        \
     *         Y
     *        / \
     *       a   X
     *          / \
     *         b   c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *         X
     *        / \
     *       Y   c
     *      / \
     *     a   b
     */
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *         Y
     *        / \
     *       a   X
     *          / \
     *         b   c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(&Y, &root_));
    assert_null(root_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *           p
     *          /
     *         X
     *        / \
     *       Y   c
     *      /
     *     a
     */
    assert_true(rock_red_black_tree_node_set_left(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *           p
     *          /
     *         Y
     *        / \
     *       a   X
     *            \
     *             c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, p, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *       p
     *        \
     *         X
     *        / \
     *       Y   c
     *      /
     *     a
     */
    assert_true(rock_red_black_tree_node_set_right(&p, &X));
    assert_true(rock_red_black_tree_node_set_parent(&X, &p));
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *       p
     *        \
     *         Y
     *        / \
     *       a   X
     *            \
     *             c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, c, X, Y;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    /*
     *         X
     *        / \
     *       Y   c
     *      /
     *     a
     */
    assert_true(rock_red_black_tree_node_set_right(&X, &c));
    assert_true(rock_red_black_tree_node_set_parent(&c, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&Y, &a));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Y));
    assert_true(rock_red_black_tree_rotate_right(&Y));
    /*
     *         Y
     *        / \
     *       a   X
     *            \
     *             c
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(&Y, &root_));
    assert_null(root_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&a, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&X, &b_));
    assert_null(b_);
    assert_true(rock_red_black_tree_node_get_right(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_error_on_Y_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_rotate_left_right(NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_error_on_Y_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node Y;
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_false(rock_red_black_tree_rotate_left_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_error_on_X_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_false(rock_red_black_tree_rotate_left_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_Y_is_not_right_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &X));
    assert_false(rock_red_black_tree_rotate_left_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_X_is_not_left_child_of_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &X));
    assert_false(rock_red_black_tree_rotate_left_right(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pL_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, d, p, X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&d));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    /*
     *           p
     *          /
     *         Z
     *        / \
     *       X   d
     *      / \
     *     a   Y
     *        / \
     *       c   d
     */
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &X));
    assert_true(rock_red_black_tree_node_set_parent(&Z, &p));
    assert_true(rock_red_black_tree_node_set_left(&p, &Z));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&d, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &d));
    assert_true(rock_red_black_tree_rotate_left_right(&Y));
    /*
     *          p
     *         /
     *        Y
     *       / \
     *      X   Z
     *     / \ / \
     *    a   bc  d
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_left(&Z, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&d, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_right(&Z, &d_));
    assert_ptr_equal(&d, d_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&Z, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pR_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, d, p, X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&d));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    /*
     *       p
     *        \
     *         Z
     *        / \
     *       X   d
     *      / \
     *     a   Y
     *        / \
     *       c   d
     */
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &X));
    assert_true(rock_red_black_tree_node_set_parent(&Z, &p));
    assert_true(rock_red_black_tree_node_set_right(&p, &Z));
    assert_true(rock_red_black_tree_node_set_parent(&a, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &a));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&d, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &d));
    assert_true(rock_red_black_tree_rotate_left_right(&Y));
    /*
     *      p
     *       \
     *        Y
     *       / \
     *      X   Z
     *     / \ / \
     *    a   bc  d
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(&a, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_left(&X, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_left(&Z, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&d, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_right(&Z, &d_));
    assert_ptr_equal(&d, d_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&Z, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_error_on_Y_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_rotate_right_left(NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_error_on_Y_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node Y;
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_false(rock_red_black_tree_rotate_right_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_error_on_X_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_false(rock_red_black_tree_rotate_right_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_Y_is_not_left_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &X));
    assert_false(rock_red_black_tree_rotate_right_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_X_is_not_right_child_of_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &X));
    assert_false(rock_red_black_tree_rotate_right_left(&Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pL_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, d, p, X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&d));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    /*
     *        p
     *       /
     *      Z
     *     / \
     *    a   X
     *       / \
     *      Y   d
     *     / \
     *    b   c
     */
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &X));
    assert_true(rock_red_black_tree_node_set_parent(&Z, &p));
    assert_true(rock_red_black_tree_node_set_left(&p, &Z));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &a));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&d, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &d));
    assert_true(rock_red_black_tree_rotate_right_left(&Y));
    /*
     *          p
     *         /
     *        Y
     *       / \
     *      Z   X
     *     / \ / \
     *    a   bc  d
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(&a, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_left(&Z, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_right(&Z, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_left(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&d, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &d_));
    assert_ptr_equal(&d, d_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&Z, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_left(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pR_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, d, p, X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&d));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));
    /*
     *    p
     *     \
     *      Z
     *     / \
     *    a   X
     *       / \
     *      Y   d
     *     / \
     *    a   b
     */
    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &X));
    assert_true(rock_red_black_tree_node_set_parent(&Z, &p));
    assert_true(rock_red_black_tree_node_set_right(&p, &Z));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &a));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&d, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &d));
    assert_true(rock_red_black_tree_rotate_right_left(&Y));
    /*
     *      p
     *       \
     *        Y
     *       / \
     *      Z   X
     *     / \ / \
     *    a   bc  d
     */
    struct rock_red_black_tree_node *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(&a, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_left(&Z, &a_));
    assert_ptr_equal(&a, a_);
    assert_true(rock_red_black_tree_node_get_parent(&b, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_right(&Z, &b_));
    assert_ptr_equal(&b, b_);
    assert_true(rock_red_black_tree_node_get_parent(&c, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_left(&X, &c_));
    assert_ptr_equal(&c, c_);
    assert_true(rock_red_black_tree_node_get_parent(&d, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_right(&X, &d_));
    assert_ptr_equal(&d, d_);
    assert_true(rock_red_black_tree_node_get_parent(&X, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_right(&Y, &X_));
    assert_ptr_equal(&X, X_);
    assert_true(rock_red_black_tree_node_get_parent(&Z, &Y_));
    assert_ptr_equal(&Y, Y_);
    assert_true(rock_red_black_tree_node_get_left(&Y, &Z_));
    assert_ptr_equal(&Z, Z_);
    assert_true(rock_red_black_tree_node_get_parent(&Y, &p_));
    assert_ptr_equal(&p, p_);
    assert_true(rock_red_black_tree_node_get_right(&p, &Y_));
    assert_ptr_equal(&Y, Y_);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_init(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_compare_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_init((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {
            .root = (void *) UINTPTR_MAX,
            .count = (-1),
            .compare = (void *) UINTPTR_MAX,
    };
    assert_true(rock_red_black_tree_init(
            &object,
            (int (*)(const struct rock_red_black_tree_node *,
                     const struct rock_red_black_tree_node *))
                    seagrass_void_ptr_compare));
    assert_null(object.root);
    assert_ptr_equal(seagrass_void_ptr_compare, object.compare);
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_invalidate(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void on_node_destroy(struct rock_red_black_tree_node *node) {
    function_called();
    assert_non_null(node);
}

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node a, b, c, d, p, X, Y, Z;
    assert_true(rock_red_black_tree_node_init(&a));
    assert_true(rock_red_black_tree_node_init(&b));
    assert_true(rock_red_black_tree_node_init(&c));
    assert_true(rock_red_black_tree_node_init(&d));
    assert_true(rock_red_black_tree_node_init(&p));
    assert_true(rock_red_black_tree_node_init(&X));
    assert_true(rock_red_black_tree_node_init(&Y));
    assert_true(rock_red_black_tree_node_init(&Z));

    assert_true(rock_red_black_tree_node_set_parent(&Y, &X));
    assert_true(rock_red_black_tree_node_set_left(&X, &Y));
    assert_true(rock_red_black_tree_node_set_parent(&X, &Z));
    assert_true(rock_red_black_tree_node_set_right(&Z, &X));
    assert_true(rock_red_black_tree_node_set_parent(&Z, &p));
    assert_true(rock_red_black_tree_node_set_right(&p, &Z));
    assert_true(rock_red_black_tree_node_set_parent(&a, &Z));
    assert_true(rock_red_black_tree_node_set_left(&Z, &a));
    assert_true(rock_red_black_tree_node_set_parent(&b, &Y));
    assert_true(rock_red_black_tree_node_set_left(&Y, &b));
    assert_true(rock_red_black_tree_node_set_parent(&c, &Y));
    assert_true(rock_red_black_tree_node_set_right(&Y, &c));
    assert_true(rock_red_black_tree_node_set_parent(&d, &X));
    assert_true(rock_red_black_tree_node_set_right(&X, &d));
    struct rock_red_black_tree object = {
            .root = &p
    };
    expect_function_calls(on_node_destroy, 8);
    assert_true(rock_red_black_tree_invalidate(&object,
                                               on_node_destroy));
    assert_null(object.root);
    assert_null(object.compare);
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_count(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_count((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {.count = 2732};
    assert_true(rock_red_black_tree_init(
            &object,
            (int (*)(const struct rock_red_black_tree_node *,
                     const struct rock_red_black_tree_node *))
                    seagrass_void_ptr_compare));
    assert_int_equal(0, object.count);
    object.count = (rand() % UINTMAX_MAX);
    uintmax_t count;
    assert_true(rock_red_black_tree_count(&object, &count));
    assert_int_equal(object.count, count);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_find(
            NULL, NULL, (void *) 1, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_error_on_needle_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_find(
            (void *) 1, NULL, NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NEEDLE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_find(
            (void *) 1, NULL, (void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

struct entry {
    uintmax_t value;
    struct rock_red_black_tree_node node;
};

static int compare_entry(const struct rock_red_black_tree_node *a,
                         const struct rock_red_black_tree_node *b) {
    struct entry *A = rock_container_of(a, struct entry, node);
    struct entry *B = rock_container_of(b, struct entry, node);
    return seagrass_uintmax_t_compare(A->value, B->value);
}

static void check_find_error_on_node_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct entry a, b, c, d, e, f, g;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    a.value = 0;
    b.value = 1;
    c.value = 2;
    d.value = 3;
    e.value = 4;
    f.value = 5;
    g.value = 6;
    /*
     *           d(3)
     *          /    \
     *       b(1)     f(5)
     *      /   \     /  \
     *    a(0)  c(2) e(4) g(6)
     */
    assert_true(rock_red_black_tree_node_set_parent(&b.node, &d.node));
    assert_true(rock_red_black_tree_node_set_left(&d.node, &b.node));
    assert_true(rock_red_black_tree_node_set_parent(&f.node, &d.node));
    assert_true(rock_red_black_tree_node_set_right(&d.node, &f.node));
    assert_true(rock_red_black_tree_node_set_parent(&a.node, &b.node));
    assert_true(rock_red_black_tree_node_set_left(&b.node, &a.node));
    assert_true(rock_red_black_tree_node_set_parent(&c.node, &b.node));
    assert_true(rock_red_black_tree_node_set_right(&b.node, &c.node));
    assert_true(rock_red_black_tree_node_set_parent(&e.node, &f.node));
    assert_true(rock_red_black_tree_node_set_left(&f.node, &e.node));
    assert_true(rock_red_black_tree_node_set_parent(&g.node, &f.node));
    assert_true(rock_red_black_tree_node_set_right(&f.node, &g.node));
    struct rock_red_black_tree tree = {
            .root = &d.node,
            .compare = compare_entry
    };
    struct rock_red_black_tree_node *out = NULL;
    struct entry needle = {.value = 10};
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_false(rock_red_black_tree_find(&tree, NULL, &needle.node, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND, rock_error);
    assert_non_null(out);
    assert_ptr_equal(&g.node, out); /* insertion point found in tree */
    assert_true(rock_red_black_tree_invalidate(&tree, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_empty_tree(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct entry needle = {.value = 12};
    assert_true(rock_red_black_tree_node_init(&needle.node));
    struct rock_red_black_tree tree = {};
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_find(&tree, NULL, &needle.node, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND, rock_error);
    assert_null(out);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct entry a, b, c, d, e, f, g;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    a.value = 0;
    b.value = 1;
    c.value = 2;
    d.value = 3;
    e.value = 4;
    f.value = 5;
    g.value = 6;
    /*
     *           d(3)
     *          /    \
     *       b(1)     f(5)
     *      /   \     /  \
     *    a(0)  c(2) e(4) g(6)
     */
    assert_true(rock_red_black_tree_node_set_parent(&b.node, &d.node));
    assert_true(rock_red_black_tree_node_set_left(&d.node, &b.node));
    assert_true(rock_red_black_tree_node_set_parent(&f.node, &d.node));
    assert_true(rock_red_black_tree_node_set_right(&d.node, &f.node));
    assert_true(rock_red_black_tree_node_set_parent(&a.node, &b.node));
    assert_true(rock_red_black_tree_node_set_left(&b.node, &a.node));
    assert_true(rock_red_black_tree_node_set_parent(&c.node, &b.node));
    assert_true(rock_red_black_tree_node_set_right(&b.node, &c.node));
    assert_true(rock_red_black_tree_node_set_parent(&e.node, &f.node));
    assert_true(rock_red_black_tree_node_set_left(&f.node, &e.node));
    assert_true(rock_red_black_tree_node_set_parent(&g.node, &f.node));
    assert_true(rock_red_black_tree_node_set_right(&f.node, &g.node));
    struct rock_red_black_tree tree = {
            .root = &d.node,
            .compare = compare_entry
    };
    struct rock_red_black_tree_node *out = NULL;
    struct entry needle = {.value = 2};
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_true(rock_red_black_tree_find(&tree, NULL, &needle.node, &out));
    assert_ptr_equal(&c.node, out); /* exact match found in tree */
    assert_true(rock_red_black_tree_invalidate(&tree, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_insert(NULL, (void *) 1, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_insertion_point_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {
            .root = (void *) 1
    };
    assert_false(rock_red_black_tree_insert(&object, NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {
            .root = (void *) 1
    };
    assert_false(rock_red_black_tree_insert(&object, (void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_node_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    object.root = &a.node;
    assert_true(rock_red_black_tree_node_set_color(
            object.root, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    a.value = b.value = (rand() % UINTMAX_MAX);
    assert_ptr_not_equal(&a.node, &b.node);
    assert_false(rock_red_black_tree_insert(&object, object.root, &b.node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS, rock_error);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_0(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a;
    assert_true(rock_red_black_tree_node_init(&a.node));
    a.value = (rand() % UINTMAX_MAX);
    bool color = ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_get_color(&a.node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_find(&object, NULL, &a.node, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND, rock_error);
    assert_null(out);
    assert_null(object.root);
    /* insert 'a' into an empty tree */
    assert_true(rock_red_black_tree_insert(&object, out, &a.node));
    assert_red_black_tree(&object);
    assert_red_black_tree_node(&a.node, NULL, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_1(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry root, a, b;
    assert_true(rock_red_black_tree_node_init(&root.node));
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    root.value = 199;
    a.value = 50;
    b.value = 263;
    assert_true(rock_red_black_tree_insert(&object, NULL, &root.node));
    assert_true(rock_red_black_tree_insert(&object, &root.node, &a.node));
    /*
     *      root(199B)
     *        /
     *     a(50R)
     */
    assert_ptr_equal(object.root, &root.node);
    assert_red_black_tree_node(&root.node, NULL, false, &a.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &root.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &root.node, &b.node));
    assert_red_black_tree(&object);
    /*
     *      root(199B)
     *        /    \
     *     a(50R)  b(263R)
     */
    assert_ptr_equal(object.root, &root.node);
    assert_red_black_tree_node(&root.node, NULL, false, &a.node, &b.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &root.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&b.node, &root.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_3(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    a.value = 4;
    b.value = 2;
    c.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    /*
     *         a(4B)
     *         /
     *     b(2R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    assert_red_black_tree(&object);
    /*
     *         b(2B)
     *         /  \
     *     c(1R)  a(4R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_2(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    a.value = 4;
    b.value = 2;
    c.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    /*
     *         a(4B)
     *         /
     *     c(1R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &c.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &c.node, &b.node));
    assert_red_black_tree(&object);
    /*
     *         b(2B)
     *         /  \
     *     c(1R)  a(4R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_1(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    a.value = 4;
    b.value = 2;
    c.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &a.node));
    /*
     *         c(1B)
     *             \
     *             a(4R)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, NULL, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_red_black_tree(&object);
    /*
     *         b(2B)
     *         /  \
     *     c(1R)  a(4R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_0(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    a.value = 4;
    b.value = 2;
    c.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &b.node));
    /*
     *         c(1B)
     *             \
     *             b(2R)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, NULL, &b.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &b.node, &a.node));
    assert_red_black_tree(&object);
    /*
     *         b(2B)
     *         /  \
     *     c(1R)  a(4R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2a(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    a.value = 5;
    b.value = 4;
    c.value = 2;
    d.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    /*
     *         b(4B)
     *         /  \
     *     c(2R)  a(5R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_red_black_tree(&object);
    /*
     *         b(4B)
     *         /  \
     *     c(2B)  a(5B)
     *     /
     *   d(1R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, &d.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 9;
    b.value = 5;
    c.value = 2;
    d.value = 4;
    e.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    /*
     *         b(5B)
     *         /  \
     *     c(2R)  a(9R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_insert(&object, &b.node, &d.node));
    assert_red_black_tree(&object);
    /*
     *         b(5B)
     *         /  \
     *     d(4B)  a(9B)
     *       /
     *   c(2R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &d.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, &c.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &d.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_insert(&object, &c.node, &e.node));
    assert_red_black_tree(&object);
    /*
     *         b(5B)
     *         /  \
     *     c(2B)  a(9B)
     *       / \
     *   e(1R) d(4R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &c.node, &a.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &b.node, true, &e.node, &d.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&d.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_first(NULL, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_first((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_tree_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree tree = {};
    struct rock_red_black_tree_node *node;
    assert_false(rock_red_black_tree_first(&tree, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 9;
    b.value = 5;
    c.value = 4;
    d.value = 2;
    e.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    struct rock_red_black_tree_node *first;
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(&a.node, first);
    uintmax_t value = rock_container_of(first, struct entry, node)->value;
    assert_int_equal(a.value, value);
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(&b.node, first);
    value = rock_container_of(first, struct entry, node)->value;
    assert_int_equal(b.value, value);
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(&c.node, first);
    value = rock_container_of(first, struct entry, node)->value;
    assert_int_equal(c.value, value);
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(&d.node, first);
    value = rock_container_of(first, struct entry, node)->value;
    assert_int_equal(d.value, value);
    assert_true(rock_red_black_tree_insert(&object, &d.node, &e.node));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(&e.node, first);
    value = rock_container_of(first, struct entry, node)->value;
    assert_int_equal(e.value, value);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_last(NULL, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_null_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree tree = {};
    assert_false(rock_red_black_tree_last(&tree, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_tree_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree tree = {};
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_last(&tree, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 1;
    b.value = 2;
    c.value = 4;
    d.value = 5;
    e.value = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    struct rock_red_black_tree_node *last;
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(&a.node, last);
    uintmax_t value = rock_container_of(last, struct entry, node)->value;
    assert_int_equal(a.value, value);
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(&b.node, last);
    value = rock_container_of(last, struct entry, node)->value;
    assert_int_equal(b.value, value);
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(&c.node, last);
    value = rock_container_of(last, struct entry, node)->value;
    assert_int_equal(c.value, value);
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(&d.node, last);
    value = rock_container_of(last, struct entry, node)->value;
    assert_int_equal(d.value, value);
    assert_true(rock_red_black_tree_insert(&object, &d.node, &e.node));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(&e.node, last);
    value = rock_container_of(last, struct entry, node)->value;
    assert_int_equal(e.value, value);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node *out;
    assert_false(rock_red_black_tree_next(NULL, &out));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_next((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 9;
    b.value = 5;
    c.value = 4;
    d.value = 2;
    e.value = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &d.node, &e.node));
    struct rock_red_black_tree_node *node;
    assert_true(rock_red_black_tree_first(&object, &node));
    assert_ptr_equal(&e.node, node);
    uintmax_t value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(1, value);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(&d.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(2, value);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(&c.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(4, value);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(&b.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(5, value);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(&a.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(9, value);
    assert_false(rock_red_black_tree_next(node, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree_node *node;
    assert_false(rock_red_black_tree_prev(NULL, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_prev((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 1;
    b.value = 2;
    c.value = 4;
    d.value = 5;
    e.value = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &d.node, &e.node));
    struct rock_red_black_tree_node *node;
    assert_true(rock_red_black_tree_last(&object, &node));
    assert_ptr_equal(&e.node, node);
    uintmax_t value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(9, value);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(&d.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(5, value);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(&c.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(4, value);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(&b.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(2, value);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(&a.node, node);
    value = rock_container_of(node, struct entry, node)->value;
    assert_int_equal(1, value);
    assert_false(rock_red_black_tree_prev(node, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_remove(NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_remove((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_leaf_nodes(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    a.value = 9;
    b.value = 4;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    /*
     *         a(9B)
     *         /
     *     b(4R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    /* remove leaf */
    assert_true(rock_red_black_tree_remove(&object, &b.node));
    /*
     *         a(9B)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    /* remove root */
    assert_true(rock_red_black_tree_remove(&object, &a.node));
    assert_null(object.root);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_with_single_child(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 1;
    b.value = 7;
    c.value = 4;
    d.value = 3;
    e.value = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &d.node, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, &d.node, &b.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &c.node, true, &a.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &c.node, false, NULL, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &d.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &d.node));
    assert_red_black_tree(&object);
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  b(7B)
     *               \
     *               e(9R)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, &a.node, &b.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &c.node, false, NULL, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_remove(&object, &b.node));
    assert_red_black_tree(&object);
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  e(9B)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, &a.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_with_two_children(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 1;
    b.value = 7;
    c.value = 4;
    d.value = 3;
    e.value = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &d.node, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_ptr_equal(object.root, &c.node);
    assert_red_black_tree_node(&c.node, NULL, false, &d.node, &b.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &c.node, true, &a.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &c.node, false, NULL, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &d.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_remove(&object, &c.node));
    /*
     *         b(7B)
     *         /  \
     *     d(3B)  e(9B)
     *      /
     *    a(1R)
     */
    assert_ptr_equal(object.root, &b.node);
    assert_red_black_tree_node(&b.node, NULL, false, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, &a.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &d.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_case_red_leaf(void **state) {
    // https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
    // Example 1: Delete 30 from RB tree in fig.3
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e, f, g, h;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    assert_true(rock_red_black_tree_node_init(&h.node));
    a.value = 10;
    b.value = 5;
    c.value = 30;
    d.value = 2;
    e.value = 9;
    f.value = 25;
    g.value = 40;
    h.value = 38;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &f.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &g.node));
    assert_true(rock_red_black_tree_insert(&object, &g.node, &h.node));
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(30R)
     *     /  \       /  \
     * d(2R) e(9R) f(25B) g(40B)
     *                      /
     *                  h(38R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, &f.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&f.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &c.node, false, &h.node, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&h.node, &g.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &c.node));
    assert_red_black_tree(&object);
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    h(38R)
     *     /  \       /  \
     * d(2R) e(9R) f(25B) g(40B)
     *
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &h.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&h.node, &a.node, false, &f.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&f.node, &h.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &h.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_case_4(void **state) {
    // https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
    // Example 2: Delete 15 from RB tree in fig. 6
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    a.value = 10;
    b.value = 5;
    c.value = 20;
    d.value = 15;
    e.value = 30;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &e.node));
    assert_true(rock_red_black_tree_node_set_color(
            &a.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &b.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &c.node, ROCK_RED_BLACK_TREE_COLOR_RED));
    assert_true(rock_red_black_tree_node_set_color(
            &d.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &e.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(20R)
     *               /  \
     *           d(15B) e(30B)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&d.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &d.node));
    assert_red_black_tree(&object);
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(20B)
     *                  \
     *                e(30R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, NULL, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_case_3_1(void **state) {
    // https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
    // Example 3: Delete '15' from fig. 9(A).
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e, f, g;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    a.value = 10;
    b.value = 5;
    c.value = 20;
    d.value = 1;
    e.value = 7;
    f.value = 15;
    g.value = 30;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &f.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &g.node));
    assert_true(rock_red_black_tree_node_set_color(
            &a.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &b.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &c.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &d.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &e.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &f.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &g.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(20B)
     *     /  \       /  \
     * d(1B) e(7B) f(15B) g(30B)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, &f.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&f.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &f.node));
    assert_red_black_tree(&object);
    /*
     *        a(10B)
     *         /   \
     *     b(5R)    c(20B)
     *     /  \          \
     * d(1B) e(7B)       g(30R)
     */
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&c.node, &a.node, false, NULL, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_case_2(void **state) {
    // https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
    // Example 4: Delete '15' from fig. 13
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e, f, g, h, i;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    assert_true(rock_red_black_tree_node_init(&h.node));
    assert_true(rock_red_black_tree_node_init(&i.node));
    a.value = 10;
    b.value = 5;
    c.value = 20;
    d.value = 1;
    e.value = 7;
    f.value = 15;
    g.value = 30;
    h.value = 25;
    i.value = 40;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &f.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &g.node));
    assert_true(rock_red_black_tree_insert(&object, &g.node, &h.node));
    assert_true(rock_red_black_tree_insert(&object, &g.node, &i.node));
    assert_true(rock_red_black_tree_node_set_color(
            &a.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &b.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &c.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &d.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &e.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &f.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &g.node, ROCK_RED_BLACK_TREE_COLOR_RED));
    assert_true(rock_red_black_tree_node_set_color(
            &h.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &i.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(20B)
     *     /  \       /  \
     * d(1B) e(7B) f(15B) g(30R)
     *                     /  \
     *                 h(25B) i(40B)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, &f.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&f.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &c.node, false, &h.node, &i.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&h.node, &g.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&i.node, &g.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &f.node));
    assert_red_black_tree(&object);
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    g(30B)
     *     /  \       /  \
     * d(1B) e(7B) c(20B) i(40B)
     *                 \
     *                 h(25R)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &a.node, false, &c.node, &i.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &g.node, true, NULL, &h.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&i.node, &g.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&h.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_case_3_5_6(void **state) {
    // https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
    // Example 5: Delete '1' from fig. 17
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct entry a, b, c, d, e, f, g, h, i;
    assert_true(rock_red_black_tree_node_init(&a.node));
    assert_true(rock_red_black_tree_node_init(&b.node));
    assert_true(rock_red_black_tree_node_init(&c.node));
    assert_true(rock_red_black_tree_node_init(&d.node));
    assert_true(rock_red_black_tree_node_init(&e.node));
    assert_true(rock_red_black_tree_node_init(&f.node));
    assert_true(rock_red_black_tree_node_init(&g.node));
    assert_true(rock_red_black_tree_node_init(&h.node));
    assert_true(rock_red_black_tree_node_init(&i.node));
    a.value = 10;
    b.value = 5;
    c.value = 30;
    d.value = 1;
    e.value = 7;
    f.value = 25;
    g.value = 20;
    h.value = 28;
    i.value = 40;
    assert_true(rock_red_black_tree_insert(&object, NULL, &a.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &b.node));
    assert_true(rock_red_black_tree_insert(&object, &a.node, &c.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &d.node));
    assert_true(rock_red_black_tree_insert(&object, &b.node, &e.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &f.node));
    assert_true(rock_red_black_tree_insert(&object, &c.node, &i.node));
    assert_true(rock_red_black_tree_insert(&object, &f.node, &g.node));
    assert_true(rock_red_black_tree_insert(&object, &f.node, &h.node));
    assert_true(rock_red_black_tree_node_set_color(
            &a.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &b.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &c.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &d.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &e.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &f.node, ROCK_RED_BLACK_TREE_COLOR_RED));
    assert_true(rock_red_black_tree_node_set_color(
            &g.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &h.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_set_color(
            &i.node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    /*
     *        a(10B)
     *         /   \
     *     b(5B)    c(30B)
     *     /  \       /  \
     * d(1B) e(7B) f(25R) i(40B)
     *              /  \
     *          g(20B) h(28B)
     */
    assert_ptr_equal(object.root, &a.node);
    assert_red_black_tree_node(&a.node, NULL, false, &b.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, &d.node, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &a.node, false, &f.node, &i.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&d.node, &b.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&f.node, &c.node, true, &g.node, &h.node,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_red_black_tree_node(&i.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &f.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&h.node, &f.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree(&object);
    assert_true(rock_red_black_tree_remove(&object, &d.node));
    assert_red_black_tree(&object);
    /*
     *        f(25B)
     *         /   \
     *    a(10B)    c(30B)
     *     /  \       /  \
     * b(5B) g(20B)h(28B) i(40B)
     *     \
     *    e(7R)
     */
    assert_ptr_equal(object.root, &f.node);
    assert_red_black_tree_node(&f.node, NULL, false, &a.node, &c.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&a.node, &f.node, true, &b.node, &g.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&c.node, &f.node, false, &h.node, &i.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&b.node, &a.node, true, NULL, &e.node,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&g.node, &a.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&h.node, &c.node, true, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&i.node, &c.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_BLACK);
    assert_red_black_tree_node(&e.node, &b.node, false, NULL, NULL,
                               ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_node_regression_1(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    const uintmax_t LIMIT = 32;
    struct entry values[LIMIT];
    struct rock_red_black_tree_node *p = NULL;
    for (uintmax_t i = 0; i < LIMIT; i++) {
        struct entry *e = &values[i];
        assert_true(rock_red_black_tree_node_init(&e->node));
        e->value = i;
        assert_true(rock_red_black_tree_insert(&object, p, &e->node));
        assert_red_black_tree(&object);
        p = &e->node;
    }
    /*
     *                   7B
     *                /       \
     *             3B           15R
     *           /  \          /     \
     *         1B   5B     11B          19B
     *        / \   / \   /   \         /   \
     *       0B 2B 4B 6B 9B   13B      17B    23R
     *                  / \   /  \    /  \    /   \
     *                8B 10B 12B 14B 16B 18B 21B    27B
     *                                      / \    /   \
     *                                    20B 22B 25B   29R
     *                                           / \    /  \
     *                                         24B 26B 28B 30B
     *                                                       \
     *                                                       31R
     */
    // remove 19 ...
    struct entry needle;
    needle.value = 19;
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_true(rock_red_black_tree_find(&object, NULL, &needle.node, &p));
    assert_true(rock_red_black_tree_remove(&object, p));
    assert_red_black_tree(&object);
    /*
     *                   7B
     *                /       \
     *             3B           15R
     *           /  \          /     \
     *         1B   5B     11B          20B
     *        / \   / \   /   \         /    \
     *       0B 2B 4B 6B 9B   13B      17B      27R
     *                  / \   /  \    /  \     /    \
     *                8B 10B 12B 14B 16B 18B 23B      29B
     *                                      /   \     /  \
     *                                    21B   25R  28B 30B
     *                                      \   /  \       \
     *                                     22R 24B 26B     31R
     */
    // remove 20 ...
    needle.value = 20;
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_true(rock_red_black_tree_find(&object, NULL, &needle.node, &p));
    assert_true(rock_red_black_tree_remove(&object, p));
    assert_red_black_tree(&object);
    /*
     *                   7B
     *                /       \
     *             3B           15R
     *           /  \          /     \
     *         1B   5B     11B          21B
     *        / \   / \   /   \         /   \
     *       0B 2B 4B 6B 9B   13B     17B      27R
     *                  / \   /  \    /  \    /    \
     *                8B 10B 12B 14B 16B 18B 23B     29B
     *                                       / \     / \
     *                                     22B 25R  28B 30B
     *                                         / \       \
     *                                       24B 26B     31R
     */
    // remove 18
    needle.value = 18;
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_true(rock_red_black_tree_find(&object, NULL, &needle.node, &p));
    assert_true(rock_red_black_tree_remove(&object, p));
    assert_red_black_tree(&object);
    /*
     *                   7B
     *                /       \
     *             3B           15R
     *           /  \          /      \
     *         1B   5B     11B           27B
     *        / \   / \   /   \          /     \
     *       0B 2B 4B 6B 9B   13B       23R       29B
     *                  / \   /  \     /   \      /  \
     *                8B 10B 12B 14B 21B    25B  28B 30B
     *                               /  \   /  \       \
     *                             17B 22B 24B 26B     31R
     *                             /
     *                           16R
     */
    // remove 9
    needle.value = 9;
    assert_true(rock_red_black_tree_node_init(&needle.node));
    assert_true(rock_red_black_tree_find(&object, NULL, &needle.node, &p));
    assert_true(rock_red_black_tree_remove(&object, p));
    assert_red_black_tree(&object);


    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_tree(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const uintmax_t LIMIT = 1000000;
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct rock_red_black_tree_node *p = NULL;
    for (uintmax_t i = 0; i < LIMIT; i++) {
        struct entry *n = calloc(1, sizeof(*n));
        assert_non_null(n);
        assert_true(rock_red_black_tree_node_init(&n->node));
        n->value = i;
        assert_int_equal(i, object.count);
        assert_true(rock_red_black_tree_insert(&object, p, &n->node));
        assert_int_equal(1 + i, object.count);
        p = &n->node;
    }
    assert_red_black_tree(&object);
    for (uintmax_t i = 0; i < LIMIT; i++) {
        assert_int_equal(LIMIT - i, object.count);
        p = object.root;
        assert_true(rock_red_black_tree_remove(&object, object.root));
        assert_int_equal((LIMIT - 1) - i, object.count);
        struct entry *n = rock_container_of(p, struct entry, node);
        free(n);
    }
    assert_null(object.root);
    assert_int_equal(0, object.count);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void fuzz_tree(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    const uintmax_t LIMIT = 10000;
    uintmax_t size;
    assert_true(seagrass_uintmax_t_multiply(LIMIT,
                                            sizeof(uintmax_t),
                                            &size));
    uintmax_t *const values = malloc(size);
    assert_non_null(values);
    for (uintmax_t i = 0; i < LIMIT; i++) {
        values[i] = i;
    }
    /* shuffle array */
    for (uintmax_t i = 0; i < LIMIT; i++) {
        const uintmax_t o = rand() % (LIMIT - i);
        if (!o) {
            continue;
        }
        const uintmax_t q = values[i + o];
        values[i + o] = values[i];
        values[i] = q;
    }
    /* insertion sequence */
    fprintf(stdout, "insert = {");
    struct rock_red_black_tree object;
    assert_true(rock_red_black_tree_init(&object, compare_entry));
    struct rock_red_black_tree_node *p = NULL;
    for (uintmax_t i = 0; i < LIMIT; i++) {
        struct entry *n = calloc(1, sizeof(*n));
        assert_non_null(n);
        assert_true(rock_red_black_tree_node_init(&n->node));
        n->value = values[i];
        fprintf(stdout, "%ju, ", n->value);
        fflush(stdout);
        assert_false(rock_red_black_tree_find(&object, NULL, &n->node, &p));
        assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND, rock_error);
        assert_true(rock_red_black_tree_insert(&object, p, &n->node));
        assert_red_black_tree(&object);
    }
    fprintf(stdout, "}\n");
    /* shuffle array */
    for (uintmax_t i = 0; i < LIMIT; i++) {
        const uintmax_t o = rand() % (LIMIT - i);
        if (!o) {
            continue;
        }
        const uintmax_t q = values[i + o];
        values[i + o] = values[i];
        values[i] = q;
    }
    /* removal sequence */
    fprintf(stdout, "remove = {");
    for (uintmax_t i = 0; object.count; i++) {
        struct entry needle = {
                .value = values[i]
        };
        assert_true(rock_red_black_tree_node_init(&needle.node));
        fprintf(stdout, "%ju, ", needle.value);
        fflush(stdout);
        assert_true(rock_red_black_tree_find(&object, NULL, &needle.node, &p));
        assert_true(rock_red_black_tree_remove(&object, p));
        free(rock_container_of(p, struct entry, node));
        assert_red_black_tree(&object);
    }
    fprintf(stdout, "}\n");
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    free(values);
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_init_error_on_node_is_null),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_node_set_parent_error_on_node_is_null),
            cmocka_unit_test(check_node_set_parent),
            cmocka_unit_test(check_node_get_parent_error_on_node_is_null),
            cmocka_unit_test(check_node_get_parent_error_on_out_is_null),
            cmocka_unit_test(check_node_get_parent),
            cmocka_unit_test(check_node_get_color_black_on_node_is_null),
            cmocka_unit_test(check_node_get_color_error_on_out_is_null),
            cmocka_unit_test(check_node_get_color),
            cmocka_unit_test(check_node_set_color_error_on_node_is_null),
            cmocka_unit_test(check_node_set_color),
            cmocka_unit_test(check_node_get_left_error_on_null_node),
            cmocka_unit_test(check_node_get_left_error_on_null_argument_ptr),
            cmocka_unit_test(check_node_get_left),
            cmocka_unit_test(check_node_set_left_error_on_node_is_null),
            cmocka_unit_test(check_node_set_left),
            cmocka_unit_test(check_node_get_right_error_on_node_is_null),
            cmocka_unit_test(check_node_get_right_error_on_out_is_null),
            cmocka_unit_test(check_node_get_right),
            cmocka_unit_test(check_node_set_right_error_on_node_is_null),
            cmocka_unit_test(check_node_set_right),
            cmocka_unit_test(check_rotate_left_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_left_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_left_error_on_Y_is_not_right_child_of_X),
            cmocka_unit_test(check_rotate_left_a_b_c_pL_X_Y),
            cmocka_unit_test(check_rotate_left_a_b_c_pR_X_Y),
            cmocka_unit_test(check_rotate_left_a_b_c_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_pL_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_pR_X_Y),
            cmocka_unit_test(check_rotate_left_a_c_X_Y),
            cmocka_unit_test(check_rotate_right_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_right_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_right_error_on_Y_is_not_left_child_of_X),
            cmocka_unit_test(check_rotate_right_a_b_c_pL_X_Y),
            cmocka_unit_test(check_rotate_right_a_b_c_pR_X_Y),
            cmocka_unit_test(check_rotate_right_a_b_c_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_pL_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_pR_X_Y),
            cmocka_unit_test(check_rotate_right_a_c_X_Y),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_left_right_error_on_X_has_no_parent),
            cmocka_unit_test(check_rotate_left_right_error_on_Y_is_not_right_child_of_X),
            cmocka_unit_test(check_rotate_left_right_error_on_X_is_not_left_child_of_Z),
            cmocka_unit_test(check_rotate_left_right_a_b_c_d_pL_X_Y_Z),
            cmocka_unit_test(check_rotate_left_right_a_b_c_d_pR_X_Y_Z),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_is_null),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_has_no_parent),
            cmocka_unit_test(check_rotate_right_left_error_on_X_has_no_parent),
            cmocka_unit_test(check_rotate_right_left_error_on_Y_is_not_left_child_of_X),
            cmocka_unit_test(check_rotate_right_left_error_on_X_is_not_right_child_of_Z),
            cmocka_unit_test(check_rotate_right_left_a_b_c_d_pL_X_Y_Z),
            cmocka_unit_test(check_rotate_right_left_a_b_c_d_pR_X_Y_Z),
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_compare_is_null),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_invalidate_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_find_error_on_object_is_null),
            cmocka_unit_test(check_find_error_on_needle_is_null),
            cmocka_unit_test(check_find_error_on_out_is_null),
            cmocka_unit_test(check_find_error_on_node_not_found),
            cmocka_unit_test(check_find_empty_tree),
            cmocka_unit_test(check_find),
            cmocka_unit_test(check_insert_error_on_object_is_null),
            cmocka_unit_test(check_insert_error_on_insertion_point_is_null),
            cmocka_unit_test(check_insert_error_on_node_is_null),
            cmocka_unit_test(check_insert_error_on_node_already_exists),
            cmocka_unit_test(check_insert_case_0),
            cmocka_unit_test(check_insert_case_1),
            cmocka_unit_test(check_insert_case_2b_configuration_3),
            cmocka_unit_test(check_insert_case_2b_configuration_2),
            cmocka_unit_test(check_insert_case_2b_configuration_1),
            cmocka_unit_test(check_insert_case_2b_configuration_0),
            cmocka_unit_test(check_insert_case_2a),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_tree_is_empty),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_null_out_is_null),
            cmocka_unit_test(check_last_error_on_tree_is_empty),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_node_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_node_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_node_is_null),
            cmocka_unit_test(check_remove_leaf_nodes),
            cmocka_unit_test(check_remove_node_with_single_child),
            cmocka_unit_test(check_remove_node_with_two_children),
            cmocka_unit_test(check_remove_node_case_red_leaf),
            cmocka_unit_test(check_remove_node_case_4),
            cmocka_unit_test(check_remove_node_case_3_1),
            cmocka_unit_test(check_remove_node_case_2),
            cmocka_unit_test(check_remove_node_case_3_5_6),
            cmocka_unit_test(check_remove_node_regression_1),
            cmocka_unit_test(check_tree),
            cmocka_unit_test(fuzz_tree),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
