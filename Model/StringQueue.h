#pragma once
#include "Mutex.h"
#include <string>
#include<queue>
class StringQueue
{
private:
	Mutex mutex;

	std::queue<std::string> string_queue;

public:
	StringQueue();

	void push(std::string _string);

	std::string pop();

	int size();

	void lock();

	void unLock();

	~StringQueue();
};

