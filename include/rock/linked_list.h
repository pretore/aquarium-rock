#ifndef _ROCK_LINKED_LIST_TREE_H_
#define _ROCK_LINKED_LIST_TREE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL                   1
#define ROCK_LINKED_LIST_ERROR_NODE_IS_NULL                     2
#define ROCK_LINKED_LIST_ERROR_OUT_IS_NULL                      3

struct rock_linked_list_node {
    struct rock_linked_list_node *next;
    struct rock_linked_list_node *prev;
};

/**
 * @brief Initialize the node instance.
 * @param [in] node instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_linked_list_node_init(struct rock_linked_list_node *object);

/**
 * @brief Insert node after the given object.
 * @param [in] object where node will be inserted after.
 * @param [in] node to be inserted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
bool rock_linked_list_insert_after(struct rock_linked_list_node *object,
                                   struct rock_linked_list_node *node);

/**
 * @brief Insert node before the given object.
 * @param [in] object where node will be inserted before.
 * @param [in] node to be inserted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
bool rock_linked_list_insert_before(struct rock_linked_list_node *object,
                                    struct rock_linked_list_node *node);

/**
 * @brief Remove node from linked list.
 * @param [in] object to remove from the linked list.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_linked_list_remove(struct rock_linked_list_node *object);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_linked_list_next(const struct rock_linked_list_node *node,
                           struct rock_linked_list_node **out);

/**
 * @brief Retrieve the previous node.
 * @param [in] node whose previous node we are to retrieve.
 * @param [out] out receive the previous node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_linked_list_prev(const struct rock_linked_list_node *node,
                           struct rock_linked_list_node **out);

#endif /* _ROCK_LINKED_LIST_TREE_H_ */
