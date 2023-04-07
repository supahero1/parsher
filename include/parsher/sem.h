#ifndef _parsher_sem_h_
#define _parsher_sem_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STDC_NO_THREADS__
#error This compiler does not support threads.h.
#endif

#include <stdint.h>
#include <threads.h>


typedef struct {
	mtx_t mutex;

	cnd_t cond;

	uint64_t value;
} sem_t;


extern void
sem_init(sem_t* sem);


extern void
sem_post(sem_t* sem, uint64_t value);


extern uint64_t
sem_wait(sem_t* sem);


extern void
sem_destroy(sem_t* sem);


#ifdef __cplusplus
}
#endif

#endif
