#ifndef _ROCK_ROCK_H_
#define _ROCK_ROCK_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <rock/array.h>
#include <rock/error.h>
#include <rock/forward_list.h>
#include <rock/linked_list.h>
#include <rock/red_black_tree.h>

/**
 * @brief Return the container of the given pointer.
 * @param [in] ptr to the embedded member.
 * @param [in] type of the container.
 * @param [in] member of the container that ptr refers to.
 */
#define rock_container_of(ptr, type, member) (\
            (type *)((char *)ptr - offsetof(type,member)))

#endif /* _ROCK_ROCK_H_ */
