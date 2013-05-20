#pragma once
#include "Sprite.h"

class SpriteAnimado: public Sprite {

private:
	float delay;
	float fps;
	float accumulatedTime;
	void advance();
	bool timeIsOver();

	void loadSurfaces();
public:
	void initialize();
	void updateFrame();
	//void getNextFrame();
	SpriteAnimado(AnimatedEntity* entity);
	bool lastFrame();
	void restart();
	~SpriteAnimado();
	void addSticks(float ticks);
	void setCurrentState(unsigned state);
};

