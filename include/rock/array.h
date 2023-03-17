#ifndef _ROCK_ARRAY_H_
#define _ROCK_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sea-urchin.h>

#define ROCK_ARRAY_ERROR_OBJECT_IS_NULL \
    SEA_URCHIN_ERROR_OBJECT_IS_NULL
#define ROCK_ARRAY_ERROR_SIZE_IS_ZERO \
    SEA_URCHIN_ERROR_VALUE_IS_ZERO
#define ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED \
    SEA_URCHIN_ERROR_MEMORY_ALLOCATION_FAILED
#define ROCK_ARRAY_ERROR_OUT_IS_NULL \
    SEA_URCHIN_ERROR_OUT_IS_NULL
#define ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS \
    SEA_URCHIN_ERROR_INDEX_IS_OUT_OF_BOUNDS
#define ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY \
    SEA_URCHIN_ERROR_IS_EMPTY
#define ROCK_ARRAY_ERROR_COUNT_IS_ZERO \
    SEA_URCHIN_ERROR_VALUE_IS_ZERO
#define ROCK_ARRAY_ERROR_ITEMS_IS_NULL \
    SEA_URCHIN_ERROR_ITEM_IS_NULL
#define ROCK_ARRAY_ERROR_ITEM_IS_NULL \
    SEA_URCHIN_ERROR_ITEM_IS_NULL
#define ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS \
    SEA_URCHIN_ERROR_ITEM_IS_OUT_OF_BOUNDS
#define ROCK_ARRAY_ERROR_END_OF_SEQUENCE \
    SEA_URCHIN_ERROR_END_OF_SEQUENCE

struct rock_array {
    void *data;
    size_t size;
    uintmax_t length;
    uintmax_t capacity;
};

/**
 * @brief Initialize array.
 * @param [in] object instance to be initialized.
 * @param [in] size of an item in the array.
 * @param [in] capacity set the initial capacity.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_SIZE_IS_ZERO if size is zero.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to initialize the array instance.
 */
int rock_array_init(struct rock_array *object,
                    size_t size,
                    uintmax_t capacity);

/**
 * @brief Invalidate the array.
 * <p>All the items contained within the array will have the given <i>on
 * destroy</i> callback invoked upon it. The actual <u>array instance
 * is not deallocated</u> since it may have been embedded in a larger
 * structure.</p>
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the item is to be destroyed.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
int rock_array_invalidate(struct rock_array *object,
                          void (*on_destroy)(void *));

/**
 * @brief Retrieve the capacity.
 * @param [in] object array instance.
 * @param [in] out receive the capacity.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_array_capacity(const struct rock_array *object,
                        uintmax_t *out);

/**
 * @brief Retrieve the length.
 * @param [in] object array instance.
 * @param [in] out receive the number of items.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_array_get_length(const struct rock_array *object,
                          uintmax_t *out);

/**
 * @brief Set the length.
 * @param [in] object array instance.
 * @param [in] length number of items in the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is not enough
 * memory to set the array length.
 */
int rock_array_set_length(struct rock_array *object,
                          uintmax_t length);

/**
 * @brief Retrieve the size of an item.
 * @param [in] object array instance.
 * @param [in] out receive the size of an item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
int rock_array_size(const struct rock_array *object,
                    size_t *out);

/**
 * @brief Release excess capacity.
 * @param [in] object array instance.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
int rock_array_shrink(struct rock_array *object);

/**
 * @brief Add an item at the end.
 * @param [in] object array instance.
 * @param [in] item to add at the end of the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is not enough
 * memory to add another item.
 */
int rock_array_add(struct rock_array *object,
                   const void *item);

/**
 * @brief Add all the items at the end.
 * @param [in] object array instance.
 * @param [in] count number of values in items.
 * @param [in] items all of which we would like to add to the end of the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_COUNT_IS_ZERO if count is zero.
 * @throws ROCK_ARRAY_ERROR_ITEMS_IS_NULL if items is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is not enough
 * memory to add all the items.
 */
