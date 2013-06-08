#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "pthread.h"

class Mutex{
private:
	pthread_mutex_t mutex;

public:
	Mutex();

	pthread_mutex_t& getMutex();

	int lock();

	bool tryLock();

	int unlock();

	~Mutex();
};

#endif // _MUTEX_H_
