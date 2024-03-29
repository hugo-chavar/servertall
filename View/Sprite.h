#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <iostream>
#include <vector>
#include <list>
#include "Surface.h"
#include "AnimatedEntity.h"

class Sprite {

protected:

	int relx,rely,_baseWidth,_baseHeight;
	EntityObject* spriteEntity;
	unsigned currentSurfaceNumber; 
	int surfacesCount;
	int surfaceWidth;
	int surfaceHeight;
	//std::vector<view::Surface* > surfaces;
	virtual void loadSurfaces();
	virtual void initialize();

public:
	Sprite(EntityObject *entity);
	Sprite();
	~Sprite();
	virtual void updateFrame();
	int relatx();
	int relaty();
	int baseWidth();
	int baseHeight();
	//int getCurrentState();
	int getSurfaceWidth();
	int getSurfaceHeight();
	unsigned getCurrentSurfaceNumber();
	void setCurrentSurfaceNumber(unsigned surfaceNumber);
	void restart();	
	//view::Surface* getSurfaceAt(int state);
	//view::Surface* getCurrentSurface();
	//void addSurface(std::string path);
};

#endif