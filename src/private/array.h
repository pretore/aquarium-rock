#ifndef _ROCK_PRIVATE_ARRAY_H_
#define _ROCK_PRIVATE_ARRAY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct rock_array;

void *rock_array_address(struct rock_array *object, size_t at);

/**
 * @brief Set the capacity.
 * @param [in] object array instance.
 * @param [in] capacity new desired array capacity.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to set the new capacity of the array.
 */
bool rock_array_set_capacity(struct rock_array *object, size_t capacity);

/**
 * @brief Adjust the capacity if needed.
 * @param [in] object array instance.
 * @param [in] count number of items that we would like to add to the array.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_ARRAY_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_ARRAY_ERROR_COUNT_IS_ZERO if count is zero.
 * @throws ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED if there is
 * insufficient memory to set the new capacity of the array.
 */
bool rock_array_adjust_capacity(struct rock_array *object, size_t count);

#endif /* _ROCK_PRIVATE_ARRAY_H_ */
