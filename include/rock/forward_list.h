#ifndef _ROCK_FORWARD_LIST_H_
#define _ROCK_FORWARD_LIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL                  1
#define ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL                    2
#define ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT         3
#define ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL                     4
#define ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE                 5

struct rock_forward_list_node {
    struct rock_forward_list_node *next;
};

/**
 * @brief Initialize the node instance.
 * @param [in] node instance to be initialized.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_forward_list_node_init(struct rock_forward_list_node *object);

/**
 * @brief Insert after the given node.
 * @param [in] object where node will be inserted after.
 * @param [in] node to be inserted.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 */
bool rock_forward_list_insert(struct rock_forward_list_node *object,
                              struct rock_forward_list_node *node);

/**
 * @brief Remove node from forward list.
 * @param [in] object from which node is to be removed.
 * @param [in] to be removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT if node is not
 * next of object.
 */
bool rock_forward_list_remove(struct rock_forward_list_node *object,
                              struct rock_forward_list_node *node);

/**
 * @brief Retrieve the next node.
 * @param [in] node whose next node we are to retrieve.
 * @param [out] out receive the next node.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL if node is <i>NULL</i>.
 * @throws ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE if there are no more next
 * nodes.
 */
bool rock_forward_list_next(const struct rock_forward_list_node *node,
                            struct rock_forward_list_node **out);

#endif /* _ROCK_FORWARD_LIST_H_ */
