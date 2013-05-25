#include "ConditionVariable.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

ConditionVariable::ConditionVariable() {
	pthread_cond_init(&(this->conditionVariable),NULL);
	this->broadcastId = 1;
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

void ConditionVariable::setBroadcastId(unsigned int broadcastId) {
	if (broadcastId == 0)
		broadcastId++;
	this->broadcastId = broadcastId;
}

pthread_cond_t& ConditionVariable::getConditionVariable() {
	return this->conditionVariable;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

Mutex& ConditionVariable::getConditionMutex() {
	return this->conditionMutex;
}

int ConditionVariable::wait() {
	return pthread_cond_wait(&(this->getConditionVariable()),&(this->getConditionMutex().getMutex()));
}

unsigned int ConditionVariable::getBroadcastId() {
	return this->broadcastId;
}

unsigned int ConditionVariable::getNextBroadcastId() {
	unsigned int nextBroadcastId;

	this->getConditionMutex().lock();
	nextBroadcastId = this->getBroadcastId();
	this->getConditionMutex().unlock();
	return nextBroadcastId;
}

int ConditionVariable::broadcast() {
	int status = 0;

	this->getConditionMutex().lock();
	status = pthread_cond_broadcast(&(this->getConditionVariable()));
	this->setBroadcastId(this->getBroadcastId() + 1);
	this->getConditionMutex().unlock();

	return status;
}

int ConditionVariable::signal() {
	return pthread_cond_signal(&(this->getConditionVariable()));
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

ConditionVariable::~ConditionVariable() {
	pthread_cond_destroy(&(this->conditionVariable));
}