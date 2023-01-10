#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

#include <test/cmocka.h>

bool rock_linked_list_node_init(struct rock_linked_list_node *const object) {
    if (!object) {
        rock_error = ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    object->prev = object;
    object->next = object;
    return true;
}

bool rock_linked_list_insert_after(struct rock_linked_list_node *const A,
                                   struct rock_linked_list_node *const C) {
    if (!A) {
        rock_error = ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!C) {
        rock_error = ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    /*
     * Before:
     * [A] <----------> [B]
     * Insert node 'C' after node 'A'.
     *
     * After:
     * [A] <----------> [C] <----------> [B]
     * .next = C        .prev = A
     *                  .next = B        .prev = C
     */
    struct rock_linked_list_node *const B = A->next;
    A->next = C;
    C->prev = A;
    C->next = B;
    B->prev = C;
    return true;
}

bool rock_linked_list_insert_before(struct rock_linked_list_node *const A,
                                    struct rock_linked_list_node *const C) {
    if (!A) {
        rock_error = ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!C) {
        rock_error = ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    /*
     * Before:
     * [B] <----------> [A]
     * Insert node 'C' before 'A'.
     *
     * After:
     * [B] <----------> [C] <----------> [A]
     * .next = C        .prev = B
     *                  .next = A        .prev = C
     */
    struct rock_linked_list_node *const B = A->prev;
    B->next = C;
    C->prev = B;
    C->next = A;
    A->prev = C;
    return true;
}

bool rock_linked_list_remove(struct rock_linked_list_node *const C) {
    if (!C) {
        rock_error = ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
        return false;
    }
    /*
     * Before:
     * [A] <-----------> [C] <-----------> [B]
     * Remove node 'C'.
     *
     * After:
     * [A] <-----X-----> [C] <-----X-----> [B]
     * .next = B                           .prev = A
     *                   .next = C
     *                   .prev = C
     */
    struct rock_linked_list_node *const A = C->prev;
    struct rock_linked_list_node *const B = C->next;
    A->next = B;
    B->prev = A;
    seagrass_required_true(rock_linked_list_node_init(C));
    return true;
}

bool rock_linked_list_next(const struct rock_linked_list_node *const node,
                           struct rock_linked_list_node **const out) {
    if (!node) {
        rock_error = ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_LINKED_LIST_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = node->next;
    return true;
}

bool rock_linked_list_prev(const struct rock_linked_list_node *const node,
                           struct rock_linked_list_node **const out) {
    if (!node) {
        rock_error = ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_LINKED_LIST_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = node->prev;
    return true;
}
