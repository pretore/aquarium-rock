#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

#ifdef TEST
#include <test/cmocka.h>
#endif

int rock_forward_list_node_init(struct rock_forward_list_node *const object) {
    if (!object) {
        return ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL;
    }
    *object = (struct rock_forward_list_node) {0};
    return 0;
}

int rock_forward_list_insert(struct rock_forward_list_node *const A,
                             struct rock_forward_list_node *const C) {
    if (!A) {
        return ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL;
    }
    if (!C) {
        return ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
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
    return 0;
}

int rock_forward_list_remove(struct rock_forward_list_node *const A,
                             struct rock_forward_list_node *const B) {
    if (!A) {
        return ROCK_FORWARD_LIST_ERROR_OBJECT_IS_NULL;
    }
    if (!B) {
        return ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
    }
    if (A->next != B) {
        return ROCK_FORWARD_LIST_ERROR_NODE_NOT_NEXT_OF_OBJECT;
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
    seagrass_required_true(!rock_forward_list_node_init(B));
    return 0;
}

int rock_forward_list_next(const struct rock_forward_list_node *const node,
                           struct rock_forward_list_node **const out) {
    if (!node) {
        return ROCK_FORWARD_LIST_ERROR_NODE_IS_NULL;
    }
    if (!out) {
        return ROCK_FORWARD_LIST_ERROR_OUT_IS_NULL;
    }
    if (!node->next) {
        return ROCK_FORWARD_LIST_ERROR_END_OF_SEQUENCE;
    }
    *out = node->next;
    return 0;
}
