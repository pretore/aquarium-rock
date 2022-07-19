#ifndef _ROCK_PRIVATE_HASH_TABLE_H_
#define _ROCK_PRIVATE_HASH_TABLE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ROCK_HASH_TABLE_ENTRY_EMPTY     \
    (struct rock_hash_table_entry) {0}
#define ROCK_HASH_TABLE_ENTRY_DELETED   \
    (struct rock_hash_table_entry) { .value = (void *) 1}

#define ROCK_HASH_TABLE_LOAD_FACTOR_DEFAULT                 0.75f
#define ROCK_HASH_TABLE_LOAD_FACTOR_MAXIMUM                 1.0f
#define ROCK_HASH_TABLE_LOAD_FACTOR_UNSPECIFIED             0.0f

#define ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_FULL            (-1)

struct rock_hash_table;
struct rock_hash_table_entry;

void rock_hash_table_entry_swap(struct rock_hash_table_entry *restrict a,
                                struct rock_hash_table_entry *restrict b);

bool rock_hash_table_entry_find(struct rock_hash_table *object,
                                const void *key,
                                struct rock_hash_table_entry **out);

bool
rock_hash_table_entry_equals(const struct rock_hash_table_entry *restrict a,
                             const struct rock_hash_table_entry *restrict b);

bool rock_hash_table_insert(struct rock_hash_table *object,
                            struct rock_hash_table_entry *entry,
                            bool ignore_duplicates);

#endif /* _ROCK_PRIVATE_HASH_TABLE_H_ */
