#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <time.h>
#include <seagrass.h>
#include <rock.h>

#include "private/hash_table.h"
#include "test/cmocka.h"

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_init(NULL, 0, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_hash_code_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_init((void *)1, 0, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_HASH_CODE_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_is_equal_is_null(void **state) {
     rock_error = ROCK_ERROR_NONE;
     assert_false(rock_hash_table_init((void *)1, 0, (void *)1, NULL));
     assert_int_equal(ROCK_HASH_TABLE_ERROR_IS_EQUAL_IS_NULL, rock_error);
     rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_load_factor_is_invalid(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_init((void *)1, 5, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_LOAD_FACTOR_IS_INVALID, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static size_t hash_code(const void *object) {
    return (size_t)object;
}

static bool is_equal(const void *a, const void *b) {
    return 0 == seagrass_void_ptr_compare(a, b);
}

static void check_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0.6, hash_code, is_equal));
    assert_ptr_equal(hash_code, object.hash_code);
    assert_ptr_equal(is_equal, object.is_equal);
    assert_int_equal(sizeof(struct rock_hash_table_entry), object.array.size);
    bool result;
    assert_true(seagrass_float_is_equal(0.6, object.lf, &result));
    assert_true(result);
    assert_int_equal(0, object.count);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_ptr_equal(hash_code, object.hash_code);
    assert_ptr_equal(is_equal, object.is_equal);
    assert_int_equal(sizeof(struct rock_hash_table_entry), object.array.size);
    assert_true(seagrass_float_is_equal(0.75, object.lf, &result));
    assert_true(result);
    assert_int_equal(0, object.count);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void hash_code_on_destroy(struct rock_hash_table_entry *entry);

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    struct rock_hash_table_entry entries[] = {
            {.key.data = &check_invalidate, .value.data = NULL},
            {.key.data = &check_invalidate_error_on_object_is_null,
                .value.data = (void *)1},
            {.key.data = &check_init, .value.data = (void *)SIZE_MAX}
    };
    void *items[] = {
            &entries[0],
            &entries[1],
            NULL,
            &entries[2]
    };
    size_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object.array, count, items));
    object.count = 3; // there is one NULL item which is an empty slot
    expect_function_calls(hash_code_on_destroy, object.count);
    assert_true(rock_hash_table_invalidate(&object, hash_code_on_destroy));
    rock_error = ROCK_ERROR_NONE;
}

