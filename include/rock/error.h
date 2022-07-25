#ifndef _ROCK_ERROR_H_
#define _ROCK_ERROR_H_

#include <stddef.h>
#include <threads.h>

#define ROCK_ERROR_NONE                                 0

extern thread_local size_t rock_error;

#endif /* _ROCK_ERROR_H_ */
