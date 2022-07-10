#ifndef _CORAL_TEST_CMOCKA_H_
#define _CORAL_TEST_CMOCKA_H_

#ifdef TEST
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

extern void* _test_malloc(size_t size, const char* file, int line);
#define malloc(size) _test_malloc(size, __FILE__, __LINE__)

extern void* _test_calloc(size_t nmemb, size_t size, const char* file, int line);
#define calloc(nmemb, size) _test_calloc(nmemb, size, __FILE__, __LINE__)

extern void* _test_realloc(void* ptr, size_t size, const char* file, int line);
#define realloc(ptr, size) _test_realloc(ptr, size, __FILE__, __LINE__)

extern void _test_free(void* ptr, const char* file, int line);
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
#endif //TEST

#endif /* _CORAL_TEST_CMOCKA_H_ */
