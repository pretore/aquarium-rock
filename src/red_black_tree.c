#include <stdlib.h>
#include <seagrass.h>
#include <rock.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"

bool rock_red_black_tree_node_init(
        struct rock_red_black_tree_node *const node) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    *node = (struct rock_red_black_tree_node) {0};
    node->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    node->left = NULL;
    node->right = NULL;
    return true;
}

bool rock_red_black_tree_node_get_parent(
        const struct rock_red_black_tree_node *const node,
        struct rock_red_black_tree_node **const out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = (struct rock_red_black_tree_node *) ((uintptr_t) node->parent & ~1);
    return true;
}

bool rock_red_black_tree_node_set_parent(
        struct rock_red_black_tree_node *restrict const node,
        struct rock_red_black_tree_node *restrict const parent) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    seagrass_required_true(node != parent);
    bool color;
    seagrass_required_true(rock_red_black_tree_node_get_color(
            node, &color));
    const uintptr_t value = (uintptr_t) parent | color;
    node->parent = (struct rock_red_black_tree_node *) value;
    return true;
}

bool rock_red_black_tree_node_get_color(
        const struct rock_red_black_tree_node *const node,
        bool *const out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes are black */
        *out = ROCK_RED_BLACK_TREE_COLOR_BLACK;
        return true;
    }
    *out = (uintmax_t) node->parent & 1;
    return true;
}

bool rock_red_black_tree_node_set_color(
        struct rock_red_black_tree_node *const node,
        const bool color) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    const uintmax_t value = (~1 & (uintmax_t) node->parent) | (uintmax_t) color;
    node->parent = (struct rock_red_black_tree_node *) value;
    return true;
}

bool rock_red_black_tree_node_get_left(
        const struct rock_red_black_tree_node *const node,
        struct rock_red_black_tree_node **const out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
        return false;
    }
    *out = node->left;
    return true;
}

bool rock_red_black_tree_node_set_left(
        struct rock_red_black_tree_node *restrict const node,
        struct rock_red_black_tree_node *restrict const left) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    seagrass_required_true(node != left);
    node->left = left;
    return true;
}

bool rock_red_black_tree_node_get_right(
        const struct rock_red_black_tree_node *const node,
        struct rock_red_black_tree_node **const out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
        return false;
    }
    *out = node->right;
    return true;
}

bool rock_red_black_tree_node_set_right(
        struct rock_red_black_tree_node *restrict const node,
        struct rock_red_black_tree_node *restrict const right) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    seagrass_required_true(node != right);
    node->right = right;
    return true;
}

bool rock_red_black_tree_node_is_on_left(
        const struct rock_red_black_tree_node *const node,
        const struct rock_red_black_tree_node *const left) {
    seagrass_required(node);
    const bool is_left = node->left == left;
    if (!is_left) {
        seagrass_required_true(node->right == left);
    }
    return is_left;
}

bool rock_red_black_tree_next(const struct rock_red_black_tree_node *node,
                              struct rock_red_black_tree_node **const out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *right;
    seagrass_required_true(rock_red_black_tree_node_get_right(
            node, &right));
    if (right) {
        rock_red_black_tree_minimum(right, out);
        return true;
    }
    struct rock_red_black_tree_node *parent;
    while (true) {
        seagrass_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        seagrass_required_true(rock_red_black_tree_node_get_right(
                parent, &right));
        if (node != right) {
            *out = parent;
            return true;
        }
        node = parent;
    }
    rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
    return false;
}

bool rock_red_black_tree_prev(const struct rock_red_black_tree_node *node,
                              struct rock_red_black_tree_node **const out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *left;
    seagrass_required_true(rock_red_black_tree_node_get_left(
            node, &left));
    if (left) {
        rock_red_black_tree_maximum(left, out);
        return true;
    }
    struct rock_red_black_tree_node *parent;
    while (true) {
        seagrass_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        seagrass_required_true(rock_red_black_tree_node_get_left(
                parent, &left));
        if (node != left) {
            *out = parent;
            return true;
        }
        node = parent;
    }
    rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
    return false;
}

