#include "StringQueue.h"

StringQueue::StringQueue()
{

}

void StringQueue::push(std::string _string)
{
	this->mutex.lock();
		this->string_queue.push(_string);
	this->mutex.unlock();
}

std::string StringQueue::pop()
{
	std::string str = this->string_queue.front();
	this->string_queue.pop();
	return  str;
}

StringQueue::~StringQueue()
{

}

int StringQueue::size()
{
	return this->string_queue.size();
}

void StringQueue::unLock() {
	this->mutex.unlock();
}

void StringQueue::lock() {
	this->mutex.lock();
}