static void hash_code_on_destroy(struct rock_hash_table_entry *entry) {
    assert_non_null(entry);
    assert_true((&check_invalidate == entry->key.data && NULL == entry->value.data)
            || (&check_invalidate_error_on_object_is_null == entry->key.data
            && (void *)1 == entry->value.data)
            || (&check_init == entry->key.data
            && (void *)SIZE_MAX == entry->value.data));
    function_called();
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_count(NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_count((void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    size_t count;
    assert_true(rock_hash_table_count(&object, &count));
    assert_int_equal(object.count, count);
    object.count = 29813;
    assert_true(rock_hash_table_count(&object, &count));
    assert_int_equal(object.count, count);
    object.count = 0;
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_rebuild_needed_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_rebuild_needed(NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rebuild_needed_error_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_rebuild_needed((void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_rebuild_needed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    bool result;
    assert_true(rock_hash_table_rebuild_needed(&object, &result));
    assert_false(result);
    object.array.length = 100;
    object.deleted = 11;
    object.count = 9;
    assert_true(rock_hash_table_rebuild_needed(&object, &result));
    assert_true(result);
    object.array.length = 0;
    object.deleted = 0;
    object.count = 0;
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_rebuild_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_rebuild(NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_add(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_key_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_add((void*)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    object.count = SIZE_MAX;
    assert_false(rock_hash_table_add(&object, (void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    object.count = 0;
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_int_equal(0, object.count);
    void *key = (void *)(rand() % UINTPTR_MAX);
    assert_true(rock_hash_table_add(&object, key, (void*)2635));
    assert_int_equal(1, object.count);
    size_t length;
    assert_true(rock_array_get_length(&object.array, &length));
    assert_true(length != 0);
    const size_t at = object.hash_code(key) % length;
    struct rock_hash_table_entry *entry;
    assert_true(rock_array_get(&object.array, at, (void **)&entry));
    assert_true(entry->key.data == key && entry->value.data == (void*)2635);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_key_already_exists(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_int_equal(0, object.count);
    assert_true(rock_hash_table_add(&object,
                                    (const void *)1,
                                    (void*)2635));
    assert_int_equal(1, object.count);
    assert_false(rock_hash_table_add(&object,
                                     (const void *)1,
                                     NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_ALREADY_EXISTS, rock_error);
    assert_int_equal(1, object.count);
    assert_true(rock_hash_table_add(&object,
                                 (const void *)2,
                                 (void *)982));
    assert_int_equal(2, object.count);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_remove(NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_key_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_remove((void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_key_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_int_equal(0, object.count);
    assert_false(rock_hash_table_remove(&object, (const void *)~0));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_add(&object,
                                    (const void *)1,
                                    (void*)2635));
    assert_int_equal(1, object.count);
    assert_false(rock_hash_table_remove(&object,
                                        (const void *)~0));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    void *key = (void *)(rand() % UINTPTR_MAX);
    assert_true(rock_hash_table_add(&object, key, (void*)2635));
    assert_int_equal(1, object.count);
    assert_int_equal(0, object.deleted);
    assert_true(rock_hash_table_remove(&object, key));
    assert_int_equal(0, object.count);
    assert_int_equal(1, object.deleted);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_set(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_key_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_set((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_key_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_false(rock_hash_table_set(&object,
                                     (const void *)~0,
                                     NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object, (const void *)1, NULL));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_entry_find(&object, (const void *)1, &entry));
    assert_true((const void *)1 == entry->key.data
                && NULL == entry->value.data);
    assert_true(rock_hash_table_set(&object, (const void *)1, (void*)524));
    assert_true((const void *)1 == entry->key.data
                && (void*)524 == entry->value.data);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_get(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_key_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_get((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_get((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_key_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_int_equal(0, object.count);
    size_t value;
    assert_false(rock_hash_table_get(&object,
                                     (const void *)1,
                                     (void **)&value));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    NULL));
    assert_int_equal(1, object.count);
    assert_false(rock_hash_table_get(&object,
                                     (const void *)1,
                                     (void **)&value));
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    void *key = (void *)(rand() % UINTPTR_MAX);
    assert_true(rock_hash_table_add(&object, key, (void*) 1821));
    assert_int_equal(1, object.count);
    size_t value;
    assert_true(rock_hash_table_get(&object,
                                     key,
                                     (void **)&value));
    assert_int_equal(1821, value);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_contains(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_key_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_contains((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_KEY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_contains((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    bool result;
    void *key = (void *)(rand() % UINTPTR_MAX);
    assert_true(rock_hash_table_contains(&object, key, &result));
    assert_false(result);
    assert_true(rock_hash_table_add(&object, key, NULL));
    assert_true(rock_hash_table_contains(&object, key, &result));
    assert_true(result);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_first(NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_first((void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_hash_table_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_false(rock_hash_table_first(&object, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(
            &object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)1,
                                    (void *)1213));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_first(&object, &entry));
    assert_true((const void *)1 == entry->key.data
                && (void *)1213 == entry->value.data);
    assert_true(rock_hash_table_add(&object,
                                    (const void *)10,
                                    (void *)5241));
    assert_true(rock_hash_table_remove(&object, (const void *)1));
    assert_true(rock_hash_table_first(&object, &entry));
    assert_true((const void *)10 == entry->key.data
                && (void *)5241 == entry->value.data);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_last(NULL, (void*)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_last((void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_hash_table_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_false(rock_hash_table_last(&object, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_HASH_TABLE_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(
            &object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~1,
                                    (void *)1214));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_last(&object, &entry));
    assert_true((const void *)~1 == entry->key.data
                && (void *)1214 == entry->value.data);
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    (void *)957));
    assert_true(rock_hash_table_last(&object, &entry));
    assert_true((const void *)~0 == entry->key.data
                && (void *)957 == entry->value.data);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_next(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_entry_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_next((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_next((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_entry_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    (void *)1));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_first(&object, &entry));
    assert_false(rock_hash_table_next(&object, (void *)1, &entry));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    NULL));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_first(&object, &entry));
    assert_false(rock_hash_table_next(&object, entry, &entry));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    NULL));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~1,
                                    (void *)1));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_first(&object, &entry));
    assert_true(rock_hash_table_next(&object, entry, &entry));
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_prev(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_entry_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_prev((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_ENTRY_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_hash_table_prev((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_entry_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    (void *)1));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_last(&object, &entry));
    assert_false(rock_hash_table_prev(&object, (void *)1, &entry));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_ENTRY_NOT_FOUND, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    NULL));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_last(&object, &entry));
    assert_false(rock_hash_table_prev(&object, entry, &entry));
    assert_int_equal(ROCK_HASH_TABLE_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_hash_table object = {};
    assert_true(rock_hash_table_init(&object, 0, hash_code, is_equal));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~0,
                                    NULL));
    assert_true(rock_hash_table_add(&object,
                                    (const void *)~1,
                                    (void *)1));
    struct rock_hash_table_entry *entry;
    assert_true(rock_hash_table_last(&object, &entry));
    assert_true(rock_hash_table_prev(&object, entry, &entry));
    assert_true(rock_hash_table_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_hash_code_is_null),
            cmocka_unit_test(check_init_error_on_is_equal_is_null),
            cmocka_unit_test(check_init_error_on_load_factor_is_invalid),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_rebuild_needed_error_on_object_is_null),
            cmocka_unit_test(check_rebuild_needed_error_out_is_null),
            cmocka_unit_test(check_rebuild_needed),
            cmocka_unit_test(check_rebuild_error_on_object_is_null),
            //cmocka_unit_test(check_rebuild)
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add_error_on_key_is_null),
            cmocka_unit_test(check_add_error_on_memory_allocation_failed),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_on_key_already_exists),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_key_is_null),
            cmocka_unit_test(check_remove_error_on_key_not_found),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_set_error_on_object_is_null),
            cmocka_unit_test(check_set_error_on_key_is_null),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_get_error_on_object_is_null),
            cmocka_unit_test(check_get_error_on_key_is_null),
            cmocka_unit_test(check_get_error_on_key_not_found),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_contains_error_on_object_is_null),
            cmocka_unit_test(check_contains_error_on_key_is_null),
            cmocka_unit_test(check_contains_error_on_out_is_null),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_hash_table_is_empty),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_out_is_null),
            cmocka_unit_test(check_last_error_on_hash_table_is_empty),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_object_is_null),
            cmocka_unit_test(check_next_error_on_entry_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next_error_on_entry_not_found),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_object_is_null),
            cmocka_unit_test(check_prev_error_on_entry_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev_error_on_entry_not_found),
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