bool rock_red_black_tree_rotate_N(struct rock_red_black_tree_node *const Y,
                                  const bool is_N_left) {
    seagrass_required(Y);
    bool (*const get_P)(const struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node **)
    = rock_red_black_tree_node_get_parent;
    bool (*const set_P)(struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node *)
    = rock_red_black_tree_node_set_parent;
    struct rock_red_black_tree_node *X;
    seagrass_required_true(get_P(Y, &X));
    if (!X) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    bool (*const get_N)(const struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node **)
    = is_N_left
      ? rock_red_black_tree_node_get_left
      : rock_red_black_tree_node_get_right;
    bool (*const set_N)(struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node *)
    = is_N_left
      ? rock_red_black_tree_node_set_left
      : rock_red_black_tree_node_set_right;
    bool (*const get_O)(const struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node **)
    = is_N_left
      ? rock_red_black_tree_node_get_right
      : rock_red_black_tree_node_get_left;
    bool (*const set_O)(struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node *)
    = is_N_left
      ? rock_red_black_tree_node_set_right
      : rock_red_black_tree_node_set_left;
    struct rock_red_black_tree_node *y;
    seagrass_required_true(get_O(X, &y));
    if (Y != y) {
        rock_error = is_N_left
                     ? ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
                     : ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X;
        return false;
    }
    struct rock_red_black_tree_node *b;
    seagrass_required_true(get_N(Y, &b));
    if (b) {
        seagrass_required_true(set_P(b, X));
    }
    seagrass_required_true(set_O(X, b));
    struct rock_red_black_tree_node *p;
    seagrass_required_true(get_P(X, &p));
    if (!p) {
        seagrass_required_true(set_P(Y, NULL));
    } else {
        struct rock_red_black_tree_node *x;
        seagrass_required_true(get_N(p, &x));
        if (X == x) {
            seagrass_required_true(set_N(p, Y));
        } else {
            seagrass_required_true(set_O(p, Y));
        }
        seagrass_required_true(set_P(Y, p));
    }
    seagrass_required_true(set_N(Y, X) && set_P(X, Y));
    return true;
}

bool rock_red_black_tree_rotate_left(
        struct rock_red_black_tree_node *const Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N(Y, true);
}

bool rock_red_black_tree_rotate_right(
        struct rock_red_black_tree_node *const Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N(Y, false);
}

bool rock_red_black_tree_rotate_N_O(struct rock_red_black_tree_node *const Y,
                                    const bool is_N_left) {
    seagrass_required(Y);
    bool (*const get_P)(const struct rock_red_black_tree_node *,
                        struct rock_red_black_tree_node **)
    = rock_red_black_tree_node_get_parent;
    struct rock_red_black_tree_node *X;
    seagrass_required_true(get_P(Y, &X));
    if (!X) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    struct rock_red_black_tree_node *Z;
    seagrass_required_true(get_P(X, &Z));
    if (!Z) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT;
        return false;
    }
    bool (*const rotate_N)(struct rock_red_black_tree_node *)
    = is_N_left
      ? rock_red_black_tree_rotate_left
      : rock_red_black_tree_rotate_right;
    bool (*const rotate_O)(struct rock_red_black_tree_node *)
    = is_N_left
      ? rock_red_black_tree_rotate_right
      : rock_red_black_tree_rotate_left;
    if (!rotate_N(Y)) {
        return false;
    }
    if (!rotate_O(Y)) {
        switch (rock_error) {
            /* disambiguate the error to be returned */
            case ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT: {
                rock_error = ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT;
                break;
            }
            case ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X: {
                rock_error =
                        ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_RIGHT_CHILD_OF_Z;
                break;
            }
            case ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X: {
                rock_error =
                        ROCK_RED_BLACK_TREE_ERROR_X_IS_NOT_LEFT_CHILD_OF_Z;
                break;
            }
            default: {
                /* no op */
            }
        }
        return false;
    }
    return true;
}

bool
rock_red_black_tree_rotate_left_right(
        struct rock_red_black_tree_node *const Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N_O(Y, true);
}

