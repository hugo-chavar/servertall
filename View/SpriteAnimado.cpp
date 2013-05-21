#include "SpriteAnimado.h"
#include "GameView.h"

SpriteAnimado::SpriteAnimado(AnimatedEntity* entity) {
	//this->inicializar(entity->pixelRefX(),entity->pixelRefY(),entity->baseWidth(),entity->baseHeight());
	surfacesCount = 0;
	spriteEntity = entity;
	//comienzo_frame = SDL_GetTicks();
	delay = static_cast<float>(entity->delay()); 
	fps = static_cast<float>(entity->fps());
	this->initialize();
	//cargarFrames(entity);
}

SpriteAnimado::~SpriteAnimado() {
}

void SpriteAnimado::initialize() {
	estado = 0;
	relx = spriteEntity->pixelRefX();
	rely = spriteEntity->pixelRefY();
	_baseWidth = spriteEntity->baseWidth();
	_baseHeight = spriteEntity->baseHeight();
	this->loadSurfaces();
}

void SpriteAnimado::updateFrame() {
	float deltaTime = GameView::instance().getTimer()->getDeltaTime();
	if (estado == 0)
		deltaTime -= delay;
	//comienzo_frame = SDL_GetTicks();
	this->addSticks(deltaTime); //TODO: traer del timer
	if ( this->timeIsOver())
		this->advance();
}
//
//void SpriteAnimado::getNextFrame() {
//	this->avanzarFrames();
//}

bool SpriteAnimado::lastFrame() {
	if (this->estado >= (surfacesCount - 1)) {
		return true;
	} else {
		return false;
	}
}

void SpriteAnimado::restart() {
	this->estado = 0;
}

void SpriteAnimado::advance() {
	if ( this->lastFrame() )
		this->estado = 0;
	else
		this->estado++;
	this->accumulatedTime -= (1000/fps);
}

bool SpriteAnimado::timeIsOver() {
	return (this->accumulatedTime >= (1000/fps));
}


void SpriteAnimado::loadSurfaces() {
	AnimatedEntity* auxEntity = (AnimatedEntity*)spriteEntity;
	//auxEntity->imagesPaths()->restartCurrentPosition();
	surfacesCount = auxEntity->imagesPaths()->count();
}

void SpriteAnimado::addSticks(float ticks) {
	this->accumulatedTime += ticks;
}

void SpriteAnimado::setCurrentState(unsigned state) {
	this->estado = state;
}