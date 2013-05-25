#include "InstructionQueue.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

InstructionQueue::InstructionQueue() {
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

Mutex& InstructionQueue::getMutex() {
	return this->mutex;
}

ConditionVariable& InstructionQueue::getConditionVariable() {
	return this->conditionVariable;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

void InstructionQueue::addInstruction(Instruction& instruction) {
	this->getMutex().lock();
	this->getInstructionQueue().push(instruction);
	this->getMutex().unlock();
	this->getConditionVariable().getConditionMutex().lock();
	this->getConditionVariable().signal();
	this->getConditionVariable().getConditionMutex().unlock();
}

Instruction InstructionQueue::getNextInstruction(bool shouldWait) {
	Instruction aux;

	this->getMutex().lock();
	if (this->getInstructionQueue().empty() && shouldWait) {
		this->getConditionVariable().getConditionMutex().lock();
		this->getMutex().unlock();
		this->getConditionVariable().wait();
		this->getMutex().lock();
		this->getConditionVariable().getConditionMutex().unlock();
	}

	if (!this->getInstructionQueue().empty()) {
		aux = this->getInstructionQueue().front();
		this->getInstructionQueue().pop();
	}
	this->getMutex().unlock();

	return aux;
}

void InstructionQueue::lock() {
	this->getMutex().lock();
}

std::queue<Instruction>& InstructionQueue::getInstructionQueue() {
	return this->instructionQueue;
}

void InstructionQueue::unLock() {
	this->getMutex().unlock();
}

void InstructionQueue::stopWaiting() {
	this->getConditionVariable().getConditionMutex().lock();
	this->getConditionVariable().signal();
	this->getConditionVariable().getConditionMutex().unlock();
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

InstructionQueue::~InstructionQueue() {
}
