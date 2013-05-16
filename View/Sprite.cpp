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
	estado = 0;
	relx = spriteEntity->pixelRefX();
	rely = spriteEntity->pixelRefY();
	_baseWidth = spriteEntity->baseWidth();
	_baseHeight = spriteEntity->baseHeight();
	this->loadSurfaces();
}

Sprite::Sprite() {
}

Sprite::~Sprite() {
}

void Sprite::actualizarFrame() {
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

int Sprite::getCurrentState() {
	return estado;
}

void Sprite::loadSurfaces() {
	//view::Surface* auxSurface = new view::Surface();
	//auxSurface->load(spriteEntity->imagePath());
	//this->surfaceWidth = auxSurface->getWidth();
	//this->surfaceHeight = auxSurface->getHeight();
	//auxSurface->free();
	surfacesCount = 1;
}