int rock_array_add_all(struct rock_array *object,
                       uintmax_t count,
                       const void **items);

/**
 * @brief Remove last item.
 * @param [in] object array instance.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY if the array is empty.
 */
int rock_array_remove_last(struct rock_array *object);

/**
 * @brief Insert an item at index.
 * @param [in] object array instance.
 * @param [in] at index where item is to be inserted.
 * @param [in] item to be inserted.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is not enough
 * memory to add another item.
 */
int rock_array_insert(struct rock_array *object,
                      uintmax_t at,
                      const void *item);

/**
 * @brief Insert all the items at index.
 * @param [in] object array instance.
 * @param [in] at index where items are to be inserted.
 * @param [in] count number of values in items.
 * @param [in] items all of which we would like to insert into the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 * @throws ROCK_ARRAY_ERROR_COUNT_IS_ZERO if count is zero.
 * @throws ROCK_ARRAY_ERROR_ITEMS_IS_NULL if items is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is not enough
 * memory to add all the items to the array.
 */
int rock_array_insert_all(struct rock_array *object,
                          uintmax_t at,
                          uintmax_t count,
                          const void **items);

/**
 * @brief Remove an item at the given index.
 * @param [in] object array instance.
 * @param [in] at index of item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 */
int rock_array_remove(struct rock_array *object,
                      uintmax_t at);

/**
 * @brief Remove all the items from the given index up to count items.
 * @param [in] object array instance.
 * @param [in] at index of item.
 * @param [in] count of items to be removed.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_COUNT_IS_ZERO if count is zero.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 */
int rock_array_remove_all(struct rock_array *object,
                          uintmax_t at,
                          uintmax_t count);

/**
 * @brief Retrieve the item at the given index.
 * @param [in] object array instance.
 * @param [in] at index of item.
 * @param [out] out receive the <u>address of</u> the item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 */
int rock_array_get(const struct rock_array *object,
                   uintmax_t at,
                   void **out);

/**
 * @brief Set the item at the given index.
 * @param [in] object array instance.
 * @param [in] at index where item should be stored.
 * @param [in] item whose <u>pointed to contents</u> will be copied into the
 * array unless it is <i>NULL</i> then that index's contents will be zeroed out.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS if at does not refer to an
 * item contained within the array.
 */
int rock_array_set(struct rock_array *object,
                   uintmax_t at,
                   const void *item);

/**
 * @brief First item of the array.
 * @param [in] object array instance.
 * @param [out] out receive first item in the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY if the array is empty.
 */
int rock_array_first(const struct rock_array *object,
                     void **out);

/**
 * @brief Last item of the array.
 * @param [in] object array instance.
 * @param [out] out receive last item in the array.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY if the array is empty.
 */
int rock_array_last(const struct rock_array *object,
                    void **out);

/**
 * @brief Retrieve next item.
 * @param [in] object array instance.
 * @param [in] item current item.
 * @param [out] out receive the next item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS if item is not contained
 * within the array.
 * @throws ROCK_ARRAY_ERROR_END_OF_SEQUENCE if there is no next item.
 */
int rock_array_next(const struct rock_array *object,
                    const void *item,
                    void **out);

/**
 * @brief Retrieve previous item.
 * @param [in] object array instance.
 * @param [in] item current item.
 * @param [out] out receive the previous item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS if item is not contained
 * within the array.
 * @throws ROCK_ARRAY_ERROR_END_OF_SEQUENCE if there is no previous item.
 */
int rock_array_prev(const struct rock_array *object,
                    const void *item,
                    void **out);

/**
 * @brief Receive the index of item.
 * @param [in] object array instance.
 * @param [in] item whose index we would like.
 * @param [out] out receive the index of item.
 * @return On success <i>0</i>, otherwise an error code.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_NULL if item is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS if item is not contained
 * within the array.
 */
int rock_array_at(const struct rock_array *object,
                  const void *item,
                  uintmax_t *out);

#endif /* _ROCK_ARRAY_H_ */
