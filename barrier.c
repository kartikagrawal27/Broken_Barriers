/**
 * Parallelized Poisson Problems
 * CS 241 - Spring 2016
 */
#include "barrier.h"

// The returns are just for errors if you want to check for them.
int barrier_destroy(barrier_t *t) 
{
	int errorval;
  	errorval=pthread_mutex_destroy(&t->mtx);
  	if(errorval)
  		return errorval;
 	errorval=pthread_cond_destroy(&t->cv);
  	if(errorval)
  		return errorval;
  	
  	return 0;
}

int barrier_init(barrier_t *t, unsigned n) 
{

  	t->count = 0;
  	t->times_used = 0;
  	t->n_threads = n;
	int errorval;
 	errorval=pthread_mutex_init(&t->mtx, NULL);
  	if(errorval)
  		return errorval;
 	errorval=pthread_cond_init(&t->cv,NULL);
  	if(errorval)
  		return errorval;

  return 0;
}

int barrier_wait(barrier_t *t)
{
	pthread_mutex_lock(&t->mtx);
		
	unsigned int new_n_threads = t->times_used;
		
	t->count++;
	
	if(t->count == t->n_threads)
	{
		t->times_used++;
		t->count=0;
		pthread_cond_broadcast(&t->cv);
	}
	else
	{
		while(t->times_used == new_n_threads)
			pthread_cond_wait(&t->cv, &t->mtx);
	}	
	
	pthread_mutex_unlock(&t->mtx);
	
	return 0;
}
