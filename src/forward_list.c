#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

bool rock_forward_list_node_init(struct rock_forward_list_node *const object) {
    if (!object) {
        rock_error = ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    *object = (struct rock_forward_list_node) {0};
    return true;
}

bool rock_forward_list_insert(struct rock_forward_list_node *const A,
                              struct rock_forward_list_node *const C) {
    if (!A) {
        rock_error = ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!C) {
        rock_error = ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    /*
     * Before:
     * [A] -----------> [B]
     * Insert node 'C' after node 'A'.
     *
     * After:
     * [A] -----------> [C] -----------> [B]
     *                  .next = B
     * .next = C
     */
    struct rock_forward_list_node *const B = A->next;
    if (B) {
        C->next = B;
    }
    A->next = C;
    return true;
}

bool rock_forward_list_remove(struct rock_forward_list_node *const A,
                              struct rock_forward_list_node *const B) {
    if (!A) {
        rock_error = ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!B) {
        rock_error = ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    if (A->next != B) {
        rock_error = ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT;
        return false;
    }
    /*
     * Before:
     * [A] ------------> [B] ------------> [C]
     * Remove node 'B'.
     *
     * After:
     * [A] ------------> [C]
     * .next = C
     */
    struct rock_forward_list_node *const C = B->next;
    A->next = C;
    seagrass_required_true(rock_forward_list_node_init(B));
    return true;
}

bool rock_forward_list_next(const struct rock_forward_list_node *const node,
                            struct rock_forward_list_node **const out) {
    if (!node) {
        rock_error = ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node->next) {
        rock_error = ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE;
        return false;
    }
    *out = node->next;
    return true;
}
