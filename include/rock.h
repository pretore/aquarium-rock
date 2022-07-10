#ifndef _ROCK_ROCK_H_
#define _ROCK_ROCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <rock/error.h>
#include <rock/red_black_tree.h>
#include <rock/tree_map.h>
#include <rock/tree_set.h>

/**
 * @brief Require that object be non-<i>NULL</i>.
 * @param [in] object to check if it is non-<i>NULL</i>.
 * @note Will call abort(3) if object is <i>NULL</i>.
 */
void rock_required(const void *object);

/**
 * @brief Require true evaluation.
 * @param [in] check to see if it is true.
 * @note Will call abort(3) if check is false.
 */
void rock_required_true(int check);

/**
 * @brief Comparison function for sorting void* data.
 * @param [in] a first void* to compare.
 * @param [in] b second void* to compare.
 * @return <i>-1</i> if <b>a</b> is <u>less than</u> <b>b</b>,
 * <i>0</i> if <b>a</b> is <u>equal to</u> <b>b</b> or <i>1</i> if <b>a</b> is
 * <u>greater than</u> <b>b</b>.
 */
int rock_compare_void_ptr(const void *a, const void *b);

/**
 * @brief Comparison function for sorting size_t* data.
 * @param [in] a first size_t* to compare.
 * @param [in] b second size_t* to compare.
 * @return <i>-1</i> if <b>a</b> is <u>less than</u> <b>b</b>,
 * <i>0</i> if <b>a</b> is <u>equal to</u> <b>b</b> or <i>1</i> if <b>a</b> is
 * <u>greater than</u> <b>b</b>.
 */
int rock_compare_size_t_ptr(const void *a, const void *b);

/**
 * @brief Comparison function for sorting size_t data.
 * @param [in] a first size_t to compare.
 * @param [in] b second size_t to compare.
 * @return <i>-1</i> if <b>a</b> is <u>less than</u> <b>b</b>,
 * <i>0</i> if <b>a</b> is <u>equal to</u> <b>b</b> or <i>1</i> if <b>a</b> is
 * <u>greater than</u> <b>b</b>.
 */
int rock_compare_size_t(const void *a, const void *b);

/**
 * @brief Add two size_t values together and check for overflow.
 * @param [in] a first unsigned size_t.
 * @param [in] b second unsigned size_t.
 * @param [out] out where the result is stored if successful.
 * @return On success true otherwise false if an error occurred.
 * @throws ROCK_ERROR_OUT_IS_NULL if the out is <i>NULL</i>.
 * @throws ROCK_ERROR_OVERFLOW_OCCURRED if an integer overflow has occurred.
 */
bool rock_add_size_t(size_t a, size_t b, size_t *out);

/**
 * @brief Multiply two size_t values and check for overflow.
 * @param [in] a first unsigned integer.
 * @param [in] b second unsigned integer.
 * @param [out] out where the result is stored if successful.
 * @return On success true otherwise false if an error occurred.
 * @throws ROCK_ERROR_OUT_IS_NULL if the out is <i>NULL</i>.
 * @throws ROCK_ERROR_OVERFLOW_OCCURRED if an integer overflow has occurred.
 */
bool rock_multiply_size_t(size_t a, size_t b, size_t *out);

/**
 * @brief Return the minimum size_t value.
 * @param [in] a first unsigned integer.
 * @param [in] b second unsigned integer.
 * @param [out] out where the result is stored ir successful.
 * @return On success true otherwise false if an error occurred.
 * @throws ROCK_ERROR_OUT_IS_NULL if the out argument is <i>NULL</i>.
 */
bool rock_minimum_size_t(size_t a, size_t b, size_t *out);

/**
 * @brief Return the maximum size_t value.
 * @param [in] a first unsigned integer.
 * @param [in] b second unsigned integer.
 * @param [out] out where the result is stored if successful.
 * @return On success true otherwise false if an error occurred.
 * @throws ROCK_ERROR_OUT_IS_NULL if the out argument is <i>NULL</i>.
 */
bool rock_maximum_size_t(size_t a, size_t b, size_t *out);

#endif /* _ROCK_ROCK_H_ */
