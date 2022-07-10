#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "wrap.h"

bool pthread_rwlock_init_is_overridden = false;
extern int __real_pthread_rwlock_init(void *rwlock, const void *attr);
int __wrap_pthread_rwlock_init(void *rwlock, const void *attr) {
    if (!pthread_rwlock_init_is_overridden) {
        return __real_pthread_rwlock_init(rwlock, attr);
    }
    return mock_type(int);
}

bool pthread_rwlock_destroy_is_overridden = false;
extern int __real_pthread_rwlock_destroy(void *rwlock);
int __wrap_pthread_rwlock_destroy(void *rwlock) {
    if (!pthread_rwlock_destroy_is_overridden) {
        return __real_pthread_rwlock_destroy(rwlock);
    }
    return mock_type(int);
}

bool pthread_rwlock_rdlock_is_overridden = false;
extern int __real_pthread_rwlock_rdlock(void *rwlock);
int __wrap_pthread_rwlock_rdlock(void *rwlock) {
    if (!pthread_rwlock_rdlock_is_overridden) {
        return __real_pthread_rwlock_rdlock(rwlock);
    }
    return mock_type(int);
}

bool pthread_rwlock_wrlock_is_overridden = false;
extern int __real_pthread_rwlock_wrlock(void *rwlock);
int __wrap_pthread_rwlock_wrlock(void *rwlock) {
    if (!pthread_rwlock_wrlock_is_overridden) {
        return __real_pthread_rwlock_wrlock(rwlock);
    }
    return mock_type(int);
}

bool pthread_rwlock_unlock_is_overridden = false;
extern int __real_pthread_rwlock_unlock(void *rwlock);
int __wrap_pthread_rwlock_unlock(void *rwlock) {
    if (!pthread_rwlock_unlock_is_overridden) {
        return __real_pthread_rwlock_unlock(rwlock);
    }
    return mock_type(int);
}

bool pthread_mutex_init_is_overridden = false;
extern int __real_pthread_mutex_init(void *mutex, void *attr);
int __wrap_pthread_mutex_init(void *mutex, void *attr) {
    if (!pthread_mutex_init_is_overridden) {
        return __real_pthread_mutex_init(mutex, attr);
    }
    return mock_type(int);
}

bool pthread_mutex_destroy_is_overridden = false;
extern int __real_pthread_mutex_destroy(void *mutex);
int __wrap_pthread_mutex_destroy(void *mutex) {
    if (!pthread_mutex_destroy_is_overridden) {
        return __real_pthread_mutex_destroy(mutex);
    }
    return mock_type(int);
}

bool pthread_mutex_lock_is_overridden = false;
extern int __real_pthread_mutex_lock(void *mutex);
int __wrap_pthread_mutex_lock(void *mutex) {
    if (!pthread_mutex_lock_is_overridden) {
        return __real_pthread_mutex_lock(mutex);
    }
    return mock_type(int);
}

bool pthread_mutex_unlock_is_overridden = false;
extern int __real_pthread_mutex_unlock(void *mutex);
int __wrap_pthread_mutex_unlock(void *mutex) {
    if (!pthread_mutex_unlock_is_overridden) {
        return __real_pthread_mutex_unlock(mutex);
    }
    return mock_type(int);
}

bool pthread_cond_init_is_overridden = false;
extern int __real_pthread_cond_init(void *cond, void *attr);
int __wrap_pthread_cond_init(void *cond, void *attr) {
    if (!pthread_cond_init_is_overridden) {
        return __real_pthread_mutex_init(cond, attr);
    }
    return mock_type(int);
}

bool pthread_cond_destroy_is_overridden = false;
extern int __real_pthread_cond_destroy(void *mutex);
int __wrap_pthread_cond_destroy(void *mutex) {
    if (!pthread_cond_destroy_is_overridden) {
        return __real_pthread_cond_destroy(mutex);
    }
    return mock_type(int);
}

bool pthread_cond_wait_is_overridden = false;
extern int __real_pthread_cond_wait(void *cond, void *mutex);
int __wrap_pthread_cond_wait(void *cond, void *mutex) {
    if (!pthread_cond_wait_is_overridden) {
        return __real_pthread_cond_wait(cond, mutex);
    }
    return mock_type(int);
}

bool pthread_cond_signal_is_overridden = false;
extern int __real_pthread_cond_signal(void *cond);
int __wrap_pthread_cond_signal(void *cond) {
    if (!pthread_cond_signal_is_overridden) {
        return __real_pthread_cond_signal(cond);
    }
    return mock_type(int);
}

bool pthread_cond_broadcast_is_overridden = false;
extern int __real_pthread_cond_broadcast(void *cond);
int __wrap_pthread_cond_broadcast(void *cond) {
    if (!pthread_cond_broadcast_is_overridden) {
        return __real_pthread_cond_broadcast(cond);
    }
    return mock_type(int);
}

bool nanosleep_is_overridden = false;
extern int __real_nanosleep(void *req, void* rem);
int __wrap_nanosleep(void *req, void *rem) {
    if (!nanosleep_is_overridden) {
        return __real_nanosleep(req, rem);
    }
    return mock_type(int);
}