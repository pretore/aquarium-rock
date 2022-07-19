#ifndef _ROCK_TREE_MAP_H_
#define _ROCK_TREE_MAP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "red_black_tree.h"

#define ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL                      1
#define ROCK_TREE_MAP_ERROR_COMPARE_IS_NULL                     2
#define ROCK_TREE_MAP_ERROR_OUT_IS_NULL                         3
#define ROCK_TREE_MAP_ERROR_KEY_ALREADY_EXISTS                  4
#define ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND                       5
#define ROCK_TREE_MAP_ERROR_MEMORY_ALLOCATION_FAILED            6
#define ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY                   7
#define ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL                       8
#define ROCK_TREE_MAP_ERROR_END_OF_SEQUENCE                     9

struct rock_tree_map_entry_key {
    const void *data;
};

struct rock_tree_map_entry_value {
    void *data;
};

struct rock_tree_map_entry {
    struct rock_tree_map_entry_key key;
    struct rock_tree_map_entry_value value;
};

struct rock_tree_map {
    struct rock_red_black_tree tree;
};

/**
 * @brief Initialise tree map.
 * @param [in] object instance to be initialised.
 * @param [in] compare comparison which must return an integer less than,
 * equal to, or greater than zero if the <u>first key</u> is considered
 * to be respectively less than, equal to, or greater than the <u>second
 * key</u>.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_COMPARE_IS_NULL if compare is <i>NULL</i>.
 */
bool rock_tree_map_init(struct rock_tree_map *object,
                        int (*compare)(const struct rock_tree_map_entry_key *,
                                       const struct rock_tree_map_entry_key *));

/**
 * @brief Invalidate tree map.
 * <p>All the entries contained within the tree map will have the given <i>on
 * destroy</i> callback invoked upon itself. The actual <u>tree map instance
 * is not deallocated</u> since it may have been embedded in a larger structure.
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the entry is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 */
bool rock_tree_map_invalidate(struct rock_tree_map *object,
                              void (*on_destroy)(struct rock_tree_map_entry *));

/**
 * @brief Retrieve the count of entries.
 * @param [in] object instance whose count we are to retrieve.
 * @param [out] out receive the count.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_tree_map_count(struct rock_tree_map *object, size_t *out);

/**
 * @brief Add a key-value association.
 * @param [in] object tree map instance.
 * @param [in] key under which value will be associated with.
 * @param [in] value that will returned from a lookup with associated key.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_KEY_ALREADY_EXISTS if the key is already
 * present in the tree map.
 * @throws ROCK_TREE_MAP_ERROR_MEMORY_ALLOCATION_FAILED if there is not
 * enough memory to add key-value association to the tree map.
 */
bool rock_tree_map_add(struct rock_tree_map *object,
                       const void *key,
                       void *value);

/**
 * @brief Remove key-value association.
 * @param [in] object tree map instance.
 * @param [in] key for which we would like the key-value association removed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_tree_map_remove(struct rock_tree_map *object,
                          const void *key);

/**
 * @brief Set the value for the given key.
 * @param [in] object tree map instance.
 * @param [in] key used for which value is to be changed.
 * @param [in] value of new association with given key.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_tree_map_set(struct rock_tree_map *object,
                       const void *key,
                       void *value);

/**
 * @brief Retrieve the value for the given key.
 * @param [in] object tree map instance.
 * @param [in] key used to get the associated value.
 * @param [out] out receive the value.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_KEY_NOT_FOUND if key could not be found.
 */
bool rock_tree_map_get(struct rock_tree_map *object,
                       const void *key,
                       void **out);

/**
 * @brief Check if tree map contains the given key.
 * @param [in] object tree map instance.
 * @param [in] key to check if it is present.
 * @param [out] out true if key is present, otherwise false.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 */
bool rock_tree_map_contains(struct rock_tree_map *object,
                            const void *key,
                            bool *out);

/**
 * @brief First entry of the tree map.
 * @param [in] object tree set instance.
 * @param [out] out receive the first entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY if tree map is empty.
 */
bool rock_tree_map_first(struct rock_tree_map *object,
                         struct rock_tree_map_entry **out);

/**
 * @brief Last entry of the tree map.
 * @param [in] object tree set instance.
 * @param [out] out receive the last entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_OBJECT_IS_NULL if object is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_TREE_MAP_IS_EMPTY if tree map is empty.
 */
bool rock_tree_map_last(struct rock_tree_map *object,
                        struct rock_tree_map_entry **out);

/**
 * @brief Retrieve next entry.
 * @param [in] entry current entry.
 * @param [out] out receive the next entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL if entry is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no next entry.
 */
bool rock_tree_map_next(struct rock_tree_map_entry *entry,
                        struct rock_tree_map_entry **out);

/**
 * @brief Retrieve previous entry.
 * @param [in] entry current entry.
 * @param [out] out receive the previous entry.
 * @return On success true, otherwise false if an error has occurred.
 * @throws ROCK_TREE_MAP_ERROR_ENTRY_IS_NULL if entry is <i>NULL</i>.
 * @throws ROCK_TREE_MAP_ERROR_OUT_IS_NULL if out is <i>NULL</i>.
 * @throws ROCK_TREE_SET_ERROR_END_OF_SEQUENCE if there is no next previous.
 */
bool rock_tree_map_prev(struct rock_tree_map_entry *entry,
                        struct rock_tree_map_entry **out);

#endif /* _ROCK_TREE_MAP_H_ */
