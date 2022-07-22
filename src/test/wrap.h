#ifndef _ROCK_TEST_WRAP_H_
#define _ROCK_TEST_WRAP_H_

#ifdef TEST
#include <stdbool.h>

extern bool pthread_rwlock_init_is_overridden;
int __wrap_pthread_rwlock_init(void *rwlock, const void *attr);

extern bool pthread_rwlock_destroy_is_overridden;
int __wrap_pthread_rwlock_destroy(void *rwlock);

extern bool pthread_rwlock_rdlock_is_overridden;
int __wrap_pthread_rwlock_rdlock(void *rwlock);

extern bool pthread_rwlock_wrlock_is_overridden;
int __wrap_pthread_rwlock_wrlock(void *rwlock);

extern bool pthread_rwlock_unlock_is_overridden;
int __wrap_pthread_rwlock_unlock(void *rwlock);

extern bool pthread_mutex_init_is_overridden;
int __wrap_pthread_mutex_init(void *mutex, void *attr);

extern bool pthread_mutex_destroy_is_overridden;
int __wrap_pthread_mutex_destroy(void *mutex);

extern bool pthread_mutex_lock_is_overridden;
int __wrap_pthread_mutex_lock(void *mutex);

extern bool pthread_mutex_unlock_is_overridden;
int __wrap_pthread_mutex_unlock(void *mutex);

extern bool pthread_cond_init_is_overridden;
int __wrap_pthread_cond_init(void *cond, void *attr);

extern bool pthread_cond_destroy_is_overridden;
int __wrap_pthread_cond_destroy(void *cond);

extern bool pthread_cond_wait_is_overridden;
int __wrap_pthread_cond_wait(void *cond, void *mutex);

extern bool pthread_cond_signal_is_overridden;
int __wrap_pthread_cond_signal(void *cond);

extern bool pthread_cond_broadcast_is_overridden;
int __wrap_pthread_cond_broadcast(void *cond);

extern bool nanosleep_is_overridden;
int __wrap_nanosleep(void *req, void *rem);
#endif /* TEST */

#endif /* _ROCK_TEST_WRAP_H_ */