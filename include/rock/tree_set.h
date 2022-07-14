#ifndef _ROCK_TREE_SET_H_
#define _ROCK_TREE_SET_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "red_black_tree.h"

#define ROCK_TREE_SET_ERROR_OBJECT_IS_NULL                      1
#define ROCK_TREE_SET_ERROR_COMPARE_IS_NULL                     2
#define ROCK_TREE_SET_ERROR_OUT_IS_NULL                         3
#define ROCK_TREE_SET_ERROR_ITEM_IS_NULL                        4
#define ROCK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS                 5
#define ROCK_TREE_SET_ERROR_ITEM_NOT_FOUND                      6
#define ROCK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED            7
#define ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY                   8
#define ROCK_TREE_SET_ERROR_END_OF_SEQUENCE                     9

struct rock_tree_set {
    struct rock_red_black_tree tree;

    int (*compare)(const void *, const void *);
};

/**
 * @brief Initialise tree set.
 * @param [in] object instance to be initialised.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first item</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * item</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_COMPARE_IS_NULL if compare is <i>NULL</i>.
 */
bool rock_tree_set_init(struct rock_tree_set *object,
                        int (*compare)(const void *first,
                                       const void *second));

/**
 * @brief Invalidate tree set.
 * <p>All the items contained within the tree set will have the given <i>on
 * destroy</i> callback invoked upon itself. The actual <u>tree set instance
 * is not deallocated</u> since it may have been embedded in a larger structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the item is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_tree_set_invalidate(struct rock_tree_set *object,
                              void (*on_destroy)(void *));

/**
 * @brief Retrieve the count of items.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_tree_set_count(struct rock_tree_set *object, size_t *out);

/**
 * @brief Add item to the tree set.
 * @param [in] object tree set instance.
 * @param [in] item to be added.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS if item is already present
 * in the tree set.
 * @throws ROCK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED if there was not
 * enough memory to add item to the tree set.
 */
bool rock_tree_set_add(struct rock_tree_set *object, void *item);

/**
 * @brief Remove item from the tree set.
 * @param [in] object tree set instance.
 * @param [in] item to be removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_ITEM_NOT_FOUND if item is not in the tree set
 * instance.
 */
bool rock_tree_set_remove(struct rock_tree_set *object, void *item);

/**
 * @brief Check if tree set contains the given item.
 * @param [in] object tree set instance.
 * @param [in] item to check if item is present.
 * @param [out] out receive true if item is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_tree_set_contains(struct rock_tree_set *object, void *item,
                            bool *out);

/**
 * @brief First item of the tree set.
 * @param [in] object tree set instance.
 * @param [out] out receive the first item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY if tree set is empty.
 */
bool rock_tree_set_first(struct rock_tree_set *object, void **out);

/**
 * @brief Last item of the tree set.
 * @param [in] object tree set instance.
 * @param [out] out receive the last item in the tree set.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_TREE_SET_IS_EMPTY if tree set is empty.
 */
bool rock_tree_set_last(struct rock_tree_set *object, void **out);

/**
 * @brief Retrieve next item.
 * @param [in] item current item.
 * @param [out] out receive the next item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no next item.
 */
bool rock_tree_set_next(void *item, void **out);

/**
 * @brief Retrieve the previous item.
 * @param [in] item current item in the tree set.
 * @param [out] out receive the previous item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no next item.
 */
bool rock_tree_set_prev(void *item, void **out);

#endif /* _ROCK_TREE_SET_H_ */
