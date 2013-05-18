#include "SpriteAnimado.h"

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

void SpriteAnimado::actualizarFrame() {
	float deltaTime = 0.0;
	if (estado == 0)
		deltaTime = delay;
	if ( tiempoFrameCumplido(deltaTime))
		this->avanzarFrames();
}

void SpriteAnimado::getNextFrame() {
	this->avanzarFrames();
}

bool SpriteAnimado::ultimoFrame() {
	if (estado >= (surfacesCount - 1)) {
		return true;
	} else {
		return false;
	}
}

void SpriteAnimado::reiniciar() {
	estado = 0;
}

void SpriteAnimado::avanzarFrames() {
	//comienzo_frame = SDL_GetTicks();
	this->setSticks(500); //TODO: traer del timer
	if ( this->ultimoFrame() )
		estado = 0;
	else
		estado = estado++;
}

bool SpriteAnimado::tiempoFrameCumplido(float delta) {
	return ((500 - comienzo_frame) >= ((1000/fps) + delta)); //TODO: traer del timer
}


void SpriteAnimado::loadSurfaces() {
	AnimatedEntity* auxEntity = (AnimatedEntity*)spriteEntity;
	auxEntity->imagesPaths()->restartCurrentPosition();
	/*view::Surface* auxSurface = new view::Surface();
	auxSurface->load(auxEntity->imagesPaths()->nextFullPath());
	this->surfaceWidth = auxSurface->getWidth();
	this->surfaceHeight = auxSurface->getHeight();
	auxSurface->free();*/
	surfacesCount = auxEntity->imagesPaths()->count();
}

void SpriteAnimado::setSticks(int ticks) {
	comienzo_frame = ticks;
}

void SpriteAnimado::setCurrentState(unsigned state) {
	estado = state;
}