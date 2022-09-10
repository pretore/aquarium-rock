#include <stdlib.h>
#include <errno.h>
#include "cmocka.h"

int _test_posix_memalign(void **const out, const size_t alignment,
                         const size_t size,
                         const char *const file, const int line) {
    if (alignment % sizeof(void *)) {
        return EINVAL;
    }
    { /* check for power of 2 */
        size_t i = alignment - 1;
        do { /* expecting bit pattern of consecutive ones */
            if (!(i & 1)) {
                return EINVAL;
            }
            i >>= 1;
        } while (i);
    }
    void *const ptr = _test_malloc(size, file, line);
    if (!ptr) {
        return ENOMEM;
    }
    *out = ptr;
    return 0;
}
