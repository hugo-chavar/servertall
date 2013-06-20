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
	SpriteAnimado(AnimatedEntity* entity);
	bool lastFrame();
	void restart();
	~SpriteAnimado();
	void addSticks(float ticks);
	float getDelay();
	void setAccumulatedTime(float ticks);
};

