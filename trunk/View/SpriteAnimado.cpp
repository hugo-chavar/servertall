#include "SpriteAnimado.h"
#include "GameView.h"

SpriteAnimado::SpriteAnimado(AnimatedEntity* entity) {
	surfacesCount = 0;
	spriteEntity = entity;
	delay = static_cast<float>(entity->delay()); 
	fps = static_cast<float>(entity->fps());
	this->initialize();
	this->accumulatedTime = 0;
}

SpriteAnimado::~SpriteAnimado() {
}

void SpriteAnimado::initialize() {
	this->restart();
	this->relx = spriteEntity->pixelRefX();
	this->rely = spriteEntity->pixelRefY();
	this->_baseWidth = spriteEntity->baseWidth();
	this->_baseHeight = spriteEntity->baseHeight();
	this->loadSurfaces();
}

void SpriteAnimado::updateFrame() {
	float deltaTime = GameView::instance().getTimer()->getDeltaTime();
	this->addSticks(deltaTime); 
	if ( this->timeIsOver())
		this->advance();
}

bool SpriteAnimado::lastFrame() {
	return (static_cast<int>(this->getCurrentSurfaceNumber()) >= (surfacesCount - 1));
}

void SpriteAnimado::restart() {
	this->currentSurfaceNumber = 0;
}

void SpriteAnimado::advance() {
	this->accumulatedTime -= ((1000/fps) + this->getDelay());
	if ( this->lastFrame() )
		this->restart();
	else
		this->currentSurfaceNumber++;
}

bool SpriteAnimado::timeIsOver() {
	return (this->accumulatedTime >= (1000/fps) - this->getDelay());
}

void SpriteAnimado::loadSurfaces() {
	AnimatedEntity* auxEntity = (AnimatedEntity*)spriteEntity;
	surfacesCount = auxEntity->imagesPaths()->count();
}

void SpriteAnimado::addSticks(float ticks) {
	this->accumulatedTime += ticks;
}

float SpriteAnimado::getDelay() {
	if (this->getCurrentSurfaceNumber() == 0)
		return this->delay;
	return 0;
}

void SpriteAnimado::setAccumulatedTime(float ticks) {
	this->accumulatedTime = ticks;
}