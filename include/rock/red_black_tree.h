#ifndef _ROCK_RED_BLACK_TREE_H_
#define _ROCK_RED_BLACK_TREE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL                  1
#define ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL                   2
#define ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED      3
#define ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE               4
#define ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL                5
#define ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL               6
#define ROCK_RED_BLACK_TREE_ERROR_VALUE_IS_NULL                 7
#define ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL       8
#define ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY                 9
#define ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND               10
#define ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS           11

/**
 * @brief Create red black tree node instance.
 * @param [in] size bytes needed to be allocated for node instance.
 * @param [out] out receive the created node instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to allocate the node instance.
 */
bool rock_red_black_tree_node(size_t size, void **out);

/**
 * @brief Destroy the red black tree node instance.
 * @param [in] node instance to be destroyed.
 */
void rock_red_black_tree_node_destroy(void *node);

struct rock_red_black_tree {
    void *root;
    size_t count;

    int (*compare)(const void *, const void *);
};

/**
 * @brief Initialise red black tree instance.
 * @param [in] object instance to be initialised.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first node</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * node</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL if compare is <i>NULL</i>.
 */
bool rock_red_black_tree_init(struct rock_red_black_tree *object,
                              int (*compare)(const void *first,
                                             const void *second));

/**
 * @brief Invalidate the red black tree instance.
 * <p>The nodes in the red black tree are destroyed and each node will have the
 * provided <i>on destroy</i> callback invoked upon itself. The actual <u>red
 * black tree instance is not deallocated</u> since it may have been embedded
 * in a larger structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the node is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_red_black_tree_invalidate(struct rock_red_black_tree *object,
                                    void (*on_destroy)(void *));

/**
 * @brief Retrieve the count of items in the red black tree.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count of the red black tree.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_red_black_tree_count(struct rock_red_black_tree *object,
                               size_t *out);

/**
 * @brief Find given value in red black tree.
 * <p>Find the value in the red black tree instance or the point at which the
 * value would have been inserted.</p>
 * @param [in] object instance to be searched.
 * @param [in] root scope the search if not specified then whole red black
 * tree will be searched for value.
 * @param [in] value to find in the tree instance.
 * @param [out] out receive the value's node or insertion point.
 * @return If exact match found true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_VALUE_IS_NULL if value is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND if value was not found.
 */
bool rock_red_black_tree_find(struct rock_red_black_tree *object,
                              const void *root,
                              const void *value,
                              void **out);

/**
 * @brief Insert node.
 * @param [in] object instance of red black tree which node is to be inserted.
 * @param [in] insertion_point obtained by performing find.
 * @param [in] node which is to be inserted at insertion point.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL if insertion point
 * is <i>NULL</i> when the tree is not empty.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS if node with value is
 * already present in the red black tree instance.
 */
bool rock_red_black_tree_insert(struct rock_red_black_tree *object,
                                void *insertion_point,
                                void *node);

/**
 * @brief Delete node.
 * @param [in] object instance of red black tree which node is to be deleted.
 * @param [in] node which is to be deleted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
bool rock_red_black_tree_delete(struct rock_red_black_tree *object,
                                void *node);

/**
 * @brief First value in the red black tree.
 * @param [in] object instance from which the first value is to be retrieved.
 * @param [out] out receive the first value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY if the red black tree is
 * empty.
 */
bool rock_red_black_tree_first(struct rock_red_black_tree *object,
                               void **out);

/**
 * @brief Last value in the red black tree.
 * @param [in] object instance from which the last value is to be retrieved.
 * @param [out] out receive the last value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY if the red black tree is
 * empty.
 */
bool rock_red_black_tree_last(struct rock_red_black_tree *object,
                              void **out);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE if there is no next node.
 */
bool rock_red_black_tree_next(void *node, void **out);

/**
 * @brief Retrieve the previous node.
 * @param [in] node whose previous node we are to retrieve.
 * @param [out] out receive the previous node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE if there is no previous
 * node.
 */
bool rock_red_black_tree_prev(void *node, void **out);

#endif /* _ROCK_RED_BLACK_TREE_H_ */
