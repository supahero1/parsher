#include <parsher/sem.h>
#include <parsher/except.h>


void
sem_init(sem_t* sem)
{
	sem->value = 0;

	if(mtx_init(&sem->mutex, mtx_plain) != thrd_success)
	{
		psh_nomem();
	}

	if(cnd_init(&sem->cond) != thrd_success)
	{
		psh_nomem();
	}
}


void
sem_post(sem_t* sem, uint64_t value)
{
	if(!value)
	{
		return;
	}

	(void) mtx_lock(&sem->mutex);

	/**
	 * Never use this with more than 2 threads.
	 * This has specifically been tweaked for
	 * the requirements of this library.
	 */

	sem->value += value;

	(void) cnd_signal(&sem->cond);

	(void) mtx_unlock(&sem->mutex);
}


uint64_t
sem_wait(sem_t* sem)
{
	(void) mtx_lock(&sem->mutex);

	while(sem->value == 0)
	{
		(void) cnd_wait(&sem->cond, &sem->mutex);
	}

	uint64_t ret = sem->value;

	sem->value = 0;

	(void) mtx_unlock(&sem->mutex);

	return ret;
}


void
sem_destroy(sem_t* sem)
{
	mtx_destroy(&sem->mutex);
	cnd_destroy(&sem->cond);
}
