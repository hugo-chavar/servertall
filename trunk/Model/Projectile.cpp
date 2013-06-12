#include "Projectile.h"

model::Projectile::Projectile() {
	this->available = false;
	this->damage = 0;
}

bool model::Projectile::isAvailable() {
	return this->available;
}

void model::Projectile::setAvailable(bool available) {
	this->available = available;
}

std::pair<int,int>& model::Projectile::getTarget() {
	return this->target;
}

void model::Projectile::setTarget(int x, int y) {
	this->target = std::pair<int,int>(x,y);
}

float model::Projectile::getDamage() {
	return this->damage;
}

void model::Projectile::setDamage(float damage) {
	this->damage = damage;
}

void model::Projectile::update() {
	//Use collision detection from simulation. Can`t find it.
	//on collision:
	/*
	this->setAvailable(false);
	this->setVisible(false); //should be on entity base class.
	entityCollisioned->getDamage(this->getDamage()); //should be on entityCollisioned base class.
	*/
}

model::Projectile::~Projectile() {
}