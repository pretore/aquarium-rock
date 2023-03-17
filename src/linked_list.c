#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

#ifdef TEST
#include <test/cmocka.h>
#endif

int rock_linked_list_node_init(struct rock_linked_list_node *const object) {
    if (!object) {
        return ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
    }
    object->prev = object;
    object->next = object;
    return 0;
}

int rock_linked_list_insert_after(struct rock_linked_list_node *const A,
                                  struct rock_linked_list_node *const C) {
    if (!A) {
        return ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
    }
    if (!C) {
        return ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
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
    return 0;
}

int rock_linked_list_insert_before(struct rock_linked_list_node *const A,
                                   struct rock_linked_list_node *const C) {
    if (!A) {
        return ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
    }
    if (!C) {
        return ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
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
    return 0;
}

int rock_linked_list_remove(struct rock_linked_list_node *const C) {
    if (!C) {
        return ROCK_LINKED_LIST_ERROR_OBJECT_IS_NULL;
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
    seagrass_required_true(!rock_linked_list_node_init(C));
    return 0;
}

int rock_linked_list_next(const struct rock_linked_list_node *const node,
                          struct rock_linked_list_node **const out) {
    if (!node) {
        return ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
    }
    if (!out) {
        return ROCK_LINKED_LIST_ERROR_OUT_IS_NULL;
    }
    *out = node->next;
    return 0;
}

int rock_linked_list_prev(const struct rock_linked_list_node *const node,
                          struct rock_linked_list_node **const out) {
    if (!node) {
        return ROCK_LINKED_LIST_ERROR_NODE_IS_NULL;
    }
    if (!out) {
        return ROCK_LINKED_LIST_ERROR_OUT_IS_NULL;
    }
    *out = node->prev;
    return 0;
}