bool
rock_red_black_tree_rotate_right_left(
        struct rock_red_black_tree_node *const Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N_O(Y, false);
}

bool rock_red_black_tree_init(
        struct rock_red_black_tree *const object,
        int (*compare)(const struct rock_red_black_tree_node *first,
                       const struct rock_red_black_tree_node *second)) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!compare) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL;
        return false;
    }
    *object = (struct rock_red_black_tree) {0};
    object->compare = compare;
    return true;
}

bool rock_red_black_tree_invalidate(
        struct rock_red_black_tree *const object,
        void (*const on_destroy)(struct rock_red_black_tree_node *node)) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    for (struct rock_red_black_tree_node *node = object->root; node;) {
        struct rock_red_black_tree_node *next;
        seagrass_required_true(rock_red_black_tree_node_get_left(
                node, &next));
        if (next) {
            seagrass_required_true(rock_red_black_tree_node_set_left(
                    node, NULL));
            node = next;
            continue;
        }
        seagrass_required_true(rock_red_black_tree_node_get_right(
                node, &next));
        if (next) {
            seagrass_required_true(rock_red_black_tree_node_set_right(
                    node, NULL));
            node = next;
            continue;
        }
        seagrass_required_true(rock_red_black_tree_node_get_parent(
                node, &next));
        if (on_destroy) {
            on_destroy(node);
        }
        node = next;
    }
    *object = (struct rock_red_black_tree) {0};
    return true;
}

bool rock_red_black_tree_count(const struct rock_red_black_tree *const object,
                               uintmax_t *const out) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = object->count;
    return true;
}

bool rock_red_black_tree_first(const struct rock_red_black_tree *const object,
                               struct rock_red_black_tree_node **const out) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->root) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY;
        return false;
    }
    rock_red_black_tree_minimum(object->root, out);
    return true;
}

bool rock_red_black_tree_last(const struct rock_red_black_tree *const object,
                              struct rock_red_black_tree_node **const out) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!object->root) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_TREE_IS_EMPTY;
        return false;
    }
    rock_red_black_tree_maximum(object->root, out);
    return true;
}

void rock_red_black_tree_minimum(const struct rock_red_black_tree_node *root,
                                 struct rock_red_black_tree_node **const out) {
    seagrass_required(root);
    seagrass_required(out);
    struct rock_red_black_tree_node *left;
    while (true) {
        seagrass_required_true(rock_red_black_tree_node_get_left(
                root, &left));
        if (!left) {
            *out = (struct rock_red_black_tree_node *) root;
            return;
        }
        root = left;
    }
}

void rock_red_black_tree_maximum(const struct rock_red_black_tree_node *root,
                                 struct rock_red_black_tree_node **const out) {
    seagrass_required(root);
    seagrass_required(out);
    struct rock_red_black_tree_node *right;
    while (true) {
        seagrass_required_true(rock_red_black_tree_node_get_right(
                root, &right));
        if (!right) {
            *out = (struct rock_red_black_tree_node *) root;
            return;
        }
        root = right;
    }
}

bool rock_red_black_tree_find(
        const struct rock_red_black_tree *const object,
        const struct rock_red_black_tree_node *const root,
        const struct rock_red_black_tree_node *const needle,
        struct rock_red_black_tree_node **const out) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!needle) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NEEDLE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = NULL;
    for (struct rock_red_black_tree_node *node
            = (struct rock_red_black_tree_node *)
                    (root ? root : object->root); node;) {
        *out = node;
        const int result = object->compare(needle, node);
        if (!result) {
            return true;
        } else if (result < 0) {
            seagrass_required_true(rock_red_black_tree_node_get_left(
                    node, &node));
        } else {
            seagrass_required_true(rock_red_black_tree_node_get_right(
                    node, &node));
        }
    }
    rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_NOT_FOUND;
    return false;
}

