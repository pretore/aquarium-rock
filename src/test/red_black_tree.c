#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <errno.h>
#include <rock.h>
#include <seagrass.h>

#include "red_black_tree.h"
#include "../private/red_black_tree.h"

void assert_red_black_tree_node(
        const struct rock_red_black_tree_node *const object,
        const struct rock_red_black_tree_node *const parent,
        const bool is_left_on_parent,
        const struct rock_red_black_tree_node *const left,
        const struct rock_red_black_tree_node *const right,
        const bool color) {
    bool color_;
    assert_int_equal(rock_red_black_tree_node_get_color(object, &color_), 0);
    assert_int_equal(color_, color);
    struct rock_red_black_tree_node *out;

    assert_int_equal(rock_red_black_tree_node_get_parent(object, &out), 0);
    if (!parent) {
        assert_null(out);
    } else {
        assert_ptr_equal(out, parent);
        assert_int_equal(is_left_on_parent
                    ? rock_red_black_tree_node_get_left(parent, &out)
                    : rock_red_black_tree_node_get_right(parent, &out), 0);
        assert_ptr_equal(out, object);
    }

    assert_int_equal(rock_red_black_tree_node_get_left(object, &out), 0);
    if (!left) {
        assert_null(out);
    } else {
        assert_ptr_equal(out, left);
        assert_int_equal(rock_red_black_tree_node_get_parent(left, &out), 0);
        assert_ptr_equal(out, object);
    }

    assert_int_equal(rock_red_black_tree_node_get_right(object, &out), 0);
    if (!right) {
        assert_null(out);
    } else {
        assert_ptr_equal(out, right);
        assert_int_equal(rock_red_black_tree_node_get_parent(right, &out), 0);
        assert_ptr_equal(out, object);
    }
}

void assert_red_black_tree(const struct rock_red_black_tree *const object) {
    seagrass_required(object);
    if (!object->count) {
        return;
    }
    bool color;
    /* RULE: root node must be BLACK */
    assert_true(!rock_red_black_tree_node_get_color(object->root, &color));
    assert_int_equal(color, ROCK_RED_BLACK_TREE_COLOR_BLACK);

    struct {
        const struct rock_red_black_tree_node **data;
        uintmax_t next;
        uintmax_t limit;
    } stack = {};

    /* Height of a balanced binary tree is log2(n). */
    for (uintmax_t i = object->count; i; i >>= 1, stack.limit++);
    /* No more than double the height of a balanced binary tree on any path is
     * expected, since there may be at most one RED node between BLACK nodes.
     * Every node on the path can be expanded to hold 2 nodes on the stack. */
    assert_true(!seagrass_uintmax_t_multiply(4, stack.limit, &stack.limit));
    uintmax_t size;
    assert_true(!seagrass_uintmax_t_multiply(sizeof(void *), stack.limit,
                                            &size));
    stack.data = malloc(size);
    assert_non_null(stack.data);

    uintmax_t height = 0;
    stack.data[stack.next] = object->root;
    assert_true(!seagrass_uintmax_t_add(1, stack.next, &stack.next));
    while (stack.data[0]) {
        assert_true(!seagrass_uintmax_t_subtract(stack.next, 1, &stack.next));
        const struct rock_red_black_tree_node *node = stack.data[stack.next];
        stack.data[stack.next] = NULL;
        int error;
        /* Expand the current node */
        struct rock_red_black_tree_node *children[2] = {};
        if ((error = rock_red_black_tree_node_get_left(node, &children[0]))
            || (error = rock_red_black_tree_node_get_right(node,
                                                           &children[1]))) {
            seagrass_required_true(ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE
                                   == error);
        }
        /* RULE: every RED node must only have BLACK children (NIL children
         * are considered BLACK) */
        assert_true(!rock_red_black_tree_node_get_color(node, &color));
        if (ROCK_RED_BLACK_TREE_COLOR_RED == color) {
            assert_true(!rock_red_black_tree_node_get_color(
                    children[0], &color));
            assert_int_equal(color, ROCK_RED_BLACK_TREE_COLOR_BLACK);
            assert_true(!rock_red_black_tree_node_get_color(
                    children[1], &color));
            assert_int_equal(color, ROCK_RED_BLACK_TREE_COLOR_BLACK);
        }
        /* RULE: every path from the root to the NIL nodes must have the same
         * number of BLACK nodes */
        if (NULL == children[0] && NULL == children[1]) {
            uintmax_t count = 0;
            for (struct rock_red_black_tree_node *p
                    = (struct rock_red_black_tree_node *) node; p; ) {
                assert_true(!rock_red_black_tree_node_get_color(p, &color));
                if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
                    seagrass_required_true(!seagrass_uintmax_t_add(
                            1, count, &count));
                }
                assert_true(!rock_red_black_tree_node_get_parent(p, &p));
            }
            if (!height) {
                height = count;
            } else {
                assert_int_equal(height, count);
            }
            continue;
        }
        /* Add children to stack */
        for (uintmax_t i = 2; i; i--) {
            uintmax_t index;
            assert_true(!seagrass_uintmax_t_subtract(i, 1, &index));
            if (!children[index]) {
                continue;
            }
            stack.data[stack.next] = children[index];
            assert_true(!seagrass_uintmax_t_add(1, stack.next, &stack.next));
            assert_true(stack.next <= stack.limit);
        }
    }
    free(stack.data);
}
