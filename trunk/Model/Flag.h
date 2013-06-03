#pragma once
#ifndef __FLAG_H__
#define __FLAG_H__

#include "AnimatedEntity.h"


class Flag {

public:
	Flag(int xPosition, int yPosition);
	~Flag();
	void hurt(float damage);
	float life();

private:
	AnimatedEntity* entity;
	float _life;
	pair <int,int> position;

};


#endif