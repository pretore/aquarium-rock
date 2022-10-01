#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <time.h>
#include <seagrass.h>
#include <rock.h>

#include "test/cmocka.h"

static int compare_uintptr_t(const void *const a, const void *const b) {
    const uintptr_t A = *(uintptr_t *)a;
    const uintptr_t B = *(uintptr_t *)b;
    return seagrass_uintmax_t_compare(A, B);
}

static void check_invalidate_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_invalidate(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_invalidate(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object = {};
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_init(NULL, 1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_size_is_zero(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_init((void *)1, 0, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_SIZE_IS_ZERO,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_size_is_too_large(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_init(
            (void *)1, SIZE_MAX, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_SIZE_IS_TOO_LARGE,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init_error_on_compare_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_init((void *)1, 1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_COMPARE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_init(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    assert_int_equal(object.size, sizeof(uintptr_t));
    assert_ptr_equal(object.compare, compare_uintptr_t);
    assert_null(object.list);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_size_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_size(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_size_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_size((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_size(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object = {};
    object.size = (rand() % UINTMAX_MAX);
    uintmax_t size;
    assert_true(rock_linked_red_black_tree_set_size(&object, &size));
    assert_int_equal(size, object.size);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_count(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_count((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_count(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object = {};
    object.tree.count = (rand() % UINTMAX_MAX);
    uintmax_t count;
    assert_true(rock_linked_red_black_tree_set_count(&object, &count));
    assert_int_equal(object.tree.count, count);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_add(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_add((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t value = (rand() % UINTPTR_MAX);
    posix_memalign_is_overridden = true;
    assert_false(rock_linked_red_black_tree_set_add(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    posix_memalign_is_overridden = false;
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    assert_int_equal(0, object.tree.count);
    assert_null(object.tree.root);
    assert_null(object.list);
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_int_equal(1, object.tree.count);
    assert_non_null(object.tree.root);
    assert_non_null(object.list);
    assert_ptr_equal(object.list, object.list->next);
    value += (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_int_equal(2, object.tree.count);
    assert_ptr_not_equal(object.list, object.list->next);
    assert_ptr_equal(object.list, object.list->next->next);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_add_error_on_item_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    assert_int_equal(0, object.tree.count);
    assert_null(object.tree.root);
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, (void*)&value));
    assert_int_equal(1, object.tree.count);
    assert_non_null(object.tree.root);
    assert_non_null(object.list);
    assert_ptr_equal(object.list, object.list->next);
    assert_false(rock_linked_red_black_tree_set_add(&object, (void*)&value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_ALREADY_EXISTS,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_remove(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_remove((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_error_on_item_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    for (uintmax_t i = 0; i < 10; i++, value++) {
        assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    }
    assert_false(rock_linked_red_black_tree_set_remove(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    assert_int_equal(0, object.tree.count);
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_int_equal(1, object.tree.count);
    assert_non_null(object.list);
    assert_true(rock_linked_red_black_tree_set_remove(&object, &value));
    assert_int_equal(0, object.tree.count);
    assert_null(object.list);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_contains(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_contains(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_contains(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_contains(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    bool result;
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_contains(
            &object, &value, &result));
    assert_false(result);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_contains(
            &object, &value, &result));
    assert_true(result);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_get(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_get(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_get(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_get_error_on_item_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_false(rock_linked_red_black_tree_set_get(
            &object, &value, (const void **)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_get(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_get(
            &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_ceiling_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_ceiling(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_ceiling_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_ceiling(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_ceiling_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_ceiling(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_ceiling_error_on_item_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    assert_false(rock_linked_red_black_tree_set_ceiling(
            &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    value++;
    assert_false(rock_linked_red_black_tree_set_ceiling(
                &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_ceiling(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_ceiling(
            &object, &value, (const void **)&item));
    assert_int_equal(*item, value);
    uintptr_t check = 187;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    value = 101;
    assert_true(rock_linked_red_black_tree_set_ceiling(
                &object, &value, (const void **)&item));
    assert_int_equal(*item, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_floor_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_floor(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_floor_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_floor(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_floor_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_floor(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_floor_error_on_item_not_found(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    assert_false(rock_linked_red_black_tree_set_floor(
            &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    value--;
    assert_false(rock_linked_red_black_tree_set_floor(
                &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_floor(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_floor(
            &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    uintptr_t check = 87;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    value = 99;
    assert_true(rock_linked_red_black_tree_set_floor(
                &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_higher_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_higher(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_higher_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_higher(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_higher_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_higher(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_higher_error_on_item_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_false(rock_linked_red_black_tree_set_higher(
            &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_false(rock_linked_red_black_tree_set_higher(
                &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_higher(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                             sizeof(uintptr_t),
                                             compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    uintptr_t check = 234;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    assert_true(rock_linked_red_black_tree_set_higher(
            &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    value = 20;
    check = 100;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    assert_true(rock_linked_red_black_tree_set_higher(
                &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    value = 100;
    check = 234;
    assert_true(rock_linked_red_black_tree_set_higher(
                &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_lower_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_lower(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_lower_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_lower(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_lower_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_lower(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_lower_error_on_item_not_found(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    const uintptr_t value = (rand() % UINTPTR_MAX);
    assert_false(rock_linked_red_black_tree_set_lower(
            &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_false(rock_linked_red_black_tree_set_lower(
                &object, &value, (const void**)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_NOT_FOUND,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_lower(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *item;
    uintptr_t value = 100;
    uintptr_t check = 20;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    assert_true(rock_linked_red_black_tree_set_lower(
            &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    value = 234;
    check = 100;
    assert_true(rock_linked_red_black_tree_set_add(&object, &check));
    assert_true(rock_linked_red_black_tree_set_lower(
                &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    value = 100;
    check = 20;
    assert_true(rock_linked_red_black_tree_set_lower(
                &object, &value, (const void **)&item));
    assert_ptr_not_equal(item, &check);
    assert_int_equal(*item, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_first(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_first((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_first_error_on_empty_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t *first;
    assert_false(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_SET_IS_EMPTY,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_first(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t check = (rand() % UINTPTR_MAX);
    uintptr_t value = check;
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintptr_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    assert_ptr_not_equal(first, &value);
    assert_int_equal(*first, check);
    value += (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    assert_ptr_not_equal(first, &value);
    assert_int_equal(*first, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_last(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_last((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_last_error_on_empty_set(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    const uintptr_t* last;
    assert_false(rock_linked_red_black_tree_set_last(
            &object, (const void **)&last));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_SET_IS_EMPTY,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_last(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* last;
    assert_true(rock_linked_red_black_tree_set_last(
            &object, (const void **)&last));
    assert_ptr_not_equal(last, &value);
    assert_int_equal(*last, value);
    value += (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_last(
            &object, (const void **)&last));
    assert_ptr_not_equal(last, &value);
    assert_int_equal(*last, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_item_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_remove_item(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_item_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_remove_item((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_remove_item(void **state) {
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                             sizeof(uintptr_t),
                                             compare_uintptr_t));
    uintmax_t value = 10;
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t *item;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **) &item));
    uintmax_t count;
    assert_true(rock_linked_red_black_tree_set_count(&object, &count));
    assert_int_equal(count, 1);
    assert_true(rock_linked_red_black_tree_set_remove_item(&object, item));
    assert_true(rock_linked_red_black_tree_set_count(&object, &count));
    assert_int_equal(count, 0);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_next(
            NULL, (void *)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_next(
            (void *)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_next(
            (void *)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_next_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* item;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&item));
    assert_false(rock_linked_red_black_tree_set_next(
            &object, item, (const void **)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_END_OF_SEQUENCE,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_next(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* item;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&item));
    value -= (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_true(rock_linked_red_black_tree_set_next(
            &object, item, (const void **)&item));
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_prev(
            NULL, (void*)1, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_prev(
            (void*)1, NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_out_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_prev(
            (void*)1, (void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OUT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev_error_on_end_of_sequence(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* item;
    assert_true(rock_linked_red_black_tree_set_last(
            &object, (const void **)&item));
    assert_false(rock_linked_red_black_tree_set_prev(
            &object, item, (const void **)&item));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_END_OF_SEQUENCE,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prev(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t check = (rand() % UINTPTR_MAX);
    uintptr_t value = check;
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    value -= (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* item;
    assert_true(rock_linked_red_black_tree_set_last(
            &object, (const void **)&item));
    assert_true(rock_linked_red_black_tree_set_prev(
            &object, item, (const void **)&item));
    assert_int_equal(*item, check);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_after(
            NULL, (void*)1, (void*)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_after(
            (void*)1, NULL,(void*)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_after(
            (void*)1, (void*)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    value += (rand() % UINTPTR_MAX);
    posix_memalign_is_overridden = true;
    assert_false(rock_linked_red_black_tree_set_insert_after(
            &object, first, &value));
    posix_memalign_is_overridden = false;
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    value += (rand() % UINTPTR_MAX);
    const uintptr_t* item;
    assert_true(rock_linked_red_black_tree_set_insert_after(
            &object, first, &value));
    assert_true(rock_linked_red_black_tree_set_next(
            &object, first, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_after_error_on_value_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    assert_false(rock_linked_red_black_tree_set_insert_after(
            &object, first, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_ALREADY_EXISTS,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_before(
            NULL, (void*)1, (void*)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before_error_on_item_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_before(
            (void*)1, NULL,(void*)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_ITEM_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_insert_before(
            (void*)1, (void*)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    value += (rand() % UINTPTR_MAX);
    posix_memalign_is_overridden = true;
    assert_false(rock_linked_red_black_tree_set_insert_before(
            &object, first, &value));
    posix_memalign_is_overridden = false;
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    value += (rand() % UINTPTR_MAX);
    const uintptr_t* item;
    assert_true(rock_linked_red_black_tree_set_insert_before(
            &object, first, &value));
    assert_true(rock_linked_red_black_tree_set_prev(
            &object, first, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_insert_before_error_on_value_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    const uintmax_t* first;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&first));
    assert_false(rock_linked_red_black_tree_set_insert_before(
            &object, first, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_ALREADY_EXISTS,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_append_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_append(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_append_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_append((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_append_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    posix_memalign_is_overridden = true;
    assert_false(rock_linked_red_black_tree_set_append(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    posix_memalign_is_overridden = false;
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_append(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_append(&object, &value));
    const uintptr_t *item;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    value += (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_append(&object, &value));
    assert_true(rock_linked_red_black_tree_set_next(
            &object, item, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_append_error_on_value_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_false(rock_linked_red_black_tree_set_append(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_ALREADY_EXISTS,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prepend_error_on_object_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_prepend(NULL, (void *)1));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_OBJECT_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prepend_error_on_value_is_null(void **state) {
    rock_error = ROCK_ERROR_NONE;
    assert_false(rock_linked_red_black_tree_set_prepend((void *)1, NULL));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_IS_NULL,
                     rock_error);
    rock_error = ROCK_ERROR_NONE;
}

static void check_prepend_error_on_memory_allocation_failed(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    posix_memalign_is_overridden = true;
    assert_false(rock_linked_red_black_tree_set_prepend(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_MEMORY_ALLOCATION_FAILED,
                     rock_error);
    posix_memalign_is_overridden = false;
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prepend(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_prepend(&object, &value));
    const uintptr_t *item;
    assert_true(rock_linked_red_black_tree_set_first(
            &object, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    value += (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_prepend(&object, &value));
    assert_true(rock_linked_red_black_tree_set_prev(
            &object, item, (const void **)&item));
    assert_ptr_not_equal(item, &value);
    assert_int_equal(*item, value);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

static void check_prepend_error_on_value_already_exists(void **state) {
    srand(time(NULL));
    rock_error = ROCK_ERROR_NONE;
    struct rock_linked_red_black_tree_set object;
    assert_true(rock_linked_red_black_tree_set_init(&object,
                                                    sizeof(uintptr_t),
                                                    compare_uintptr_t));
    uintptr_t value = (rand() % UINTPTR_MAX);
    assert_true(rock_linked_red_black_tree_set_add(&object, &value));
    assert_false(rock_linked_red_black_tree_set_prepend(&object, &value));
    assert_int_equal(ROCK_LINKED_RED_BLACK_TREE_SET_ERROR_VALUE_ALREADY_EXISTS,
                     rock_error);
    assert_true(rock_linked_red_black_tree_set_invalidate(&object, NULL));
    rock_error = ROCK_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_size_is_zero),
            cmocka_unit_test(check_init_error_on_size_is_too_large),
            cmocka_unit_test(check_init_error_on_compare_is_null),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_size_error_on_object_is_null),
            cmocka_unit_test(check_size_error_on_out_is_null),
            cmocka_unit_test(check_size),
            cmocka_unit_test(check_count_error_on_object_is_null),
            cmocka_unit_test(check_count_error_on_out_is_null),
            cmocka_unit_test(check_count),
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add_error_on_value_is_null),
            cmocka_unit_test(check_add_error_on_memory_allocation_failed),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_on_item_already_exists),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_value_is_null),
            cmocka_unit_test(check_remove_error_on_item_not_found),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_contains_error_on_object_is_null),
            cmocka_unit_test(check_contains_error_on_value_is_null),
            cmocka_unit_test(check_contains_error_on_out_is_null),
            cmocka_unit_test(check_contains),
            cmocka_unit_test(check_get_error_on_object_is_null),
            cmocka_unit_test(check_get_error_on_value_is_null),
            cmocka_unit_test(check_get_error_on_out_is_null),
            cmocka_unit_test(check_get_error_on_item_not_found),
            cmocka_unit_test(check_get),
            cmocka_unit_test(check_ceiling_error_on_object_is_null),
            cmocka_unit_test(check_ceiling_error_on_value_is_null),
            cmocka_unit_test(check_ceiling_error_on_out_is_null),
            cmocka_unit_test(check_ceiling_error_on_item_not_found),
            cmocka_unit_test(check_ceiling),
            cmocka_unit_test(check_floor_error_on_object_is_null),
            cmocka_unit_test(check_floor_error_on_value_is_null),
            cmocka_unit_test(check_floor_error_on_out_is_null),
            cmocka_unit_test(check_floor_error_on_item_not_found),
            cmocka_unit_test(check_floor),
            cmocka_unit_test(check_higher_error_on_object_is_null),
            cmocka_unit_test(check_higher_error_on_value_is_null),
            cmocka_unit_test(check_higher_error_on_out_is_null),
            cmocka_unit_test(check_higher_error_on_item_not_found),
            cmocka_unit_test(check_higher),
            cmocka_unit_test(check_lower_error_on_object_is_null),
            cmocka_unit_test(check_lower_error_on_value_is_null),
            cmocka_unit_test(check_lower_error_on_out_is_null),
            cmocka_unit_test(check_lower_error_on_item_not_found),
            cmocka_unit_test(check_lower),
            cmocka_unit_test(check_first_error_on_object_is_null),
            cmocka_unit_test(check_first_error_on_out_is_null),
            cmocka_unit_test(check_first_error_on_empty_set),
            cmocka_unit_test(check_first),
            cmocka_unit_test(check_last_error_on_object_is_null),
            cmocka_unit_test(check_last_error_on_out_is_null),
            cmocka_unit_test(check_last_error_on_empty_set),
            cmocka_unit_test(check_last),
            cmocka_unit_test(check_remove_item_error_on_object_is_null),
            cmocka_unit_test(check_remove_item_error_on_item_is_null),
            cmocka_unit_test(check_remove_item),
            cmocka_unit_test(check_next_error_on_object_is_null),
            cmocka_unit_test(check_next_error_on_item_is_null),
            cmocka_unit_test(check_next_error_on_out_is_null),
            cmocka_unit_test(check_next_error_on_end_of_sequence),
            cmocka_unit_test(check_next),
            cmocka_unit_test(check_prev_error_on_object_is_null),
            cmocka_unit_test(check_prev_error_on_item_is_null),
            cmocka_unit_test(check_prev_error_on_out_is_null),
            cmocka_unit_test(check_prev_error_on_end_of_sequence),
            cmocka_unit_test(check_prev),
            cmocka_unit_test(check_insert_after_error_on_object_is_null),
            cmocka_unit_test(check_insert_after_error_on_item_is_null),
            cmocka_unit_test(check_insert_after_error_on_value_is_null),
            cmocka_unit_test(check_insert_after_error_on_memory_allocation_failed),
            cmocka_unit_test(check_insert_after),
            cmocka_unit_test(check_insert_after_error_on_value_already_exists),
            cmocka_unit_test(check_insert_before_error_on_object_is_null),
            cmocka_unit_test(check_insert_before_error_on_item_is_null),
            cmocka_unit_test(check_insert_before_error_on_value_is_null),
            cmocka_unit_test(check_insert_before_error_on_memory_allocation_failed),
            cmocka_unit_test(check_insert_before),
            cmocka_unit_test(check_insert_before_error_on_value_already_exists),
            cmocka_unit_test(check_append_error_on_object_is_null),
            cmocka_unit_test(check_append_error_on_value_is_null),
            cmocka_unit_test(check_append_error_on_memory_allocation_failed),
            cmocka_unit_test(check_append),
            cmocka_unit_test(check_append_error_on_value_already_exists),
            cmocka_unit_test(check_prepend_error_on_object_is_null),
            cmocka_unit_test(check_prepend_error_on_value_is_null),
            cmocka_unit_test(check_prepend_error_on_memory_allocation_failed),
            cmocka_unit_test(check_prepend),
            cmocka_unit_test(check_prepend_error_on_value_already_exists),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
