#ifndef _ROCK_RED_BLACK_TREE_SET_H_
#define _ROCK_RED_BLACK_TREE_SET_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "red_black_tree.h"

#define ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL            1
#define ROCK_RED_BLACK_TREE_SET_ERROR_COMPARE_IS_NULL           2
#define ROCK_RED_BLACK_TREE_SET_ERROR_SIZE_IS_ZERO              3
#define ROCK_RED_BLACK_TREE_SET_ERROR_SIZE_IS_TOO_LARGE         4
#define ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL               5
#define ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL              6
#define ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS       7
#define ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND            8
#define ROCK_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED  9
#define ROCK_RED_BLACK_TREE_SET_ERROR_SET_IS_EMPTY              10
#define ROCK_RED_BLACK_TREE_SET_ERROR_END_OF_SEQUENCE           11

struct rock_red_black_tree_set {
    struct rock_red_black_tree tree;
    size_t size;
    size_t alloc;

    int (*compare)(const void *, const void *);
};

/**
 * @brief Initialize red black tree set.
 * @param [in] object instance to be initialized.
 * @param [in] size of items to be contained within the set.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first item</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * item</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_COMPARE_IS_NULL if compare is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_SIZE_IS_ZERO if size is zero.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_SIZE_IS_TOO_LARGE if size is too large.
 */
bool rock_red_black_tree_set_init(struct rock_red_black_tree_set *object,
                                  size_t size,
                                  int (*compare)(const void *first,
                                                 const void *second));

/**
 * @brief Invalidate red black tree set.
 * <p>All the items contained within the tree set will have the given <i>on
 * destroy</i> callback invoked upon itself. The actual <u>tree set instance
 * is not deallocated</u> since it may have been embedded in a larger
 * structure.</p>
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the item is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 */
bool rock_red_black_tree_set_invalidate(struct rock_red_black_tree_set *object,
                                        void (*on_destroy)(void *item));

/**
 * @brief Retrieve the size of an item.
 * @param [in] object tree set instance.
 * @param [in] out receive the size of an item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_red_black_tree_set_size(const struct rock_red_black_tree_set *object,
                                  size_t *out);

/**
 * @brief Retrieve the count of items.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_red_black_tree_set_count(const struct rock_red_black_tree_set *object,
                                   uintmax_t *out);

/**
 * @brief Add item to the tree set.
 * @param [in] object tree set instance.
 * @param [in] item to be added.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_ALREADY_EXISTS if item is already
 * present in the tree set.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED if there was
 * not enough memory to add item to the tree set.
 */
bool rock_red_black_tree_set_add(struct rock_red_black_tree_set *object,
                                 const void *item);

/**
 * @brief Remove item from the tree set.
 * @param [in] object tree set instance.
 * @param [in] item to be removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if item is not in the
 * tree set instance.
 */
bool rock_red_black_tree_set_remove(struct rock_red_black_tree_set *object,
                                    const void *item);

/**
 * @brief Check if tree set contains the given item.
 * @param [in] object tree set instance.
 * @param [in] item to check if item is present.
 * @param [out] out receive true if item is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool
rock_red_black_tree_set_contains(const struct rock_red_black_tree_set *object,
                                 const void *item,
                                 bool *out);

/**
 * @brief Retrieve item.
 * @param [in] object tree set instance.
 * @param [in] item to find.
 * @param [out] out receive the <u>address of</u> item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if item is not in the
 * tree set instance.
 */
bool rock_red_black_tree_set_get(const struct rock_red_black_tree_set *object,
                                 const void *item,
                                 const void **out);

/**
 * @brief Retrieve item or the next higher value.
 * @param [in] object tree set instance.
 * @param [in] item to find or its next higher.
 * @param [out] out receive the <u>address of</u> item or the next higher value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if item nor a
 * higher value is in the tree set instance.
 */
bool
rock_red_black_tree_set_ceiling(const struct rock_red_black_tree_set *object,
                                const void *item,
                                const void **out);

/**
 * @brief Retrieve item or the next lower value.
 * @param [in] object tree set instance.
 * @param [in] item to find or its next lower.
 * @param [out] out receive the <u>address of</u> item or the next lower value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if item nor a lower
 * value is in the tree set instance.
 */
bool rock_red_black_tree_set_floor(const struct rock_red_black_tree_set *object,
                                   const void *item,
                                   const void **out);

/**
 * @brief Retrieve next higher value for item.
 * @param [in] object tree set instance.
 * @param [in] item whose next higher value we are trying to find.
 * @param [out] out receive the <u>address of</u> the next higher value for
 * item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if no greater value
 * for item is in the tree set instance.
 */
bool
rock_red_black_tree_set_higher(const struct rock_red_black_tree_set *object,
                               const void *item,
                               const void **out);

/**
 * @brief Retrieve the next lower value for item.
 * @param [in] object tree set instance.
 * @param [in] item whose next lower value we are trying to find.
 * @param [out] out receive the <u>address of</u> the next lower value for item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND if no lower value for
 * item is in the tree set instance.
 */
bool rock_red_black_tree_set_lower(const struct rock_red_black_tree_set *object,
                                   const void *item,
                                   const void **out);

/**
 * @brief First item of the tree set.
 * @param [in] object tree set instance.
 * @param [out] out receive the <u>address of</u> the first item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_SET_IS_EMPTY if tree set is empty.
 */
bool rock_red_black_tree_set_first(const struct rock_red_black_tree_set *object,
                                   const void **out);

/**
 * @brief Last item of the tree set.
 * @param [in] object tree set instance.
 * @param [out] out receive the <u>address of</u> the last item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if object is
 * <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_SET_IS_EMPTY if tree set is empty.
 */
bool rock_red_black_tree_set_last(const struct rock_red_black_tree_set *object,
                                  const void **out);

/**
 * @brief Retrieve next item.
 * @param [in] item current <u>address of</u> item.
 * @param [out] out receive the <u>address of</u> the next item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no next
 * item.
 */
bool rock_red_black_tree_set_next(const void *item, const void **out);

/**
 * @brief Retrieve the previous item.
 * @param [in] item current <u>address of</u> item.
 * @param [out] out receive the <u>address of</u> the previous item.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_RED_BLACK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no
 * previous item.
 */
bool rock_red_black_tree_set_prev(const void *item, const void **out);

#endif /* _ROCK_RED_BLACK_TREE_SET_H_ */
