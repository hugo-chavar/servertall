#include "Weapon.h"

model::Weapon::Weapon() {
	this->active = false;
	this->range = 0;
	this->damage = 0;
	this->precision = 0;
}

void model::Weapon::setActive(bool active) {
	this->active = active;
}

void model::Weapon::setRange(unsigned int range) {
	this->range = range;
}

void model::Weapon::setDamage(float damage) {
	this->damage = damage;
}

void model::Weapon::setPrecision(float precision) {
	this->precision = precision;
}

void model::Weapon::initialize(bool active, unsigned int range, float damage, float precision) {
	this->setActive(active);
	this->setRange(range);
	this->setDamage(damage);
	this->setPrecision(precision);
}

bool model::Weapon::getActive() {
	return this->active;
}

unsigned int model::Weapon::getRange() {
	return this->range;
}

float model::Weapon::getDamage() {
	return this->damage;
}

float model::Weapon::getPrecision() {
	return this->precision;
}

model::Weapon::~Weapon() {
}