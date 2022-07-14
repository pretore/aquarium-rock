#include <stdlib.h>
#include <rock.h>

#include "private/red_black_tree.h"
#include "test/cmocka.h"

struct rock_red_black_tree_node *rock_red_black_tree_node_from(void *node) {
    rock_required(node);
    const char *node_ = node;
    return (struct rock_red_black_tree_node *)
            (node_ - sizeof(struct rock_red_black_tree_node));
}

void *rock_red_black_tree_node_to(struct rock_red_black_tree_node *node) {
    rock_required(node);
    const char *node_ = (const char *) node;
    return (void *) (node_ + sizeof(struct rock_red_black_tree_node));
}

bool rock_red_black_tree_node(const size_t size, void **out) {
    if (rock_red_black_tree_node_alloc(size, out)) {
        if (rock_red_black_tree_node_init(*out)) {
            return true;
        }
        free(*out);
        *out = NULL;
    }
    return false;
}

bool rock_red_black_tree_node_alloc(const size_t size, void **out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    size_t alloc_size;
    struct rock_red_black_tree_node *node;
    if (!rock_add_size_t(size, sizeof(*node), &alloc_size)) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    node = calloc(1, alloc_size);
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    *out = rock_red_black_tree_node_to(node);
    return true;
}

bool rock_red_black_tree_node_init(void *node) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->parent = (void *) ROCK_RED_BLACK_TREE_COLOR_RED;
    node_->left = NULL;
    node_->right = NULL;
    return true;
}

void rock_red_black_tree_node_destroy(void *node) {
    if (node) {
        free(rock_red_black_tree_node_from(node));
    }
}

bool rock_red_black_tree_node_get_parent(void *node, void **out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    *out = (void *) ((size_t) node_->parent & ~1);
    return true;
}

bool rock_red_black_tree_node_set_parent(void *node, void *parent) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    bool color;
    rock_required_true(rock_red_black_tree_node_get_color(
            node, &color));
    const size_t value = (size_t) parent | color;
    node_->parent = (struct rock_red_black_tree_node *) value;
    return true;
}

bool rock_red_black_tree_node_get_color(void *node, bool *out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes are black */
        *out = ROCK_RED_BLACK_TREE_COLOR_BLACK;
        return true;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    *out = (size_t) node_->parent & 1;
    return true;
}

bool rock_red_black_tree_node_set_color(void *node, const bool color) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    size_t value = (~1 & (size_t) node_->parent) | (size_t) color;
    node_->parent = (struct rock_red_black_tree_node *) value;
    return true;
}

bool rock_red_black_tree_node_get_left(void *node, void **out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    *out = node_->left;
    return true;
}

bool rock_red_black_tree_node_set_left(void *node, void *left) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->left = left;
    return true;
}

bool rock_red_black_tree_node_get_right(void *node, void **out) {
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!node) { /* NIL nodes have no children */
        rock_error = ROCK_RED_BLACK_TREE_ERROR_END_OF_SEQUENCE;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    *out = node_->right;
    return true;
}

bool rock_red_black_tree_node_set_right(void *node, void *right) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    node_->right = right;
    return true;
}

bool rock_red_black_tree_node_is_on_left(void *node, void *left) {
    struct rock_red_black_tree_node *node_;
    node_ = rock_red_black_tree_node_from(node);
    const bool is_left = node_->left == left;
    if (!is_left) {
        rock_required_true(node_->right == left);
    }
    return is_left;
}

