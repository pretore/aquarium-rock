#ifndef _ROCK_RED_BLACK_TREE_H_
#define _ROCK_RED_BLACK_TREE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sea-urchin.h>

#define ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL \
    SEA_URCHIN_ERROR_ITEM_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL \
    SEA_URCHIN_ERROR_OUT_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE \
    SEA_URCHIN_ERROR_END_OF_SEQUENCE
#define ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL \
    SEA_URCHIN_ERROR_OBJECT_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL \
    SEA_URCHIN_ERROR_FUNCTION_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_NEEDLE_IS_NULL \
    SEA_URCHIN_ERROR_VALUE_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL \
    SEA_URCHIN_ERROR_OTHER_IS_NULL
#define ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY \
    SEA_URCHIN_ERROR_IS_EMPTY
#define ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND \
    SEA_URCHIN_ERROR_VALUE_NOT_FOUND
#define ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS \
    SEA_URCHIN_ERROR_VALUE_ALREADY_EXISTS

struct rock_red_black_tree_node {
    /* color bit is stored in the lowest bit */
    struct rock_red_black_tree_node *parent;
    struct rock_red_black_tree_node *left;
    struct rock_red_black_tree_node *right;
};

/**
 * @brief Initialize the node instance.
 * @param [in] node instance to be initialized.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
int rock_red_black_tree_node_init(struct rock_red_black_tree_node *node);

struct rock_red_black_tree {
    struct rock_red_black_tree_node *root;
    uintmax_t count;

    int (*compare)(const struct rock_red_black_tree_node *,
                   const struct rock_red_black_tree_node *);
};

/**
 * @brief Initialise red black tree instance.
 * @param [in] object instance to be initialised.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first node</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * node</u>.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL if compare is <i>NULL</i>.
 */
int rock_red_black_tree_init(
        struct rock_red_black_tree *object,
        int (*compare)(const struct rock_red_black_tree_node *first,
                       const struct rock_red_black_tree_node *second));

/**
 * @brief Invalidate the red black tree instance.
 * <p>The nodes in the red black tree are destroyed and each node will have the
 * provided <i>on destroy</i> callback invoked upon itself. The actual <u>red
 * black tree instance is not deallocated</u> since it may have been embedded
 * in a larger structure.</p>
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the node is to be destroyed.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
int rock_red_black_tree_invalidate(
        struct rock_red_black_tree *object,
        void (*on_destroy)(struct rock_red_black_tree_node *node));

/**
 * @brief Retrieve the count of items in the red black tree.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count of the red black tree.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_red_black_tree_count(const struct rock_red_black_tree *object,
                              uintmax_t *out);

/**
 * @brief Find a node in red black tree.
 * <p>Find the node in the red black tree instance or the point at which the
 * node will be inserted.</p>
 * @param [in] object instance to be searched.
 * @param [in] root restrict the search to subtree.
 * @param [in] needle used to find the node.
 * @param [out] out receive the node or node's insertion point.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NEEDLE_IS_NULL if needle is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND if node was not found.
 */
int rock_red_black_tree_find(const struct rock_red_black_tree *object,
                             const struct rock_red_black_tree_node *root,
                             const struct rock_red_black_tree_node *needle,
                             struct rock_red_black_tree_node **out);

/**
 * @brief Insert node.
 * @param [in] object instance of red black tree.
 * @param [in] insertion_point obtained by performing find.
 * @param [in] node which is to be inserted at insertion point.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL if insertion point
 * is <i>NULL</i> when the tree is not empty.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS if node with value is
 * already present in the red black tree instance.
 */
int rock_red_black_tree_insert(
        struct rock_red_black_tree *object,
        struct rock_red_black_tree_node *insertion_point,
        struct rock_red_black_tree_node *node);

/**
 * @brief Remove node.
 * @param [in] object instance of red black tree.
 * @param [in] node which is to be removed.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
int rock_red_black_tree_remove(struct rock_red_black_tree *object,
                               struct rock_red_black_tree_node *node);

/**
 * @brief First value in the red black tree.
 * @param [in] object instance from which the first value is to be retrieved.
 * @param [out] out receive the first value.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY if the red black tree is
 * empty.
 */
int rock_red_black_tree_first(const struct rock_red_black_tree *object,
                              struct rock_red_black_tree_node **out);

/**
 * @brief Last value in the red black tree.
 * @param [in] object instance from which the last value is to be retrieved.
 * @param [out] out receive the last value.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY if the red black tree is
 * empty.
 */
int rock_red_black_tree_last(const struct rock_red_black_tree *object,
                             struct rock_red_black_tree_node **out);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE if there is no next node.
 */
int rock_red_black_tree_next(const struct rock_red_black_tree_node *node,
                             struct rock_red_black_tree_node **out);

/**
 * @brief Retrieve the previous node.
 * @param [in] node whose previous node we are to retrieve.
 * @param [out] out receive the previous node.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE if there is no previous
 * node.
 */
int rock_red_black_tree_prev(const struct rock_red_black_tree_node *node,
                             struct rock_red_black_tree_node **out);

#endif /* _ROCK_RED_BLACK_TREE_H_ */
