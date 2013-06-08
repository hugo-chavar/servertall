#include <sstream> 
#include "Sprite.h"
//#include "Surface.h"
#include "AnimatedEntity.h"
#include "DirList.h"

Sprite::Sprite(EntityObject *entity) {
	surfacesCount = 0;
	spriteEntity = entity;
	this->initialize();
}

void Sprite::initialize() {
	this->restart();
	this->relx = this->spriteEntity->pixelRefX();
	this->rely = this->spriteEntity->pixelRefY();
	this->_baseWidth = this->spriteEntity->baseWidth();
	this->_baseHeight = this->spriteEntity->baseHeight();
	this->loadSurfaces();
}

Sprite::Sprite() {
}

Sprite::~Sprite() {
}

void Sprite::updateFrame() {
}

int Sprite::relatx() {
	return relx;
}

int Sprite::relaty() {
	return rely;
}

int Sprite::baseWidth() {
	return _baseWidth;
}

int Sprite::baseHeight() {
	return _baseHeight;
}

unsigned Sprite::getCurrentSurfaceNumber() {
	return this->currentSurfaceNumber;
}

void Sprite::loadSurfaces() {
	//view::Surface* auxSurface = new view::Surface();
	//auxSurface->load(spriteEntity->imagePath());
	//this->surfaceWidth = auxSurface->getWidth();
	//this->surfaceHeight = auxSurface->getHeight();
	//auxSurface->free();
	surfacesCount = 1;
}

void Sprite::restart() {
	this->setCurrentSurfaceNumber(0);
}

void Sprite::setCurrentSurfaceNumber(unsigned surfaceNumber) {
	this->currentSurfaceNumber = surfaceNumber;
}
