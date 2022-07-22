#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <seagrass.h>
#include <rock.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"

static void check_node_alloc_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_alloc(0, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_alloc_error_on_memory_allocation_failed(void **s) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_alloc(SIZE_MAX, (void **) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_alloc(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct object {
        size_t count;
    } *object;
    assert_true(rock_red_black_tree_node_alloc(
            sizeof(*object),
            (void **) &object));
    rock_red_black_tree_node_destroy(object);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_init_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_init(NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_true(rock_red_black_tree_node_alloc(0, &node));
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->right = (void *) ~0;
    node_->left = (void *) ~0;
    node_->parent = (void *) ~0;
    assert_true(rock_red_black_tree_node_init(node));
    assert_null(node_->parent);
    assert_null(node_->left);
    assert_null(node_->right);
    rock_red_black_tree_node_destroy(node);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node(0, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node(SIZE_MAX, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    assert_null(node_->parent);
    assert_null(node_->left);
    assert_null(node_->right);
    rock_red_black_tree_node_destroy(node);
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
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(rock_red_black_tree_node_set_parent(node, &rock_error));
    assert_ptr_equal((void *) ((size_t) node_->parent & ~1), &rock_error);
    assert_int_equal(((size_t) node_->parent & 1),
                     ROCK_RED_BLACK_TREE_COLOR_RED);
    node_->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_set_parent(node, &rock_error));
    assert_ptr_equal((void *) ((size_t) node_->parent & ~1), &rock_error);
    assert_int_equal(((size_t) node_->parent & 1),
                     ROCK_RED_BLACK_TREE_COLOR_BLACK);
    rock_red_black_tree_node_destroy(node);
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
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    void *parent;
    assert_true(rock_red_black_tree_node_get_parent(node, &parent));
    assert_null(parent);
    assert_true(rock_red_black_tree_node_set_parent(node, &rock_error));
    assert_true(rock_red_black_tree_node_get_parent(node, &parent));
    assert_int_equal(&rock_error, parent);
    rock_red_black_tree_node_destroy(node);
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
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    bool color;
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    node_->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    rock_red_black_tree_node_destroy(node);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_color_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_node_set_color(
            NULL,
            ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_node_set_color(void **state) {
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    bool color;
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    assert_true(rock_red_black_tree_node_set_color(
            node, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    assert_true(rock_red_black_tree_node_set_color(
            node, ROCK_RED_BLACK_TREE_COLOR_RED));
    assert_true(rock_red_black_tree_node_get_color(node, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    rock_red_black_tree_node_destroy(node);
}

static void check_node_get_left_error_on_null_node(void**state) {
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
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    void *left = &check_node_get_left;
    assert_true(rock_red_black_tree_node_get_left(node, &left));
    assert_null(left);
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->left = &rock_error;
    assert_true(rock_red_black_tree_node_get_left(node, &left));
    assert_ptr_equal(&rock_error, left);
    rock_red_black_tree_node_destroy(node);
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
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    void *left;
    assert_true(rock_red_black_tree_node_get_left(node, &left));
    assert_null(left);
    assert_true(rock_red_black_tree_node_set_left(node, &rock_error));
    assert_true(rock_red_black_tree_node_get_left(node, &left));
    assert_ptr_equal(&rock_error, left);
    rock_red_black_tree_node_destroy(node);
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
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    void *right = &check_node_get_right;
    assert_true(rock_red_black_tree_node_get_right(node, &right));
    assert_null(right);
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->right = &rock_error;
    assert_true(rock_red_black_tree_node_get_right(node, &right));
    assert_ptr_equal(&rock_error, right);
    rock_red_black_tree_node_destroy(node);
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
    void *node;
    assert_true(rock_red_black_tree_node(0, &node));
    void *right;
    assert_true(rock_red_black_tree_node_get_right(node, &right));
    assert_null(right);
    assert_true(rock_red_black_tree_node_set_right(node, &rock_error));
    assert_true(rock_red_black_tree_node_get_right(node, &right));
    assert_ptr_equal(&rock_error, right);
    rock_red_black_tree_node_destroy(node);
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
    void *Y;
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_false(rock_red_black_tree_rotate_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_error_on_Y_is_not_right_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_false(rock_red_black_tree_rotate_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_b_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_a_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(X, a)
                && rock_red_black_tree_node_set_parent(a, X));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, c)
                && rock_red_black_tree_node_set_parent(c, Y));
    assert_true(rock_red_black_tree_rotate_left(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(Y, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &Y_)
                && Y == Y_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
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
    void *Y;
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_false(rock_red_black_tree_rotate_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_error_on_Y_is_not_left_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_false(rock_red_black_tree_rotate_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_b_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_right(Y, b)
                && rock_red_black_tree_node_set_parent(b, Y));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_pL_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_left(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_left(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_pR_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *p, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(p, X)
                && rock_red_black_tree_node_set_parent(X, p));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *p_, *X_, *Y_;
    assert_true(rock_red_black_tree_node_get_right(p, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_a_c_X_Y(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *c, *X, *Y;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_right(X, c)
                && rock_red_black_tree_node_set_parent(c, X));
    assert_true(rock_red_black_tree_node_set_left(X, Y)
                && rock_red_black_tree_node_set_parent(Y, X));
    assert_true(rock_red_black_tree_node_set_left(Y, a)
                && rock_red_black_tree_node_set_parent(a, Y));
    assert_true(rock_red_black_tree_rotate_right(Y));
    void *a_, *b_, *c_, *X_, *Y_, *root_;
    assert_true(rock_red_black_tree_node_get_parent(Y, &root_)
                && NULL == root_);
    assert_true(rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_
                && rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(Y, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &Y_)
                && Y == Y_);
    assert_true(rock_red_black_tree_node_get_left(X, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node_get_right(X, &c_)
                && c == c_
                && rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(a);
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
    void *Y;
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_false(rock_red_black_tree_rotate_left_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_error_on_X_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_right(X, Y));
    assert_false(rock_red_black_tree_rotate_left_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_Y_is_not_right_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_left(Z, X));
    assert_false(rock_red_black_tree_rotate_left_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X,
                     rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_left_right_error_on_X_is_not_left_child_of_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_right(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_right(Z, X));
    assert_false(rock_red_black_tree_rotate_left_right(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z,
                     rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pL_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &d));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_right(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_left(Z, X));
    assert_true(rock_red_black_tree_node_set_parent(Z, p)
                && rock_red_black_tree_node_set_left(p, Z));
    assert_true(rock_red_black_tree_node_set_parent(a, X)
                && rock_red_black_tree_node_set_left(X, a));
    assert_true(rock_red_black_tree_node_set_parent(b, Y)
                && rock_red_black_tree_node_set_left(Y, b));
    assert_true(rock_red_black_tree_node_set_parent(c, Y)
                && rock_red_black_tree_node_set_right(Y, c));
    assert_true(rock_red_black_tree_node_set_parent(d, Z)
                && rock_red_black_tree_node_set_right(Z, d));
    assert_true(rock_red_black_tree_rotate_left_right(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_
                && rock_red_black_tree_node_get_left(X, &a_)
                && a == a_);
    assert_true(rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_
                && rock_red_black_tree_node_get_right(X, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_node_get_parent(c, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_left(Z, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_node_get_parent(d, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_right(Z, &d_)
                && d == d_);
    assert_true(rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_parent(Z, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_right(Y, &Z_)
                && Z == Z_);
    assert_true(rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_
                && rock_red_black_tree_node_get_left(p, &Y)
                && Y == Y_);
    rock_red_black_tree_node_destroy(a);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(d);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_left_right_a_b_c_d_pR_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &d));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_right(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_left(Z, X));
    assert_true(rock_red_black_tree_node_set_parent(Z, p)
                && rock_red_black_tree_node_set_right(p, Z));
    assert_true(rock_red_black_tree_node_set_parent(a, X)
                && rock_red_black_tree_node_set_left(X, a));
    assert_true(rock_red_black_tree_node_set_parent(b, Y)
                && rock_red_black_tree_node_set_left(Y, b));
    assert_true(rock_red_black_tree_node_set_parent(c, Y)
                && rock_red_black_tree_node_set_right(Y, c));
    assert_true(rock_red_black_tree_node_set_parent(d, Z)
                && rock_red_black_tree_node_set_right(Z, d));
    assert_true(rock_red_black_tree_rotate_left_right(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(a, &X_)
                && X == X_
                && rock_red_black_tree_node_get_left(X, &a_)
                && a == a_);
    assert_true(rock_red_black_tree_node_get_parent(b, &X_)
                && X == X_
                && rock_red_black_tree_node_get_right(X, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_node_get_parent(c, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_left(Z, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_node_get_parent(d, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_right(Z, &d_)
                && d == d_);
    assert_true(rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_left(Y, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_parent(Z, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_right(Y, &Z_)
                && Z == Z_);
    assert_true(rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_
                && rock_red_black_tree_node_get_right(p, &Y)
                && Y == Y_);
    rock_red_black_tree_node_destroy(a);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(d);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
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
    void *Y;
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_false(rock_red_black_tree_rotate_right_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_error_on_X_has_no_parent(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_false(rock_red_black_tree_rotate_right_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT, rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_Y_is_not_left_child_of_X(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_right(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_left(Z, X));
    assert_false(rock_red_black_tree_rotate_right_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X,
                     rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_rotate_right_left_error_on_X_is_not_right_child_of_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_left(Z, X));
    assert_false(rock_red_black_tree_rotate_right_left(Y));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z,
                     rock_error);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pL_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &d));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_right(Z, X));
    assert_true(rock_red_black_tree_node_set_parent(Z, p)
                && rock_red_black_tree_node_set_left(p, Z));
    assert_true(rock_red_black_tree_node_set_parent(a, Z)
                && rock_red_black_tree_node_set_left(Z, a));
    assert_true(rock_red_black_tree_node_set_parent(b, Y)
                && rock_red_black_tree_node_set_left(Y, b));
    assert_true(rock_red_black_tree_node_set_parent(c, Y)
                && rock_red_black_tree_node_set_right(Y, c));
    assert_true(rock_red_black_tree_node_set_parent(d, X)
                && rock_red_black_tree_node_set_right(X, d));
    assert_true(rock_red_black_tree_rotate_right_left(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(a, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_left(Z, &a_)
                && a == a_);
    assert_true(rock_red_black_tree_node_get_parent(b, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_right(Z, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_
                && rock_red_black_tree_node_get_left(X, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_node_get_parent(d, &X_)
                && X == X_
                && rock_red_black_tree_node_get_right(X, &d_)
                && d == d_);
    assert_true(rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_parent(Z, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_left(Y, &Z_)
                && Z == Z_);
    assert_true(rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_
                && rock_red_black_tree_node_get_left(p, &Y)
                && Y == Y_);
    rock_red_black_tree_node_destroy(a);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(d);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rotate_right_left_a_b_c_d_pR_X_Y_Z(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &d));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_right(Z, X));
    assert_true(rock_red_black_tree_node_set_parent(Z, p)
                && rock_red_black_tree_node_set_right(p, Z));
    assert_true(rock_red_black_tree_node_set_parent(a, Z)
                && rock_red_black_tree_node_set_left(Z, a));
    assert_true(rock_red_black_tree_node_set_parent(b, Y)
                && rock_red_black_tree_node_set_left(Y, b));
    assert_true(rock_red_black_tree_node_set_parent(c, Y)
                && rock_red_black_tree_node_set_right(Y, c));
    assert_true(rock_red_black_tree_node_set_parent(d, X)
                && rock_red_black_tree_node_set_right(X, d));
    assert_true(rock_red_black_tree_rotate_right_left(Y));
    void *a_, *b_, *c_, *d_, *p_, *X_, *Y_, *Z_;
    assert_true(rock_red_black_tree_node_get_parent(a, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_left(Z, &a_)
                && a == a_);
    assert_true(rock_red_black_tree_node_get_parent(b, &Z_)
                && Z == Z_
                && rock_red_black_tree_node_get_right(Z, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_node_get_parent(c, &X_)
                && X == X_
                && rock_red_black_tree_node_get_left(X, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_node_get_parent(d, &X_)
                && X == X_
                && rock_red_black_tree_node_get_right(X, &d_)
                && d == d_);
    assert_true(rock_red_black_tree_node_get_parent(X, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_right(Y, &X_)
                && X == X_);
    assert_true(rock_red_black_tree_node_get_parent(Z, &Y_)
                && Y == Y_
                && rock_red_black_tree_node_get_left(Y, &Z_)
                && Z == Z_);
    assert_true(rock_red_black_tree_node_get_parent(Y, &p_)
                && p == p_
                && rock_red_black_tree_node_get_right(p, &Y)
                && Y == Y_);
    rock_red_black_tree_node_destroy(a);
    rock_red_black_tree_node_destroy(b);
    rock_red_black_tree_node_destroy(c);
    rock_red_black_tree_node_destroy(d);
    rock_red_black_tree_node_destroy(p);
    rock_red_black_tree_node_destroy(X);
    rock_red_black_tree_node_destroy(Y);
    rock_red_black_tree_node_destroy(Z);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    int (*compare)(const void *, const void *) = (void *) 1;
    assert_false(rock_red_black_tree_init(NULL,
                                           compare));
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
            .root = (void *)SIZE_MAX,
            .count = (-1),
            .compare = (void *)SIZE_MAX,
    };
    assert_true(rock_red_black_tree_init(&object,
                                         seagrass_void_ptr_compare));
    assert_null(object.root);
    assert_ptr_equal(seagrass_void_ptr_compare, object.compare);
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void (*on_node_destroy)(void *) = (void *) 1;
    assert_false(rock_red_black_tree_invalidate(NULL, on_node_destroy));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void on_node_destroy(void *node) {
    function_called();
    assert_non_null(node);
}

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *a, *b, *c, *d, *p, *X, *Y, *Z;
    assert_true(rock_red_black_tree_node(0, &a));
    assert_true(rock_red_black_tree_node(0, &b));
    assert_true(rock_red_black_tree_node(0, &c));
    assert_true(rock_red_black_tree_node(0, &d));
    assert_true(rock_red_black_tree_node(0, &p));
    assert_true(rock_red_black_tree_node(0, &X));
    assert_true(rock_red_black_tree_node(0, &Y));
    assert_true(rock_red_black_tree_node(0, &Z));
    assert_true(rock_red_black_tree_node_set_parent(Y, X)
                && rock_red_black_tree_node_set_left(X, Y));
    assert_true(rock_red_black_tree_node_set_parent(X, Z)
                && rock_red_black_tree_node_set_right(Z, X));
    assert_true(rock_red_black_tree_node_set_parent(Z, p)
                && rock_red_black_tree_node_set_right(p, Z));
    assert_true(rock_red_black_tree_node_set_parent(a, Z)
                && rock_red_black_tree_node_set_left(Z, a));
    assert_true(rock_red_black_tree_node_set_parent(b, Y)
                && rock_red_black_tree_node_set_left(Y, b));
    assert_true(rock_red_black_tree_node_set_parent(c, Y)
                && rock_red_black_tree_node_set_right(Y, c));
    assert_true(rock_red_black_tree_node_set_parent(d, X)
                && rock_red_black_tree_node_set_right(X, d));
    struct rock_red_black_tree object = {
            .root = p
    };
    expect_function_calls(on_node_destroy, 8);
    assert_true(rock_red_black_tree_invalidate(&object, on_node_destroy));
    assert_null(object.root);
    assert_null(object.compare);
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_count(NULL, (void *)1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_count((void *)1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {.count = 2732};
    assert_true(rock_red_black_tree_init(&object, seagrass_void_ptr_compare));
    assert_int_equal(0, object.count);
    object.count = 62213;
    size_t count;
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

static void check_find_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_find(
            (void *) 1, NULL, NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_VALUE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_find(
            (void *) 1, NULL, (void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_error_on_value_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const size_t size = sizeof(size_t);
    size_t *a, *b, *c, *d, *e, *f, *g;
    assert_true(rock_red_black_tree_node(size, (void **) &a));
    assert_true(rock_red_black_tree_node(size, (void **) &b));
    assert_true(rock_red_black_tree_node(size, (void **) &c));
    assert_true(rock_red_black_tree_node(size, (void **) &d));
    assert_true(rock_red_black_tree_node(size, (void **) &e));
    assert_true(rock_red_black_tree_node(size, (void **) &f));
    assert_true(rock_red_black_tree_node(size, (void **) &g));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;
    *e = 4;
    *f = 5;
    *g = 6;
    seagrass_required_true(rock_red_black_tree_node_set_parent(b, d)
                        && rock_red_black_tree_node_set_left(d, b));
    seagrass_required_true(rock_red_black_tree_node_set_parent(f, d)
                        && rock_red_black_tree_node_set_right(d, f));
    seagrass_required_true(rock_red_black_tree_node_set_parent(a, b)
                        && rock_red_black_tree_node_set_left(b, a));
    seagrass_required_true(rock_red_black_tree_node_set_parent(c, b)
                        && rock_red_black_tree_node_set_right(b, c));
    seagrass_required_true(rock_red_black_tree_node_set_parent(e, f)
                        && rock_red_black_tree_node_set_left(f, e));
    seagrass_required_true(rock_red_black_tree_node_set_parent(g, f)
                        && rock_red_black_tree_node_set_right(f, g));
    struct rock_red_black_tree tree = {
            .root = d,
            .compare = (int (*)(const void *, const void *))seagrass_size_t_ptr_compare
    };
    size_t *item = NULL;
    size_t key = 10;
    assert_false(rock_red_black_tree_find(
            &tree, NULL, &key, (void **) &item));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND, rock_error);
    assert_ptr_equal(g, item); /* insertion point found in tree */
    assert_true(rock_red_black_tree_invalidate(&tree, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_find_empty_tree(void **state) {
    rock_error = ROCK_ERROR_NONE;
    size_t value = 12;
    struct rock_red_black_tree tree = {};
    void *at = &value;
    assert_false(rock_red_black_tree_find(
            &tree, NULL, &value, &at));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND, rock_error);
    assert_null(at);
    rock_error = ROCK_ERROR_NONE;
}

static void check_find(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const size_t size = sizeof(size_t);
    size_t *a, *b, *c, *d, *e, *f, *g;
    assert_true(rock_red_black_tree_node(size, (void **) &a));
    assert_true(rock_red_black_tree_node(size, (void **) &b));
    assert_true(rock_red_black_tree_node(size, (void **) &c));
    assert_true(rock_red_black_tree_node(size, (void **) &d));
    assert_true(rock_red_black_tree_node(size, (void **) &e));
    assert_true(rock_red_black_tree_node(size, (void **) &f));
    assert_true(rock_red_black_tree_node(size, (void **) &g));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;
    *e = 4;
    *f = 5;
    *g = 6;
    assert_true(rock_red_black_tree_node_set_parent(b, d)
                        && rock_red_black_tree_node_set_left(d, b));
    assert_true(rock_red_black_tree_node_set_parent(f, d)
                        && rock_red_black_tree_node_set_right(d, f));
    assert_true(rock_red_black_tree_node_set_parent(a, b)
                        && rock_red_black_tree_node_set_left(b, a));
    assert_true(rock_red_black_tree_node_set_parent(c, b)
                        && rock_red_black_tree_node_set_right(b, c));
    assert_true(rock_red_black_tree_node_set_parent(e, f)
                        && rock_red_black_tree_node_set_left(f, e));
    assert_true(rock_red_black_tree_node_set_parent(g, f)
                        && rock_red_black_tree_node_set_right(f, g));
    struct rock_red_black_tree tree = {
            .root = d,
            .compare = (int (*)(const void *, const void *))seagrass_size_t_ptr_compare
    };
    size_t *item = NULL;
    size_t key = 2;
    assert_true(rock_red_black_tree_find(
            &tree, NULL, &key, (void **) &item));
    assert_ptr_equal(c, item); /* exact match found in tree */
    assert_true(rock_red_black_tree_invalidate(&tree, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_insert(
            NULL, (void *) 1, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_insertion_point_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {
            .root = (void *) 1
    };
    assert_false(rock_red_black_tree_insert(
            &object, NULL, (void *) 1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {
            .root = (void *) 1
    };
    assert_false(rock_red_black_tree_insert(
            &object, (void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_node_already_exists(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *value;
    assert_true(rock_red_black_tree_node(
            sizeof(*value), (void **) &value));
    *value = 99;
    object.root = value;
    assert_true(rock_red_black_tree_node_set_color(
            value, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    size_t *node;
    assert_true(rock_red_black_tree_node(
            sizeof(*node), (void **) &node));
    *node = 99;
    assert_ptr_not_equal(value, node);
    assert_false(rock_red_black_tree_insert(&object, value, node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS, rock_error);
    rock_red_black_tree_node_destroy(node);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_0(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    *a = 45;
    bool color = ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(rock_red_black_tree_node_get_color(a, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_RED, color);
    void *at;
    assert_false(rock_red_black_tree_find(&object, NULL, a, &at));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND, rock_error);
    assert_null(at);
    assert_null(object.root);
    assert_true(rock_red_black_tree_insert(&object, at, a));
    assert_true(rock_red_black_tree_node_get_color(a, &color));
    assert_int_equal(ROCK_RED_BLACK_TREE_COLOR_BLACK, color);
    assert_ptr_equal(a, object.root);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_1(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *root, *a, *b;
    assert_true(rock_red_black_tree_node(sizeof(*root), (void **) &root));
    *root = 199;
    object.root = root;
    assert_true(rock_red_black_tree_node_set_color(
            root, ROCK_RED_BLACK_TREE_COLOR_BLACK));
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    *a = 50;
    assert_true(rock_red_black_tree_insert(&object, root, a));
    void *root_, *a_, *b_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == root
                && rock_red_black_tree_node_get_color(root, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(root, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_right(root, &b_)
                && NULL == b_);
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    *b = 263;
    assert_true(rock_red_black_tree_insert(&object, root, b));
    color = ROCK_RED_BLACK_TREE_COLOR_BLACK;
    assert_true(object.root == root
                && rock_red_black_tree_node_get_left(root, &a_)
                && a == a_
                && rock_red_black_tree_node_get_parent(a, &root_)
                && root == root_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_right(root, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &root_)
                && root == root_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_3(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, b));
    void *a_, *b_, *c_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == a
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(a, &b_)
                && b == b_
                && rock_red_black_tree_node_get_color(b, &color)
                && color == ROCK_RED_BLACK_TREE_COLOR_RED);
    assert_true(rock_red_black_tree_insert(&object, b, c));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_2(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, c));
    void *a_, *b_, *c_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == a
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(a, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_insert(&object, c, b));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_1(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, c));
    assert_true(rock_red_black_tree_insert(&object, c, a));
    void *a_, *b_, *c_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(c, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2b_configuration_0(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    *a = 4;
    *b = 2;
    *c = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, c));
    assert_true(rock_red_black_tree_insert(&object, c, b));
    void *a_, *b_, *c_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_insert(&object, b, a));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_case_2a(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    *a = 5;
    *b = 4;
    *c = 2;
    *d = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, b));
    assert_true(rock_red_black_tree_insert(&object, b, a));
    assert_true(rock_red_black_tree_insert(&object, b, c));
    void *a_, *b_, *c_, *d_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(c, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 2;
    *d = 4;
    *e = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(rock_red_black_tree_insert(&object, b, c));
    void *a_, *b_, *c_, *d_, *e_;
    bool color = ROCK_RED_BLACK_TREE_COLOR_RED;
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_insert(&object, b, d));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(d, &b_)
                && b == b_
                && rock_red_black_tree_node_get_left(d, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(c, &d_)
                && d == d_
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_insert(&object, c, e));
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(a, &b_)
                && b == b_
                && rock_red_black_tree_node_get_left(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(a, &c_)
                && b == b_
                && rock_red_black_tree_node_get_right(c, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(d, &c_)
                && c == c_
                && rock_red_black_tree_node_get_left(c, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(e, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_false(rock_red_black_tree_first(NULL, &node));
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
    void *node;
    assert_false(rock_red_black_tree_first(&tree, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 4;
    *d = 2;
    *e = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    void *first;
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(a, first);
    assert_int_equal(*a, *(size_t *) first);
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(b, first);
    assert_int_equal(*b, *(size_t *) first);
    assert_true(rock_red_black_tree_insert(&object, b, c));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(c, first);
    assert_int_equal(*c, *(size_t *) first);
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(d, first);
    assert_int_equal(*d, *(size_t *) first);
    assert_true(rock_red_black_tree_insert(&object, d, e));
    assert_true(rock_red_black_tree_first(&object, &first));
    assert_ptr_equal(e, first);
    assert_int_equal(*e, *(size_t *) first);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_false(rock_red_black_tree_last(NULL, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_null_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree tree = {};
    void *node;
    assert_false(rock_red_black_tree_last(&tree, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_tree_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree tree = {};
    void *node;
    assert_false(rock_red_black_tree_last(&tree, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 2;
    *c = 4;
    *d = 5;
    *e = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    void *last;
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(a, last);
    assert_int_equal(*a, *(size_t *) last);
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(b, last);
    assert_int_equal(*b, *(size_t *) last);
    assert_true(rock_red_black_tree_insert(&object, b, c));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(c, last);
    assert_int_equal(*c, *(size_t *) last);
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(d, last);
    assert_int_equal(*d, *(size_t *) last);
    assert_true(rock_red_black_tree_insert(&object, d, e));
    assert_true(rock_red_black_tree_last(&object, &last));
    assert_ptr_equal(e, last);
    assert_int_equal(*e, *(size_t *) last);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_false(rock_red_black_tree_next(NULL, &node));
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
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 9;
    *b = 5;
    *c = 4;
    *d = 2;
    *e = 1;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(rock_red_black_tree_insert(&object, b, c));
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_insert(&object, d, e));
    void *node;
    assert_true(rock_red_black_tree_first(&object, &node));
    assert_ptr_equal(e, node);
    assert_int_equal(1, *(size_t *) node);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(d, node);
    assert_int_equal(2, *(size_t *) node);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(c, node);
    assert_int_equal(4, *(size_t *) node);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(b, node);
    assert_int_equal(5, *(size_t *) node);
    assert_true(rock_red_black_tree_next(node, &node));
    assert_ptr_equal(a, node);
    assert_int_equal(9, *(size_t *) node);
    assert_false(rock_red_black_tree_next(node, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    void *node;
    assert_false(rock_red_black_tree_prev(NULL, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_prev((void *) 1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 2;
    *c = 4;
    *d = 5;
    *e = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, b));
    assert_true(rock_red_black_tree_insert(&object, b, c));
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_insert(&object, d, e));
    void *node;
    assert_true(rock_red_black_tree_last(&object, &node));
    assert_ptr_equal(e, node);
    assert_int_equal(9, *(size_t *) node);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(d, node);
    assert_int_equal(5, *(size_t *) node);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(c, node);
    assert_int_equal(4, *(size_t *) node);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(b, node);
    assert_int_equal(2, *(size_t *) node);
    assert_true(rock_red_black_tree_prev(node, &node));
    assert_ptr_equal(a, node);
    assert_int_equal(1, *(size_t *) node);
    assert_false(rock_red_black_tree_prev(node, &node));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_delete_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_delete(NULL, (void *)1));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_delete_error_on_node_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_red_black_tree_delete((void*)1, NULL));
    assert_int_equal(ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_delete_leaf_nodes(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    *a = 9;
    *b = 4;
    assert_true(rock_red_black_tree_insert(&object, NULL, a));
    assert_true(rock_red_black_tree_insert(&object, a, b));
    bool color;
    void *a_, *b_, *_;
    assert_true(object.root == a
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(a, &b_)
                && b == b_
                && rock_red_black_tree_node_get_parent(b, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _);
    /* delete leaf */
    assert_true(rock_red_black_tree_delete(&object, b));
    assert_true(object.root == a
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _);
    /* delete root */
    assert_true(rock_red_black_tree_delete(&object, a));
    assert_null(object.root);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_delete_node_with_single_child(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 7;
    *c = 4;
    *d = 3;
    *e = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, c));
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_insert(&object, c, b));
    assert_true(rock_red_black_tree_insert(&object, d, a));
    assert_true(rock_red_black_tree_insert(&object, b, e));
    bool color;
    void *a_, *b_, *c_, *d_, *e_, *_;
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(c, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(d, &c_)
                && c == c_
                && rock_red_black_tree_node_get_right(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_right(d, &_)
                && NULL == _
                && rock_red_black_tree_node_get_left(d, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(a, &d_)
                && d == d_
                && rock_red_black_tree_node_get_left(b, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(b, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(e, &b_)
                && b == b_);
    /* single left node */
    assert_true(rock_red_black_tree_delete(&object, d));
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  b(7B)
     *               \
     *               e(9R)
     */
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(c, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(a, &c_)
                && c == c_
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_parent(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_left(b, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(b, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_parent(e, &b_)
                && b == b_);
    /* single right node */
    assert_true(rock_red_black_tree_delete(&object, b));
    /*
     *         c(4B)
     *         /  \
     *     a(1B)  e(9B)
     */
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(c, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(a, &c_)
                && c == c_
                && rock_red_black_tree_node_get_right(c, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(e, &c_)
                && c == c_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_delete_node_with_two_children(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *a, *b, *c, *d, *e;
    assert_true(rock_red_black_tree_node(sizeof(*a), (void **) &a));
    assert_true(rock_red_black_tree_node(sizeof(*b), (void **) &b));
    assert_true(rock_red_black_tree_node(sizeof(*c), (void **) &c));
    assert_true(rock_red_black_tree_node(sizeof(*d), (void **) &d));
    assert_true(rock_red_black_tree_node(sizeof(*e), (void **) &e));
    *a = 1;
    *b = 7;
    *c = 4;
    *d = 3;
    *e = 9;
    assert_true(rock_red_black_tree_insert(&object, NULL, c));
    assert_true(rock_red_black_tree_insert(&object, c, d));
    assert_true(rock_red_black_tree_insert(&object, c, b));
    assert_true(rock_red_black_tree_insert(&object, d, a));
    assert_true(rock_red_black_tree_insert(&object, b, e));
    bool color;
    void *a_, *b_, *c_, *d_, *e_, *_;
    /*
     *         c(4B)
     *         /  \
     *     d(3B)  b(7B)
     *      /        \
     *    a(1R)      e(9R)
     */
    assert_true(object.root == c
                && rock_red_black_tree_node_get_color(c, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(c, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(d, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(d, &c_)
                && c == c_
                && rock_red_black_tree_node_get_right(c, &b_)
                && b == b_
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(b, &c_)
                && c == c_
                && rock_red_black_tree_node_get_left(d, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(a, &d_)
                && d == d_
                && rock_red_black_tree_node_get_right(b, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(e, &b_)
                && b == b_);
    /* delete node with two children */
    assert_true(rock_red_black_tree_delete(&object, c));
    /*
     *         b(7B)
     *         /  \
     *     d(3B)  e(9B)
     *      /
     *    a(1R)
     */
    assert_true(object.root == b
                && rock_red_black_tree_node_get_color(b, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(b, &d_)
                && d == d_
                && rock_red_black_tree_node_get_color(d, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_right(d, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(d, &b_)
                && b == b_
                && rock_red_black_tree_node_get_left(d, &a_)
                && a == a_
                && rock_red_black_tree_node_get_color(a, &color)
                && ROCK_RED_BLACK_TREE_COLOR_RED == color
                && rock_red_black_tree_node_get_left(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(a, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(a, &d_)
                && d == d_
                && rock_red_black_tree_node_get_right(b, &e_)
                && e == e_
                && rock_red_black_tree_node_get_color(e, &color)
                && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                && rock_red_black_tree_node_get_left(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_right(e, &_)
                && NULL == _
                && rock_red_black_tree_node_get_parent(e, &b_)
                && b == b_);
    assert_true(rock_red_black_tree_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_tree(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_red_black_tree object = {};
    assert_true(rock_red_black_tree_init(&object,
                                         (int (*)(const void *, const void *))seagrass_size_t_ptr_compare));
    size_t *p = NULL;
    for (size_t i = 0; i < 10000; i++) {
        size_t *n;
        assert_true(rock_red_black_tree_node(sizeof(*n), (void **)&n));
        *n = i;
        assert_int_equal(i, object.count);
        assert_true(rock_red_black_tree_insert(&object, p, n));
        assert_int_equal(1 + i, object.count);
        p = n;
    }
    for (size_t i = 0; i < 10000; i++) {
        assert_int_equal(10000 - i, object.count);
        assert_true(rock_red_black_tree_delete(&object, object.root));
        assert_int_equal(9999 - i, object.count);
    }
    assert_null(object.root);
    assert_int_equal(0, object.count);
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_node_alloc_error_on_out_is_null),
            cmocka_unit_test(check_node_alloc_error_on_memory_allocation_failed),
            cmocka_unit_test(check_node_alloc),
            cmocka_unit_test(check_node_init_error_on_node_is_null),
            cmocka_unit_test(check_node_init),
            cmocka_unit_test(check_node_error_on_out_is_null),
            cmocka_unit_test(check_node_error_on_memory_allocation_failed),
            cmocka_unit_test(check_node),
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
            cmocka_unit_test(check_find_error_on_value_is_null),
            cmocka_unit_test(check_find_error_on_out_is_null),
            cmocka_unit_test(check_find_error_on_value_not_found),
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
            cmocka_unit_test(check_get_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev),
            cmocka_unit_test(check_delete_error_on_object_is_null),
            cmocka_unit_test(check_delete_error_on_node_is_null),
            cmocka_unit_test(check_delete_leaf_nodes),
            cmocka_unit_test(check_delete_node_with_single_child),
            cmocka_unit_test(check_delete_node_with_two_children),
            cmocka_unit_test(check_tree)
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
