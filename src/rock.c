#include <stdlib.h>
#include <rock.h>

void rock_required(const void *object) {
    rock_required_true(NULL != object);
}

void rock_required_true(const int check) {
    if (!check) {
        abort();
    }
}

int rock_compare_void_ptr(const void *a, const void *b) {
    if (a == b) {
        return 0;
    } else if (a > b) {
        return 1;
    }
    return (-1);
}

int rock_compare_size_t_ptr(const void *a, const void *b) {
    return rock_compare_size_t((const void *) *(size_t *) a,
                               (const void *) *(size_t *) b);
}

int rock_compare_size_t(const void *a, const void *b) {
    const size_t A = (size_t) a;
    const size_t B = (size_t) b;
    if (A == B) {
        return 0;
    } else if (A > B) {
        return 1;
    }
    return (-1);
}

bool rock_add_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        rock_error = ROCK_ERROR_OUT_IS_NULL;
        return false;
    }
    const size_t result = a + b;
    if (result < a) {
        rock_error = ROCK_ERROR_OVERFLOW_OCCURRED;
        return false;
    }
    *out = result;
    return true;
}

bool rock_multiply_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        rock_error = ROCK_ERROR_OUT_IS_NULL;
        return false;
    }
    if (!a || !b) {
        *out = 0;
        return true;
    }
    const size_t result = a * b;
    if (result / a != b) {
        rock_error = ROCK_ERROR_OVERFLOW_OCCURRED;
        return false;
    }
    *out = result;
    return true;
}

bool rock_minimum_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        rock_error = ROCK_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = a < b ? a : b;
    return true;
}

bool rock_maximum_size_t(const size_t a, const size_t b, size_t *out) {
    if (!out) {
        rock_error = ROCK_ERROR_OUT_IS_NULL;
        return false;
    }
    *out = a > b ? a : b;
    return true;
}

