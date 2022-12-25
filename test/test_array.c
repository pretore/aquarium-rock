#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <time.h>
#include <rock.h>

#include "private/array.h"
#include "test/cmocka.h"

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_init(NULL, sizeof(void *), 1233));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_size_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_init((void *)1, 0, SIZE_MAX));
    assert_int_equal(ROCK_ARRAY_ERROR_SIZE_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_init(&object, sizeof(void *), SIZE_MAX));
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_false(rock_array_init(&object, SIZE_MAX, sizeof(void *)));
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    calloc_is_overridden = malloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_init(&object, sizeof(void *), 10));
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    calloc_is_overridden = malloc_is_overridden = realloc_is_overridden = false;
    rock_error = ROCK_ERROR_NONE;
}

static void check_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    assert_int_equal(0, object.capacity);
    assert_int_equal(0, object.length);
    assert_int_equal(sizeof(void *), object.size);
    assert_null(object.data);
    assert_true(rock_array_invalidate(&object, NULL));
    assert_true(rock_array_init(&object, sizeof(void *), 10));
    assert_int_equal(10, object.capacity);
    assert_int_equal(0, object.length);
    assert_int_equal(sizeof(void *), object.size);
    assert_non_null(object.data);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void array_on_destroy(void *item) {
    assert_non_null(item);
    function_called();
}

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_true(rock_array_init(&object, sizeof(void *), 10));
    assert_true(rock_array_invalidate(&object, array_on_destroy));
    assert_true(rock_array_init(&object, sizeof(void *), 10));
    object.length = 10;
    expect_function_calls(array_on_destroy, 10);
    assert_true(rock_array_invalidate(&object, array_on_destroy));
    rock_error = ROCK_ERROR_NONE;
}

