#ifndef _ROCK_PRIVATE_RED_BLACK_TREE_H_
#define _ROCK_PRIVATE_RED_BLACK_TREE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROCK_RED_BLACK_TREE_COLOR_RED                           0
#define ROCK_RED_BLACK_TREE_COLOR_BLACK                         1

#define ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL                     (-1)
#define ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT               (-2)
#define ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X      (-3)
#define ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X     (-4)
#define ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT               (-5)
#define ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z      (-6)
#define ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z     (-7)

struct rock_red_black_tree_node;

/**
 * @brief Check if a node is on the left or right.
 * @param [in] node whose left and right will be checked.
 * @param [in] left expected node to be on the left.
 * @return True if left is on the left of node, false otherwise.
 * @note abort(3) is called if node is <i>NULL</i>.
 * @note abort(3) is called if left is not held by node at all.
 */
bool rock_red_black_tree_node_is_on_left(
        const struct rock_red_black_tree_node *node,
        const struct rock_red_black_tree_node *left);

bool rock_red_black_tree_rotate_N(struct rock_red_black_tree_node *Y,
                                  bool is_N_left);

bool rock_red_black_tree_rotate_N_O(struct rock_red_black_tree_node *Y,
                                    bool is_N_left);

void rock_red_black_tree_minimum(const struct rock_red_black_tree_node *root,
                                 struct rock_red_black_tree_node **out);

void rock_red_black_tree_maximum(const struct rock_red_black_tree_node *root,
                                 struct rock_red_black_tree_node **out);

/**
 * @brief Retrieve parent of node.
 * @param [in] node whose parent we are to retrieve.
 * @param [out] out receive the parent of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_red_black_tree_node_get_parent(
        const struct rock_red_black_tree_node *node,
        struct rock_red_black_tree_node **out);

/**
 * @brief Set the parent of node.
 * @param [in] node whose parent we are to set.
 * @param [in] parent which will be the node's parent.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @note abort(3) is called if node is equal to parent.
 */
bool rock_red_black_tree_node_set_parent(
        struct rock_red_black_tree_node *restrict node,
        struct rock_red_black_tree_node *restrict parent);

/**
 * @brief Retrieve the color of node.
 * @param [in] node whose color we are to retrieve.
 * @param [out] out receive the color of the node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if node is <i>NULL</i>.
 */
bool rock_red_black_tree_node_get_color(
        const struct rock_red_black_tree_node *node, bool *out);

/**
 * @brief Set the color of the node.
 * @param [in] node whose color we are to set.
 * @param [in] colour which will be the node's color.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
bool rock_red_black_tree_node_set_color(struct rock_red_black_tree_node *node,
                                        bool colour);

/**
 * @brief Retrieve left child of node.
 * @param [in] node whose left child node we are to retrieve.
 * @param [out] out receive the left child node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_REF_BLACK_TREE_ERROR_END_OF_SEQUENCE if trying to get left
 * child node from a NIL node.
 */
bool rock_red_black_tree_node_get_left(
        const struct rock_red_black_tree_node *node,
        struct rock_red_black_tree_node **out);

/**
 * @brief Set the left child of node.
 * @param [in] node whose left child we are to set.
 * @param [in] left which will be the left child of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @note abort(3) is called if node is equal to left.
 */
bool rock_red_black_tree_node_set_left(
        struct rock_red_black_tree_node *restrict node,
        struct rock_red_black_tree_node *restrict left);

/**
 * @brief Retrieve right child of node.
 * @param [in] node whose left right node we are to retrieve.
 * @param [out] out receive the right child node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_REF_BLACK_TREE_ERROR_END_OF_SEQUENCE if trying to get right
 * child node from a NIL node.
 */
bool rock_red_black_tree_node_get_right(
        const struct rock_red_black_tree_node *node,
        struct rock_red_black_tree_node **out);

/**
 * @brief Set the right child of node.
 * @param [in] node whose right child we are to set.
 * @param [in] right which will be the right child of node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @note abort(3) is called if node is equal to right.
 */
bool rock_red_black_tree_node_set_right(
        struct rock_red_black_tree_node *restrict node,
        struct rock_red_black_tree_node *restrict right);

/**
 * @brief Perform left rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (x)               (y)
 *     / \       =>      / \
 *    a  (y)           (x)  c
 *       / \           / \
 *      b   c         a   b
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
 */
bool rock_red_black_tree_rotate_left(struct rock_red_black_tree_node *node);

/**
 * @brief Perform right rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (x)               (y)
 *     / \       =>      / \
 *   (y)  c             a  (x)
 *   / \                   / \
 *  a   b                 b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X
 */
bool rock_red_black_tree_rotate_right(struct rock_red_black_tree_node *node);

/**
 * @brief Perform left-right rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (z)               (y)
 *     / \       =>      / \
 *   (x)  d            (x)  (z)
 *   / \               / \  / \
 *  a  (y)            a   bc   d
 *     / \
 *    b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
 * @throws ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z
 */
bool
rock_red_black_tree_rotate_left_right(struct rock_red_black_tree_node *node);

/**
 * @brief Perform right-left rotation.
 * @verbatim
 *      p                 p
 *      |                 |
 *     (z)               (y)
 *     / \       =>      / \
 *    a  (x)           (z)  (x)
 *       / \           / \  / \
 *     (y)  d         a   bc   d
 *     / \
 *    b   c
 * @endverbatim
 * @param [in] node (y) in the picture above before rotation is performed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X
 * @throws ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT
 * @throws ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z
 */
bool
rock_red_black_tree_rotate_right_left(struct rock_red_black_tree_node *node);

#endif /* _ROCK_PRIVATE_RED_BLACK_TREE_H_ */