bool rock_red_black_tree_insert(struct rock_red_black_tree *const object,
                                struct rock_red_black_tree_node *parent,
                                struct rock_red_black_tree_node *child) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!child) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!parent && object->root) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_INSERTION_POINT_IS_NULL;
        return false;
    }
    /* required that child be RED */
    bool color;
    seagrass_required_true(rock_red_black_tree_node_set_color(
            child, ROCK_RED_BLACK_TREE_COLOR_RED));
    /* insert */
    if (!parent) {
        parent = object->root;
    }
    if (!parent) {
        object->root = child;
    } else {
        const int result = object->compare(child, parent);
        if (!result) { /* duplicates are not allowed */
            rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_ALREADY_EXISTS;
            return false;
        }
        const bool is_N_left = result < 0;
        bool (*const get_N)(const struct rock_red_black_tree_node *,
                            struct rock_red_black_tree_node **)
        = is_N_left
          ? rock_red_black_tree_node_get_left
          : rock_red_black_tree_node_get_right;
        bool (*const set_N)(struct rock_red_black_tree_node *,
                            struct rock_red_black_tree_node *)
        = is_N_left
          ? rock_red_black_tree_node_set_left
          : rock_red_black_tree_node_set_right;
        struct rock_red_black_tree_node *node;
        seagrass_required_true(get_N(parent, &node));
        if (node) { /* replace existing node in parent */
            seagrass_required_true(rock_red_black_tree_node_get_color(
                    node, &color));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    node, ROCK_RED_BLACK_TREE_COLOR_RED));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    child, color));
            seagrass_required_true(rock_red_black_tree_node_set_parent(
                    child, parent));
            seagrass_required_true(set_N(parent, child));
            parent = child;
            child = node;
        }
        seagrass_required_true(rock_red_black_tree_node_set_parent(
                child, parent));
        seagrass_required_true(set_N(parent, child));
    }
    seagrass_required_true(seagrass_uintmax_t_add(
            1, object->count, &object->count));
    /* repair */
    while (true) {
        seagrass_required_true(rock_red_black_tree_node_get_color(
                child, &color));
        seagrass_required_true(ROCK_RED_BLACK_TREE_COLOR_RED == color);
        if (!parent) {
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            return true;
        }
        seagrass_required_true(rock_red_black_tree_node_get_color(
                parent, &color));
        /* case 1: parent is BLACK */
        if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            return true;
        }
        struct rock_red_black_tree_node *grandparent, *sibling;
        seagrass_required_true(rock_red_black_tree_node_get_parent(
                parent, &grandparent));
        seagrass_required_true(rock_red_black_tree_node_get_left(
                grandparent, &sibling));
        if (sibling == parent) {
            seagrass_required_true(rock_red_black_tree_node_get_right(
                    grandparent, &sibling));
        }
        seagrass_required_true(rock_red_black_tree_node_get_color(
                sibling, &color));
        /* Legend: C - child            B - BLACK
         *         P - parent           R - RED
         *         G - grandparent
         *         S - sibling
         */
        /* case 2a: parent's sibling is RED */
        if (ROCK_RED_BLACK_TREE_COLOR_RED == color) {
            /*         G(B)                      G(R)
             *         /  \                      /  \
             *      P(R)  S(R)       =>       P(B)  S(B)
             *      /                         /
             *   C(R)                       C(R)
             * --------------------- or ---------------------
             *         G(B)                      G(R)
             *         /  \                      /  \
             *      S(R)  P(R)       =>       S(B)  P(B)
             *              \                         \
             *              C(R)                      C(R)
             */
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    grandparent, ROCK_RED_BLACK_TREE_COLOR_RED));
            /* Changing grandparent to RED may have introduced two consecutive
             * REDs - grandparent and possibly great-grandparent. We will
             * iterate to correct this violation.
             */
            child = grandparent;
            seagrass_required_true(rock_red_black_tree_node_get_parent(
                    child, &parent));
            continue;
        }
        /* case 2b: parent's sibling is BLACK */
        const bool is_parent_left = rock_red_black_tree_node_is_on_left(
                grandparent, parent);
        const bool is_child_left = rock_red_black_tree_node_is_on_left(
                parent, child);
        switch (is_parent_left << 1 | is_child_left) {
            /*         G(B)                      P(B)
             *         /  \                      /  \
             *     S(B)    P(R)      =>       G(R)   C(R)
             *                \                /
             *                C(R)           S(B)
             * --------------------- or ---------------------
             *         G(B)                      P(B)
             *         /  \                      /  \
             *     P(R)    S(B)      =>      C(R)    G(R)
             *     /                                    \
             *  C(R)                                    S(B)
             */
            case 0:
            case 3: {
                seagrass_required_true(rock_red_black_tree_rotate_N(
                        parent, !is_parent_left));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        grandparent, ROCK_RED_BLACK_TREE_COLOR_RED));
                if (object->root == grandparent) {
                    object->root = parent;
                }
                return true;
            }
                /*         G(B)                      C(B)
                 *         /  \                      /  \
                 *     S(B)    P(R)      =>       G(R)   P(R)
                 *             /                   /
                 *           C(R)                S(B)
                 * --------------------- or ---------------------
                 *         G(B)                      C(B)
                 *         /  \                      /  \
                 *     P(R)    S(B)      =>      P(R)    G(R)
                 *        \                                 \
                 *        C(R)                              S(B)
                 */
            case 1:
            case 2: {
                seagrass_required_true(rock_red_black_tree_rotate_N_O(
                        child, !is_child_left));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        grandparent, ROCK_RED_BLACK_TREE_COLOR_RED));
                if (object->root == grandparent) {
                    object->root = child;
                }
                return true;
            }
        }
    }
}

