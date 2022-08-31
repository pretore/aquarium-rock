#ifndef _ROCK_HASH_TABLE_H_
#define _ROCK_HASH_TABLE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "array.h"

#define ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL                    1
#define ROCK_HASH_TABLE_ERROR_HASH_CODE_IS_NULL                 2
#define ROCK_HASH_TABLE_ERROR_IS_EQUAL_IS_NULL                  3
#define ROCK_HASH_TABLE_ERROR_LOAD_FACTOR_IS_INVALID            4
#define ROCK_HASH_TABLE_ERROR_OUT_IS_NULL                       5
#define ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED          6
#define ROCK_HASH_TABLE_ERROR_KEY_IS_NULL                       7
#define ROCK_HASH_TABLE_ERROR_KEY_ALREADY_EXISTS                8
#define ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND                     9
#define ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY               10
#define ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL                     11
#define ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE                   12
#define ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND                   13

struct rock_hash_table_entry_key {
    const void *data;
};

struct rock_hash_table_entry_value {
    void *data;
};

struct rock_hash_table_entry {
    struct rock_hash_table_entry_key key;
    struct rock_hash_table_entry_value value;
};

struct rock_hash_table {
    struct rock_array array;
    uintmax_t count;
    uintmax_t deleted;
    float lf;

    uintmax_t (*hash_code)(const void *);
    bool (*is_equal)(const void *, const void *);
};

/**
 * @brief Initialise hash table.
 * @param [in] object instance to be initialised.
 * @param [in] load_factor absolute value between 0 (exclusive) and 1.0
 * (inclusive) that represents the occupancy level, when reached, that the
 * hash table will be increased in size to stay below that occupancy level.
 * If zero is provided, then a <u>default value of 0.75</u> is applied.
 * @param [in] hash_code function used to generate a hash code.
 * @param [in] is_equal function used to check if two items are equal.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_HASH_CODE_IS_NULL if hash_code is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_IS_EQUAL_IS_NULL if is_equal id <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_LOAD_FACTOR_IS_INVALID if load_factor is
 * greater than one.
 */
bool rock_hash_table_init(struct rock_hash_table *object,
                          float load_factor,
                          uintmax_t (*hash_code)(const void *),
                          bool (*is_equal)(const void *, const void *));

/**
 * @brief Invalidate the hash table.
 * <p>All the items contained within the hash table will have the given <i>on
 * destroy</i> callback invoked upon it. The actual <u>hash table instance
 * is not deallocated</u> since it may have been embedded in a larger
 * structure.</p>
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the item is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool
rock_hash_table_invalidate(struct rock_hash_table *object,
                           void (*on_destroy)(struct rock_hash_table_entry *));

/**
 * @brief Retrieve the count of entries.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_hash_table_count(struct rock_hash_table *object, uintmax_t *out);

/**
 * @brief Check if a rebuild is needed.
 * <p>If more than 10% of the capacity is marked as deleted and the
 * deleted entries outnumber all other entries.</p>
 * @param [in] object hash table instance.
 * @param [out] out return true if rebuild needed, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_hash_table_rebuild_needed(struct rock_hash_table *object, bool *out);

/**
 * @brief Rebuild hash table.
 * @param [in] object hash table instance.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @note A rebuild could invalidate any entries given for navigation.
 * @see rock_hash_table_first
 * @see rock_hash_table_last
 * @see rock_hash_table_next
 * @see rock_hash_table_prev
 */
bool rock_hash_table_rebuild(struct rock_hash_table *object);

/**
 * @brief Add a key-value association.
 * @param [in] object hash table instance.
 * @param [in] key under which value will be associated with.
 * @param [in] value that will returned from a lookup with associated key.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_IS_NULL if key is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_ALREADY_EXISTS if the key is already
 * present in the hash table.
 * @throws ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED if there is not
 * enough memory to add another entry.
 * @note Will trigger a rebuild if the hash table needs to resized.
 * @see rock_hash_table_rebuild
 */
bool rock_hash_table_add(struct rock_hash_table *object,
                         const void *key,
                         void *value);

/**
 * @brief Remove key-value association.
 * @param [in] object hash table instance.
 * @param [in] key for which we would like the key-value association removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_IS_NULL if key is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_hash_table_remove(struct rock_hash_table *object,
                            const void *key);

/**
 * @brief Set the value for the given key.
 * @param [in] object hash table instance.
 * @param [in] key used for which value is to be changed.
 * @param [in] value of new association with given key.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_IS_NULL if key is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_hash_table_set(struct rock_hash_table *object,
                         const void *key,
                         void *value);

/**
 * @brief Retrieve the value for the given key.
 * @param [in] object hash table instance.
 * @param [in] key used to get the associated value.
 * @param [out] out receive the value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_IS_NULL if key is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_hash_table_get(struct rock_hash_table *object,
                         const void *key,
                         void **out);

/**
 * @brief Check if hash table contains the given key.
 * @param [in] object hash table instance.
 * @param [in] key used to get the associated value.
 * @param [out] out true if key is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_KEY_IS_NULL if key is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_hash_table_contains(struct rock_hash_table *object,
                              const void *key,
                              bool *out);

/**
 * @brief First entry of the hash table.
 * @param [in] object hash table instance.
 * @param [out] out receive the first entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY if hash table is empty.
 * @note A rebuild could invalidate the received first entry.
 * @see rock_hash_table_rebuild
 */
bool rock_hash_table_first(struct rock_hash_table *object,
                           struct rock_hash_table_entry **out);

/**
 * @brief Last entry of the hash table.
 * @param [in] object hash table instance.
 * @param [out] out receive the last entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY if hash table is empty.
 * @note A rebuild could invalidate the received last entry.
 * @see rock_hash_table_rebuild
 */
bool rock_hash_table_last(struct rock_hash_table *object,
                          struct rock_hash_table_entry **out);

/**
 * @brief Retrieve next entry.
 * @param [in] object hash table instance.
 * @param [in] entry current entry.
 * @param [out] out receive the next entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL if entry is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND if entry is not contained
 * within the hash table.
 * @throws ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE if there is no next entry.
 * @note A rebuild could invalidate entries.
 * @see rock_hash_table_rebuild
 */
bool rock_hash_table_next(struct rock_hash_table *object,
                          struct rock_hash_table_entry *entry,
                          struct rock_hash_table_entry **out);

/**
 * @brief Retrieve previous entry.
 * @param [in] object hash table instance.
 * @param [in] entry current entry.
 * @param [out] out receive the previous entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL if entry is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND if entry is not contained
 * within the hash table.
 * @throws ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE if there is no previous entry.
 * @note A rebuild could invalidate entries.
 * @see rock_hash_table_rebuild
 */
bool rock_hash_table_prev(struct rock_hash_table *object,
                          struct rock_hash_table_entry *entry,
                          struct rock_hash_table_entry **out);

#endif /* _ROCK_HASH_TABLE_H_ */
