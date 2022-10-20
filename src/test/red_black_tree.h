#ifndef _ROCK_TEST_RED_BLACK_TREE_H_
#define _ROCK_TEST_RED_BLACK_TREE_H_

#ifdef TEST
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct rock_red_black_tree_node;

/**
 * @brief Assert the node by examining its properties.
 * @param [in] object node to be asserted.
 * @param [in] parent of node.
 * @param [in] is_left_on_parent is the node on the left of the parent.
 * @param [in] left child of node.
 * @param [in] right child of node.
 * @param [in] color either ROCK_RED_BLACK_TREE_COLOR_RED or
 * ROCK_RED_BLACK_TREE_COLOR_BLACK.
 */
void assert_red_black_tree_node(
        const struct rock_red_black_tree_node *object,
        const struct rock_red_black_tree_node *parent,
        bool is_left_on_parent,
        const struct rock_red_black_tree_node *left,
        const struct rock_red_black_tree_node *right,
        bool color);

struct rock_red_black_tree;

/**
 * @brief Assert that the red-black tree is conformant.
 * <p>In addition to the requirements imposed on a binary search tree the
 * following must be satisfied by a red–black tree:
 * <ul>
 * <li/> Every node is either red or black.
 * <li/> All NIL nodes are considered black.
 * <li/> A red node does not have a red child.
 * <li/> Every path from a given node to any of its descendant NIL nodes goes
 * through the same number of black nodes.
 * </ul></p>
 * @param [in] object red-black tree instance.
 */
void assert_red_black_tree(const struct rock_red_black_tree *object);

#endif /* TEST */
#endif /* _ROCK_TEST_RED_BLACK_TREE_H_ */