static void check_capacity_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_capacity(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_capacity_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_capacity((void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_capacity(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .capacity = rand() % UINTMAX_MAX
    };
    uintmax_t out;
    assert_true(rock_array_capacity(&object, &out));
    assert_int_equal(out, object.capacity);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_capacity_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_set_capacity(NULL, 123));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void
check_set_capacity_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .size = sizeof(void *)
    };
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_set_capacity(&object, 10));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_capacity(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    assert_int_equal(0, object.capacity);
    assert_null(object.data);
    assert_true(rock_array_set_capacity(&object, 10));
    assert_int_equal(10, object.capacity);
    assert_non_null(object.data);
    assert_true(rock_array_set_capacity(&object, 100));
    assert_int_equal(100, object.capacity);
    assert_non_null(object.data);
    assert_true(rock_array_set_capacity(&object, 10));
    assert_int_equal(10, object.capacity);
    assert_non_null(object.data);
    assert_true(rock_array_set_capacity(&object, 0));
    assert_int_equal(0, object.capacity);
    assert_null(object.data);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_length_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_get_length(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_length_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_get_length((void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_length(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .length = rand() % UINTMAX_MAX
    };
    uintmax_t out;
    assert_true(rock_array_get_length(&object, &out));
    assert_int_equal(out, object.length);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_length_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_set_length(NULL, 0));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_length_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_set_length(&object, 1));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_length(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    assert_int_equal(0, object.length);
    assert_true(rock_array_set_length(&object, 100));
    assert_int_equal(100, object.length);
    assert_true(rock_array_set_length(&object, 0));
    assert_int_equal(0, object.length);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_size_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_size(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_size_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_size((void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_size(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .size = rand() % UINTMAX_MAX
    };
    uintmax_t out;
    assert_true(rock_array_size(&object, &out));
    assert_int_equal(object.size, out);
    rock_error = ROCK_ERROR_NONE;
}

static void check_shrink_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_shrink(NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_shrink(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_true(rock_array_init(&object, sizeof(void *), 12));
    uintmax_t capacity;
    assert_true(rock_array_capacity(&object, &capacity));
    assert_int_equal(12, capacity);
    assert_true(rock_array_shrink(&object));
    assert_true(rock_array_capacity(&object, &capacity));
    assert_int_equal(0, capacity);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_add(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_memory_allocation_failed(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_add(&object, &rock_error));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const uintmax_t count = sizeof(values) / sizeof(uintmax_t);
    for (uintmax_t i = 0; i < count; i++) {
        assert_int_equal(object.length, i);
        assert_true(rock_array_add(&object, &values[i]));
        assert_int_equal(object.length, 1 + i);
        assert_memory_equal(rock_array_address(&object, i),
                            &values[i],
                            object.size);
    }
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_all_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const void *items[] = {
            NULL
    };
    assert_false(rock_array_add_all(NULL, 1, items));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_all_error_on_count_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const void *items[] = {
            NULL
    };
    assert_false(rock_array_add_all((void *)1, 0, items));
    assert_int_equal(ROCK_ARRAY_ERROR_COUNT_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_all_error_on_items_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_add_all((void *)1, 1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEMS_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_all_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2]
    };
    const uintmax_t count = sizeof(items) / sizeof(uintmax_t *);
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_add_all(&object, count, items));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_all(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 1));
    assert_non_null(object.data);
    assert_int_equal(object.capacity, 1);
    assert_int_equal(object.length, 0);
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    assert_non_null(object.data);
    assert_int_equal(object.capacity, 4);
    assert_int_equal(count, object.length);
    for (uintmax_t i = 0; i < count; i++) {
        assert_memory_equal(items[i],
                            rock_array_address(&object, i),
                            object.size);
    }
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_remove_last(NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_last_error_on_array_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_remove_last(&object));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_last(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    assert_true(rock_array_add(&object, check_remove_last));
    uintmax_t length;
    assert_true(rock_array_get_length(&object, &length));
    assert_int_equal(1, length);
    assert_true(rock_array_remove_last(&object));
    assert_true(rock_array_get_length(&object, &length));
    assert_int_equal(0, length);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_insert(NULL, 0, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_insert(&object, 0, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
    };
    assert_true(rock_array_add(&object, items[0]));
    object.capacity = 1;
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_insert(&object, 0, items[1]));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX
    };
    const void *items[] = {
            &values[0],
            &values[1]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add(&object, items[0]));
    assert_true(rock_array_insert(&object, 0, items[1]));
    for (uintmax_t i = 0; i < count; i++) {
        assert_memory_equal(rock_array_address(&object, i),
                            items[1 - i],
                            object.size);
    }
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    const void *items[] = {
            NULL
    };
    assert_false(rock_array_insert_all(NULL, 0, 1, items));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    const void *items[] = {
            NULL
    };
    assert_false(rock_array_insert_all(&object, 0, 1, items));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all_error_on_count_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .length = 1
    };
    const void *items[] = {
             NULL
    };
    assert_false(rock_array_insert_all(&object, 0, 0, items));
    assert_int_equal(ROCK_ARRAY_ERROR_COUNT_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all_error_on_items_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .length = 1
    };
    assert_false(rock_array_insert_all(&object, 0, 1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEMS_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    object.length = 1;
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = true;
    assert_false(rock_array_insert_all(&object, 0, count, items));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden = false;
    assert_int_equal(ROCK_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_all(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t value = rand() % UINTMAX_MAX;
    assert_true(rock_array_add(&object, &value));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_insert_all(&object, 0, count, items));
    for (uintmax_t i = 0; i < count; i++) {
        assert_memory_equal(rock_array_address(&object, i),
                            items[i],
                            object.size);
    }
    assert_memory_equal(rock_array_address(&object, count),
                        &value,
                        object.size);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_remove(NULL, 0));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_remove(&object, 0));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t value = rand() % UINTMAX_MAX;
    assert_true(rock_array_add(&object, &value));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_insert_all(&object, 0, count, items));
    assert_int_equal(3, object.length);
    assert_true(rock_array_remove(&object, 0));
    assert_int_equal(2, object.length);
    assert_memory_equal(rock_array_address(&object, 0),
                        items[1],
                        object.size);
    assert_memory_equal(rock_array_address(&object, 1),
                        &value,
                        object.size);
    assert_true(rock_array_remove(&object, 1));
    assert_int_equal(1, object.length);
    assert_memory_equal(rock_array_address(&object, 0),
                        items[1],
                        object.size);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_all_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_remove_all(NULL, 0, 1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_all_error_on_count_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .length = 1
    };
    assert_false(rock_array_remove_all(&object, 0, 0));
    assert_int_equal(ROCK_ARRAY_ERROR_COUNT_IS_ZERO, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_all_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_remove_all(&object, 0, 1));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_all(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2],
            &values[3]
    };
    uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    assert_int_equal(4, object.length);
    assert_true(rock_array_remove_all(&object, 1, 672532));
    assert_int_equal(1, object.length);
    assert_memory_equal(rock_array_address(&object, 0),
                        items[0],
                        object.size);
    assert_true(rock_array_add_all(&object, count, items));
    assert_int_equal(5, object.length);
    assert_true(rock_array_remove_all(&object, 1, 3));
    assert_int_equal(2, object.length);
    assert_memory_equal(rock_array_address(&object, 0),
                        items[0],
                        object.size);
    assert_memory_equal(rock_array_address(&object, 1),
                        items[3],
                        object.size);
    assert_true(rock_array_remove_all(&object, 0, 2));
    assert_int_equal(0, object.length);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_get(NULL, 1, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_get((void *)1, 1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_get(&object, 0, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2],
            &values[3]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    for (uintmax_t i = 0; i < count; i++) {
        void *out;
        assert_true(rock_array_get(&object, i, &out));
        assert_memory_equal(out, items[i], object.size);
    }
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_set(NULL, 0, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_set_error_on_index_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    assert_false(rock_array_set(&object, 0, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_INDEX_IS_OUT_OF_BOUNDS, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_set(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1],
            &values[2]
    };
    uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    assert_true(rock_array_set(&object, 0, NULL));
    const uintmax_t null = 0;
    uintmax_t *out;
    assert_true(rock_array_get(&object, 0, (void **) &out));
    assert_memory_equal(out, &null, object.size);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_first(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_first((void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_array_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_first(&object, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(void *), 0));
    const void *values[] = {
            (const void *) (rand() % UINTMAX_MAX),
            (const void *) (rand() % UINTMAX_MAX)
    };
    void *first;
    assert_true(rock_array_add(&object, &values[0]));
    assert_true(rock_array_first(&object, (void **) &first));
    assert_memory_equal(first, &values[0], sizeof(void *));
    assert_true(rock_array_remove(&object, 0));
    assert_true(rock_array_add(&object,  &values[1]));
    assert_true(rock_array_first(&object, (void **) &first));
    assert_memory_equal(first, &values[1], sizeof(void *));
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_last(NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_last((void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_array_is_empty(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {};
    assert_false(rock_array_last(&object, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ARRAY_IS_EMPTY, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    assert_true(rock_array_add(&object, &values[0]));
    uintmax_t *last;
    assert_true(rock_array_last(&object, (void **) &last));
    assert_memory_equal(last, &values[0], sizeof(uintmax_t));
    assert_true(rock_array_remove(&object, 0));
    assert_true(rock_array_add(&object, &values[1]));
    assert_true(rock_array_last(&object, (void **) &last));
    assert_memory_equal(last, &values[1], sizeof(uintmax_t));
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_next(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_next((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEM_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_next((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_item_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .data = (void *) 0x1000,
            .length = 1
    };
    assert_false(rock_array_next(&object, (void *)1, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t value = rand() % UINTMAX_MAX;
    assert_true(rock_array_add(&object, &value));
    uintmax_t *item;
    assert_true(rock_array_first(&object, (void **) &item));
    assert_false(rock_array_next(&object, item, (void **) &item));
    assert_int_equal(ROCK_ARRAY_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    void *item;
    assert_true(rock_array_first(&object, &item));
    assert_memory_equal(item, items[0], object.size);
    assert_true(rock_array_next(&object, item, &item));
    assert_memory_equal(item, items[1], object.size);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_prev(NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_OBJECT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_prev((void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEM_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_array_prev((void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_ARRAY_ERROR_OUT_IS_NULL, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_item_is_out_of_bounds(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object = {
            .data = (void *) 0x2000,
            .length = 1
    };
    assert_false(rock_array_prev(&object, (void *)1, (void *)1));
    assert_int_equal(ROCK_ARRAY_ERROR_ITEM_IS_OUT_OF_BOUNDS, rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t value = rand() % UINTMAX_MAX;
    assert_true(rock_array_add(&object, &value));
    void *item;
    assert_true(rock_array_last(&object, &item));
    assert_false(rock_array_prev(&object, item, &item));
    assert_int_equal(ROCK_ARRAY_ERROR_END_OF_SEQUENCE, rock_error);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_array object;
    assert_true(rock_array_init(&object, sizeof(uintmax_t), 0));
    const uintmax_t values[] = {
            rand() % UINTMAX_MAX,
            rand() % UINTMAX_MAX,
    };
    const void *items[] = {
            &values[0],
            &values[1]
    };
    const uintmax_t count = sizeof(items) / sizeof(void *);
    assert_true(rock_array_add_all(&object, count, items));
    void *item;
    assert_true(rock_array_last(&object, &item));
    assert_memory_equal(item, items[1], object.size);
    assert_true(rock_array_prev(&object, item, &item));
    assert_memory_equal(item, items[0], object.size);
    assert_true(rock_array_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_size_is_zero),
            cmocka_unit_test(check_init_error_on_memory_allocation_failed),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_capacity_error_on_object_is_null),
            cmocka_unit_test(check_capacity_error_on_out_is_null),
            cmocka_unit_test(check_capacity),
            cmocka_unit_test(check_set_capacity_error_on_object_is_null),
            cmocka_unit_test(check_set_capacity_error_on_memory_allocation_failed),
            cmocka_unit_test(check_set_capacity),
            cmocka_unit_test(check_get_length_error_on_object_is_null),
            cmocka_unit_test(check_get_length_error_on_out_is_null),
            cmocka_unit_test(check_get_length),
            cmocka_unit_test(check_set_length_error_on_object_is_null),
            cmocka_unit_test(check_set_length_error_on_memory_allocation_failed),
            cmocka_unit_test(check_set_length),
            cmocka_unit_test(check_size_error_on_object_is_null),
            cmocka_unit_test(check_size_error_on_out_is_null),
            cmocka_unit_test(check_size),
            cmocka_unit_test(check_shrink_error_on_object_is_null),
            cmocka_unit_test(check_shrink),
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add_error_on_memory_allocation_failed),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_all_error_on_object_is_null),
            cmocka_unit_test(check_add_all_error_on_count_is_zero),
            cmocka_unit_test(check_add_all_error_on_items_is_null),
            cmocka_unit_test(check_add_all_error_on_memory_allocation_failed),
            cmocka_unit_test(check_add_all),
            cmocka_unit_test(check_remove_last_error_on_object_is_null),
            cmocka_unit_test(check_remove_last_error_on_array_is_empty),
            cmocka_unit_test(check_remove_last),
            cmocka_unit_test(check_insert_error_on_object_is_null),
            cmocka_unit_test(check_insert_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_insert_error_on_memory_allocation_failed),
            cmocka_unit_test(check_insert),
            cmocka_unit_test(check_insert_all_error_on_object_is_null),
            cmocka_unit_test(check_insert_all_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_insert_all_error_on_count_is_zero),
            cmocka_unit_test(check_insert_all_error_on_items_is_null),
            cmocka_unit_test(check_insert_all_error_on_memory_allocation_failed),
            cmocka_unit_test(check_insert_all),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_remove_all_error_on_object_is_null),
            cmocka_unit_test(check_remove_all_error_on_count_is_zero),
            cmocka_unit_test(check_remove_all_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_remove_all),
            cmocka_unit_test(check_get_error_on_object_is_null),
            cmocka_unit_test(check_get_error_on_out_is_null),
            cmocka_unit_test(check_get_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_set_error_on_object_is_null),
            cmocka_unit_test(check_set_error_on_index_is_out_of_bounds),
            cmocka_unit_test(check_set),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_array_is_empty),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_out_is_null),
            cmocka_unit_test(check_last_error_on_array_is_empty),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_next_error_on_object_is_null),
            cmocka_unit_test(check_next_error_on_item_is_null),
            cmocka_unit_test(check_next_error_on_item_is_out_of_bounds),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_object_is_null),
            cmocka_unit_test(check_prev_error_on_item_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev_error_on_item_is_out_of_bounds),
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