bool rock_red_black_tree_remove(struct rock_red_black_tree *const object,
                                struct rock_red_black_tree_node *const node) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    /* remove */
    bool color;
    struct rock_red_black_tree_node *left, *right, *parent, *double_black = NULL;
    while (!double_black) {
        seagrass_required_true(rock_red_black_tree_node_get_color(
                node, &color));
        seagrass_required_true(rock_red_black_tree_node_get_left(
                node, &left));
        seagrass_required_true(rock_red_black_tree_node_get_right(
                node, &right));
        seagrass_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        switch ((NULL == left) << 1 | (NULL == right)) {
            case 3: { /* leaf node */
                if (parent) {
                    bool (*const set_N)(struct rock_red_black_tree_node *,
                                        struct rock_red_black_tree_node *)
                    = rock_red_black_tree_node_is_on_left(parent, node)
                      ? rock_red_black_tree_node_set_left
                      : rock_red_black_tree_node_set_right;
                    seagrass_required_true(set_N(parent, NULL));
                }
                seagrass_required_true(seagrass_uintmax_t_subtract(
                        object->count, 1, &object->count));
                if (node == object->root) {
                    object->root = NULL;
                } else if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
                    double_black = node;
                    continue;
                }
                return true;
            }
            case 2:
            case 1: { /* node with single child */
                void *child = left ? left : right;
                /* 'child' to replace 'node' */
                bool color_;
                seagrass_required_true(rock_red_black_tree_node_get_color(
                        child, &color_));
                if (parent) {
                    bool (*const set_N)(struct rock_red_black_tree_node *,
                                        struct rock_red_black_tree_node *)
                    = rock_red_black_tree_node_is_on_left(parent, node)
                      ? rock_red_black_tree_node_set_left
                      : rock_red_black_tree_node_set_right;
                    seagrass_required_true(set_N(parent, child));
                }
                seagrass_required_true(rock_red_black_tree_node_set_parent(
                        child, parent));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                if (node == object->root) {
                    object->root = child;
                }
                seagrass_required_true(seagrass_uintmax_t_subtract(
                        object->count, 1, &object->count));
                if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                    && ROCK_RED_BLACK_TREE_COLOR_BLACK == color_) {
                    double_black = child;
                    continue;
                }
                return true;
            }
            case 0: { /* node with two children */
                struct rock_red_black_tree_node *next;
                seagrass_required_true(rock_red_black_tree_next(
                        node, &next));
                /* swap 'next' with 'node' */
                bool color_;
                seagrass_required_true(rock_red_black_tree_node_get_color(
                        next, &color_));
                struct rock_red_black_tree_node *parent_, *left_, *right_;
                seagrass_required_true(rock_red_black_tree_node_get_parent(
                        next, &parent_));
                seagrass_required_true(rock_red_black_tree_node_get_left(
                        next, &left_));
                seagrass_required_true(rock_red_black_tree_node_get_right(
                        next, &right_));

                seagrass_required_true(rock_red_black_tree_node_set_parent(
                        node, node == parent_ ? next : parent_));
                seagrass_required_true(rock_red_black_tree_node_set_parent(
                        next, next == parent ? node : parent));
                seagrass_required_true(rock_red_black_tree_node_set_left(
                        node, node == left_ ? next : left_));
                seagrass_required_true(rock_red_black_tree_node_set_left(
                        next, next == left ? node : left));
                seagrass_required_true(rock_red_black_tree_node_set_right(
                        node, node == right_ ? next : right_));
                seagrass_required_true(rock_red_black_tree_node_set_right(
                        next, next == right ? node : right));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        node, color_));
                seagrass_required_true(rock_red_black_tree_node_set_color(
                        next, color));

                if (parent_ && node != parent_) {
                    bool (*const set_N)(struct rock_red_black_tree_node *,
                                        struct rock_red_black_tree_node *)
                    = rock_red_black_tree_node_is_on_left(parent_, next)
                      ? rock_red_black_tree_node_set_left
                      : rock_red_black_tree_node_set_right;
                    seagrass_required_true(set_N(parent_, node));
                }
                if (parent && next != parent) {
                    bool (*const set_N)(struct rock_red_black_tree_node *,
                                        struct rock_red_black_tree_node *)
                    = rock_red_black_tree_node_is_on_left(parent, node)
                      ? rock_red_black_tree_node_set_left
                      : rock_red_black_tree_node_set_right;
                    seagrass_required_true(set_N(parent, next));
                }
                if (left_ && node != left_) {
                    seagrass_required_true(rock_red_black_tree_node_set_parent(
                            left_, node));
                }
                if (left && next != left) {
                    seagrass_required_true(rock_red_black_tree_node_set_parent(
                            left, next));
                }
                if (right_ && node != right_) {
                    seagrass_required_true(rock_red_black_tree_node_set_parent(
                            right_, node));
                }
                if (right && next != right) {
                    seagrass_required_true(rock_red_black_tree_node_set_parent(
                            right, next));
                }
                if (node == object->root) {
                    object->root = next;
                }
                continue;
            }
        }
    }
    /* repair */
    /* Legend: C - child            B - BLACK
     *         P - parent           R - RED
     *         N - node             ? - ANY COLOR
     *         S - sibling         (B)- DOUBLE BLACK
     *                             ...- SUBTREE
     */
    while (true) {
        /* case 1: root is double black
         *          N((B))
         *     drop double black
         *           N(B)
         * [done]
         */
        if (double_black == object->root) {
            break;
        }
        struct rock_red_black_tree_node *parent_ = parent;
        bool is_left; /* double black on left or right */
        struct rock_red_black_tree_node *sibling;
        if (parent_->left == double_black || !parent_->left) {
            sibling = parent_->right;
            is_left = true;
        } else {
            sibling = parent_->left;
            is_left = false;
        }
        seagrass_required_true(rock_red_black_tree_node_get_left(
                sibling, &left));
        seagrass_required_true(rock_red_black_tree_node_get_right(
                sibling, &right));
        /* case 2: if sibling is RED
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(R)          |          S(R)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                    rotate S through P
         *                  switch P and S colors
         *           S(B)              |            S(B)
         *          /    \             |           /    \
         *        P(R)   C(B)          |         C(B)   P(R)
         *       /   \   /  \          |        /   \   /  \
         *   N((B)) C(B)... ...        |       ... ...C(B)  N((B))
         *   /  \   /  \               |              /  \   /  \
         * ... ......  ...             |            ... ......  ...
         */
        if (rock_red_black_tree_node_get_color(parent, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(sibling, &color)
            && ROCK_RED_BLACK_TREE_COLOR_RED == color
            && rock_red_black_tree_node_get_color(left, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(right, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            seagrass_required_true(rock_red_black_tree_rotate_N(
                    sibling, is_left));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_RED));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            if (parent == object->root) {
                object->root = sibling;
            }
            continue; /* sibling has changed */
        }
        /* case 3: if sibling is BLACK
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                        color S RED
         *                parent is now double black
         *           P((B))            |            P((B))
         *          /    \             |            /    \
         *        N(B)   S(R)          |          S(R)    N(B)
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         */
        if (rock_red_black_tree_node_get_color(parent, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(sibling, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(left, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(right, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            double_black = parent;
            seagrass_required_true(rock_red_black_tree_node_get_parent(
                    parent, &parent));
            continue; /* go to case 1 as parent is now double black */
        }
        /* case 4: If parent is RED sibling and children are BLACK
         *           P(R)              |             P(R)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                   color P BLACK and S RED
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *        N(B)   S(R)          |          S(R)    N(B)
         *        / \     / \          |          / \      / \
         *      ... ... C(B) C(B)      |       C(B) C(B) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *  [done]
         */
        if (rock_red_black_tree_node_get_color(parent, &color)
            && ROCK_RED_BLACK_TREE_COLOR_RED == color
            && rock_red_black_tree_node_get_color(sibling, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(left, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(right, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            break;
        }
        /* closest C from N */
        struct rock_red_black_tree_node *closest = is_left ? left : right;
        /* farthest C from N */
        struct rock_red_black_tree_node *farthest = is_left ? right : left;
        /* case 5: Sibling is BLACK and closest child is RED while farthest
         * child is BLACK.
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... C(R) C(B)      |       C(B) C(R) ... ...
         *              / \  / \       |       / \  / \
         *            ... .... ...     |     ... .... ...
         *                   rotate C(R) through S
         *              change S to RED and C(R) to BLACK
         *           P(B)              |             P(B)
         *          /    \             |            /    \
         *      N((B))   C(B)          |          C(B)   N((B))
         *        / \     / \          |          / \      / \
         *      ... ... ...  S(R)      |       S(R) ...  ... ...
         *                   / \       |       / \
         *                 ... C(B)    |    C(B) ...
         *                     / \     |    / \
         *                   ... ...   |  ... ...
         */
        if (rock_red_black_tree_node_get_color(parent, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(sibling, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(closest, &color)
            && ROCK_RED_BLACK_TREE_COLOR_RED == color
            && rock_red_black_tree_node_get_color(farthest, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            seagrass_required_true(rock_red_black_tree_rotate_N(
                    closest, !is_left));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    closest, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            continue; /* sibling has changed */
        }
        /* case 6: Sibling is BLACK and farthest child is RED.
         *           P(?)              |             P(?)
         *          /    \             |            /    \
         *      N((B))   S(B)          |          S(B)   N((B))
         *        / \     / \          |          / \     / \
         *      ... ... ...  C(R)      |       C(R) ... ... ...
         *                  / \        |       / \
         *                ... ...      |     ... ...
         *                     rotate S through P
         *                      set S to P color
         *                    set P and C to BLACK
         *           S(?)              |             S(?)
         *          /    \             |            /    \
         *        P(B)   C(B)          |          P(B)   C(B)
         *        / \     / \          |          / \     / \
         *     N(B) ... ... ...        |        ... ... ... N(B)
         *     / \                     |                     / \
         *   ... ...                   |                   ... ...
         *  [done]
         */
        if (rock_red_black_tree_node_get_color(sibling, &color)
            && ROCK_RED_BLACK_TREE_COLOR_BLACK == color
            && rock_red_black_tree_node_get_color(farthest, &color)
            && ROCK_RED_BLACK_TREE_COLOR_RED == color) {
            seagrass_required_true(rock_red_black_tree_rotate_N(
                    sibling, is_left));
            seagrass_required_true(rock_red_black_tree_node_get_color(
                    parent, &color));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    sibling, color));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            seagrass_required_true(rock_red_black_tree_node_set_color(
                    farthest, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            if (parent == object->root) {
                object->root = sibling;
            }
            break;
        }
    }
    return true;
}
