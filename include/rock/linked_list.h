#ifndef _ROCK_LINKED_LIST_H_
#define _ROCK_LINKED_LIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sea-urchin.h>

#define ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL \
    SEA_URCHIN_ERROR_OBJECT_IS_NULL
#define ROCK_LINKED_LIST_ERROR_NODE_IS_NULL \
    SEA_URCHIN_ERROR_ITEM_IS_NULL
#define ROCK_LINKED_LIST_ERROR_OUT_IS_NULL \
    SEA_URCHIN_ERROR_OUT_IS_NULL

struct rock_linked_list_node {
    struct rock_linked_list_node *next;
    struct rock_linked_list_node *prev;
};

/**
 * @brief Initialize the node instance.
 * @param [in] node instance to be initialized.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
int rock_linked_list_node_init(struct rock_linked_list_node *object);

/**
 * @brief Insert after the given node.
 * @param [in] object where node will be inserted after.
 * @param [in] node to be inserted.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
int rock_linked_list_insert_after(struct rock_linked_list_node *object,
                                  struct rock_linked_list_node *node);

/**
 * @brief Insert before the given node.
 * @param [in] object where node will be inserted before.
 * @param [in] node to be inserted.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
int rock_linked_list_insert_before(struct rock_linked_list_node *object,
                                   struct rock_linked_list_node *node);

/**
 * @brief Remove node from linked list.
 * @param [in] object to remove from the linked list.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
int rock_linked_list_remove(struct rock_linked_list_node *object);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_linked_list_next(const struct rock_linked_list_node *node,
                          struct rock_linked_list_node **out);

/**
 * @brief Retrieve the previous node.
 * @param [in] node whose previous node we are to retrieve.
 * @param [out] out receive the previous node.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_LINKED_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_LINKED_LIST_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_linked_list_prev(const struct rock_linked_list_node *node,
                          struct rock_linked_list_node **out);

#endif /* _ROCK_LINKED_LIST_H_ */