bool rock_red_black_tree_next(void *node, void **out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    void *right;
    rock_required_true(rock_red_black_tree_node_get_right(
            node, &right));
    if (right) {
        rock_red_black_tree_minimum(right, out);
        return true;
    }
    void *parent;
    while (true) {
        rock_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        rock_required_true(rock_red_black_tree_node_get_right(
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

bool rock_red_black_tree_prev(void *node, void **out) {
    if (!node) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_NODE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    void *left;
    rock_required_true(rock_red_black_tree_node_get_left(
            node, &left));
    if (left) {
        rock_red_black_tree_maximum(left, out);
        return true;
    }
    void *parent;
    while (true) {
        rock_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        if (!parent) {
            break;
        }
        rock_required_true(rock_red_black_tree_node_get_left(
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

bool rock_red_black_tree_rotate_N(void *Y, const bool is_N_left) {
    rock_required(Y);
    bool (*const get_P)(void *, void **) = rock_red_black_tree_node_get_parent;
    bool (*const set_P)(void *, void *) = rock_red_black_tree_node_set_parent;
    void *X;
    rock_required_true(get_P(Y, &X));
    if (!X) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    bool (*const get_N)(void *, void **) = is_N_left
                                           ? rock_red_black_tree_node_get_left
                                           : rock_red_black_tree_node_get_right;
    bool (*const set_N)(void *, void *) = is_N_left
                                          ? rock_red_black_tree_node_set_left
                                          : rock_red_black_tree_node_set_right;
    bool (*const get_O)(void *, void **) = is_N_left
                                           ? rock_red_black_tree_node_get_right
                                           : rock_red_black_tree_node_get_left;
    bool (*const set_O)(void *, void *) = is_N_left
                                          ? rock_red_black_tree_node_set_right
                                          : rock_red_black_tree_node_set_left;
    void *y;
    rock_required_true(get_O(X, &y));
    if (Y != y) {
        rock_error = is_N_left
                     ? ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_RIGHT_CHILD_OF_X
                     : ROCK_RED_BLACK_TREE_ERROR_Y_IS_NOT_LEFT_CHILD_OF_X;
        return false;
    }
    void *b;
    rock_required_true(get_N(Y, &b));
    if (b) {
        rock_required_true(set_P(b, X));
    }
    rock_required_true(set_O(X, b));
    void *p;
    rock_required_true(get_P(X, &p));
    if (!p) {
        rock_required_true(set_P(Y, NULL));
    } else {
        void *x;
        rock_required_true(get_N(p, &x));
        if (X == x) {
            rock_required_true(set_N(p, Y));
        } else {
            rock_required_true(set_O(p, Y));
        }
        rock_required_true(set_P(Y, p));
    }
    rock_required_true(set_N(Y, X) && set_P(X, Y));
    return true;
}

bool rock_red_black_tree_rotate_left(void *Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N(Y, true);
}

bool rock_red_black_tree_rotate_right(void *Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N(Y, false);
}

bool rock_red_black_tree_rotate_N_O(void *Y, const bool is_N_left) {
    rock_required(Y);
    bool (*const get_P)(void *, void **) = rock_red_black_tree_node_get_parent;
    void *X;
    rock_required_true(get_P(Y, &X));
    if (!X) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_HAS_NO_PARENT;
        return false;
    }
    void *Z;
    rock_required_true(get_P(X, &Z));
    if (!Z) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_X_HAS_NO_PARENT;
        return false;
    }
    bool (*const rotate_N)(void *) = is_N_left
                                     ? rock_red_black_tree_rotate_left
                                     : rock_red_black_tree_rotate_right;
    bool (*const rotate_O)(void *) = is_N_left
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

bool rock_red_black_tree_rotate_left_right(void *Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N_O(Y, true);
}

bool rock_red_black_tree_rotate_right_left(void *Y) {
    if (!Y) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_Y_IS_NULL;
        return false;
    }
    return rock_red_black_tree_rotate_N_O(Y, false);
}

bool rock_red_black_tree_init(struct rock_red_black_tree *object,
                              int (*compare)(const void *first,
                                             const void *second)) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!compare) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_COMPARE_IS_NULL;
        return false;
    }
    (*object) = (struct rock_red_black_tree) {0};
    object->compare = compare;
    return true;
}

bool rock_red_black_tree_invalidate(struct rock_red_black_tree *object,
                                    void (*on_destroy)(void *)) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    for (void *node = object->root; node;) {
        void *next;
        rock_required_true(rock_red_black_tree_node_get_left(
                node, &next));
        if (next) {
            rock_required_true(rock_red_black_tree_node_set_left(
                    node, NULL));
            node = next;
            continue;
        }
        rock_required_true(rock_red_black_tree_node_get_right(
                node, &next));
        if (next) {
            rock_required_true(rock_red_black_tree_node_set_right(
                    node, NULL));
            node = next;
            continue;
        }
        rock_required_true(rock_red_black_tree_node_get_parent(
                node, &next));
        if (on_destroy) {
            on_destroy(node);
        }
        rock_red_black_tree_node_destroy(node);
        node = next;
    }
    (*object) = (struct rock_red_black_tree) {0};
    return true;
}

bool rock_red_black_tree_count(struct rock_red_black_tree *object,
                               size_t *out) {
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

bool rock_red_black_tree_first(struct rock_red_black_tree *object,
                               void **out) {
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

bool rock_red_black_tree_last(struct rock_red_black_tree *object,
                              void **out) {
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

void rock_red_black_tree_minimum(void *root, void **out) {
    rock_required(root);
    rock_required(out);
    void *left;
    while (true) {
        rock_required_true(rock_red_black_tree_node_get_left(
                root, &left));
        if (!left) {
            *out = root;
            return;
        }
        root = left;
    }
}

void rock_red_black_tree_maximum(void *root, void **out) {
    rock_required(root);
    rock_required(out);
    void *right;
    while (true) {
        rock_required_true(rock_red_black_tree_node_get_right(
                root, &right));
        if (!right) {
            *out = root;
            return;
        }
        root = right;
    }
}

bool rock_red_black_tree_find(struct rock_red_black_tree *object,
                              const void *root,
                              const void *value,
                              void **out) {
    if (!object) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!value) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_VALUE_IS_NULL;
        return false;
    }
    if (!out) {
        rock_error = ROCK_RED_BLACK_TREE_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = NULL;
    for (void *node = (void *) (root ? root : object->root); node;) {
        *out = node;
        const int result = object->compare(value, node);
        if (!result) {
            return true;
        } else if (result < 0) {
            rock_required_true(rock_red_black_tree_node_get_left(
                    node, &node));
        } else {
            rock_required_true(rock_red_black_tree_node_get_right(
                    node, &node));
        }
    }
    rock_error = ROCK_RED_BLACK_TREE_ERROR_VALUE_NOT_FOUND;
    return false;
}

bool rock_red_black_tree_insert(struct rock_red_black_tree *object,
                                void *parent, void *child) {
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
    rock_required_true(rock_red_black_tree_node_set_color(
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
        bool (*const get_N)(void *, void **) = is_N_left
                                               ? rock_red_black_tree_node_get_left
                                               : rock_red_black_tree_node_get_right;
        bool (*const set_N)(void *, void *) = is_N_left
                                              ? rock_red_black_tree_node_set_left
                                              : rock_red_black_tree_node_set_right;
        void *node;
        rock_required_true(get_N(parent, &node));
        if (node) { /* replace existing node in parent */
            rock_required_true(rock_red_black_tree_node_get_color(
                    node, &color));
            rock_required_true(rock_red_black_tree_node_set_color(
                    node, ROCK_RED_BLACK_TREE_COLOR_RED));
            rock_required_true(rock_red_black_tree_node_set_color(
                    child, color));
            rock_required_true(rock_red_black_tree_node_set_parent(
                    child, parent));
            rock_required_true(set_N(parent, child));
            parent = child;
            child = node;
        }
        rock_required_true(rock_red_black_tree_node_set_parent(
                child, parent));
        rock_required_true(set_N(parent, child));
    }
    object->count += 1;
    /* repair */
    while (true) {
        rock_required_true(rock_red_black_tree_node_get_color(
                child, &color));
        rock_required_true(ROCK_RED_BLACK_TREE_COLOR_RED == color);
        if (!parent) {
            rock_required_true(rock_red_black_tree_node_set_color(
                    child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            return true;
        }
        rock_required_true(rock_red_black_tree_node_get_color(
                parent, &color));
        /* case 1: parent is BLACK */
        if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color) {
            return true;
        }
        void *grandparent, *sibling;
        rock_required_true(rock_red_black_tree_node_get_parent(
                parent, &grandparent));
        rock_required_true(rock_red_black_tree_node_get_left(
                grandparent, &sibling));
        if (sibling == parent) {
            rock_required_true(rock_red_black_tree_node_get_right(
                    grandparent, &sibling));
        }
        rock_required_true(rock_red_black_tree_node_get_color(
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
            rock_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            rock_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            rock_required_true(rock_red_black_tree_node_set_color(
                    grandparent, ROCK_RED_BLACK_TREE_COLOR_RED));
            /* Changing grandparent to RED may have introduced two consecutive
             * REDs - grandparent and possibly great-grandparent. We will
             * iterate to correct this violation.
             */
            child = grandparent;
            rock_required_true(rock_red_black_tree_node_get_parent(
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
                rock_required_true(rock_red_black_tree_rotate_N(
                        parent, !is_parent_left));
                rock_required_true(rock_red_black_tree_node_set_color(
                        parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                rock_required_true(rock_red_black_tree_node_set_color(
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
                rock_required_true(rock_red_black_tree_rotate_N_O(
                        child, !is_child_left));
                rock_required_true(rock_red_black_tree_node_set_color(
                        child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                rock_required_true(rock_red_black_tree_node_set_color(
                        grandparent, ROCK_RED_BLACK_TREE_COLOR_RED));
                if (object->root == grandparent) {
                    object->root = child;
                }
                return true;
            }
        }
    }
}

bool rock_red_black_tree_delete(struct rock_red_black_tree *object,
                                void *node) {
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
    void *left, *right, *parent, *double_black = NULL;
    while (!double_black) {
        rock_required_true(rock_red_black_tree_node_get_color(
                node, &color));
        rock_required_true(rock_red_black_tree_node_get_left(
                node, &left));
        rock_required_true(rock_red_black_tree_node_get_right(
                node, &right));
        rock_required_true(rock_red_black_tree_node_get_parent(
                node, &parent));
        switch ((NULL == left) << 1 | (NULL == right)) {
            case 3: { /* leaf node */
                if (parent) {
                    bool (*const set_N)(void *, void *) =
                    rock_red_black_tree_node_is_on_left(parent, node)
                    ? rock_red_black_tree_node_set_left
                    : rock_red_black_tree_node_set_right;
                    rock_required_true(set_N(parent, NULL));
                }
                rock_red_black_tree_node_destroy(node);
                object->count -= 1;
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
                rock_required_true(rock_red_black_tree_node_get_color(
                        child, &color_));
                if (parent) {
                    bool (*const set_N)(void *, void *) =
                    rock_red_black_tree_node_is_on_left(parent, node)
                    ? rock_red_black_tree_node_set_left
                    : rock_red_black_tree_node_set_right;
                    rock_required_true(set_N(parent, child));
                }
                rock_required_true(rock_red_black_tree_node_set_parent(
                        child, parent));
                rock_required_true(rock_red_black_tree_node_set_color(
                        child, ROCK_RED_BLACK_TREE_COLOR_BLACK));
                if (node == object->root) {
                    object->root = child;
                }
                rock_red_black_tree_node_destroy(node);
                object->count -= 1;
                if (ROCK_RED_BLACK_TREE_COLOR_BLACK == color
                    && ROCK_RED_BLACK_TREE_COLOR_BLACK == color_) {
                    double_black = child;
                    continue;
                }
                return true;
            }
            case 0: { /* node with two children */
                void *next;
                rock_required_true(rock_red_black_tree_next(
                        node, &next));
                /* swap 'next' with 'node' */
                bool color_;
                rock_required_true(rock_red_black_tree_node_get_color(
                        next, &color_));
                void *parent_, *left_, *right_;
                rock_required_true(rock_red_black_tree_node_get_parent(
                        next, &parent_));
                rock_required_true(rock_red_black_tree_node_get_left(
                        next, &left_));
                rock_required_true(rock_red_black_tree_node_get_right(
                        next, &right_));

                rock_required_true(rock_red_black_tree_node_set_parent(
                        node, node == parent_ ? next : parent_));
                rock_required_true(rock_red_black_tree_node_set_parent(
                        next, next == parent ? node : parent));
                rock_required_true(rock_red_black_tree_node_set_left(
                        node, node == left_ ? next : left_));
                rock_required_true(rock_red_black_tree_node_set_left(
                        next, next == left ? node : left));
                rock_required_true(rock_red_black_tree_node_set_right(
                        node, node == right_ ? next : right_));
                rock_required_true(rock_red_black_tree_node_set_right(
                        next, next == right ? node : right));
                rock_required_true(rock_red_black_tree_node_set_color(
                        node, color_));
                rock_required_true(rock_red_black_tree_node_set_color(
                        next, color));

                if (parent_ && node != parent_) {
                    bool (*const set_N)(void *, void *) =
                    rock_red_black_tree_node_is_on_left(parent_, next)
                    ? rock_red_black_tree_node_set_left
                    : rock_red_black_tree_node_set_right;
                    rock_required_true(set_N(parent_, node));
                }
                if (parent && next != parent) {
                    bool (*const set_N)(void *, void *) =
                    rock_red_black_tree_node_is_on_left(parent, node)
                    ? rock_red_black_tree_node_set_left
                    : rock_red_black_tree_node_set_right;
                    rock_required_true(set_N(parent, next));
                }
                if (left_ && node != left_) {
                    rock_required_true(rock_red_black_tree_node_set_parent(
                            left_, node));
                }
                if (left && next != left) {
                    rock_required_true(rock_red_black_tree_node_set_parent(
                            left, next));
                }
                if (right_ && node != right_) {
                    rock_required_true(rock_red_black_tree_node_set_parent(
                            right_, node));
                }
                if (right && next != right) {
                    rock_required_true(rock_red_black_tree_node_set_parent(
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
        struct rock_red_black_tree_node *parent_;
        parent_ = rock_red_black_tree_node_from(parent);
        bool is_left; /* double black on left or right */
        void *sibling;
        if (parent_->left == double_black || !parent_->left) {
            sibling = parent_->right;
            is_left = true;
        } else {
            sibling = parent_->left;
            is_left = false;
        }
        rock_required_true(rock_red_black_tree_node_get_left(
                sibling, &left));
        rock_required_true(rock_red_black_tree_node_get_right(
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
            rock_required_true(rock_red_black_tree_rotate_N(
                    sibling, is_left));
            rock_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_RED));
            rock_required_true(rock_red_black_tree_node_set_color(
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
            rock_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            double_black = parent;
            rock_required_true(rock_red_black_tree_node_get_parent(
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
            rock_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            rock_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            break;
        }
        /* closest C from N */
        void *closest = is_left ? left : right;
        /* farthest C from N */
        void *farthest = is_left ? right : left;
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
            rock_required_true(rock_red_black_tree_rotate_N(
                    closest, !is_left));
            rock_required_true(rock_red_black_tree_node_set_color(
                    sibling, ROCK_RED_BLACK_TREE_COLOR_RED));
            rock_required_true(rock_red_black_tree_node_set_color(
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
            rock_required_true(rock_red_black_tree_rotate_N(
                    sibling, is_left));
            rock_required_true(rock_red_black_tree_node_get_color(
                    parent, &color));
            rock_required_true(rock_red_black_tree_node_set_color(
                    sibling, color));
            rock_required_true(rock_red_black_tree_node_set_color(
                    parent, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            rock_required_true(rock_red_black_tree_node_set_color(
                    farthest, ROCK_RED_BLACK_TREE_COLOR_BLACK));
            if (parent == object->root) {
                object->root = sibling;
            }
            break;
        }
    }
    return true;
}
