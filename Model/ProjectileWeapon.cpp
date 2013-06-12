#include "ProjectileWeapon.h"

model::ProjectileWeapon::ProjectileWeapon() {
	this->ammo = 0;
	this->nextProjectileIndex = 0;

	model::Projectile* projectile = NULL;
	for (unsigned int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		projectile = new Projectile();
		this->getProjectiles().push_back(projectile);
	}
}

std::vector<model::Projectile*>& model::ProjectileWeapon::getProjectiles() {
	return this->projectiles;
}

unsigned int model::ProjectileWeapon::getNextProjectileIndex() {
	return this->nextProjectileIndex;
}

void model::ProjectileWeapon::setNextProjectileIndex(unsigned int availableProjectileIndex) {
	this->nextProjectileIndex = availableProjectileIndex;
}

model::Projectile* model::ProjectileWeapon::getAvailableProjectile() {
	model::Projectile* projectile = NULL;
	bool found = false;
	unsigned int i = this->getNextProjectileIndex();

	do {
		projectile = this->getProjectiles()[i];

		if (projectile->isAvailable()) {
			found = true;
			projectile->setAvailable(false);
		}

		if (i < PROJECTILE_POOL_SIZE)
			i++;
		else
			i = 0;
	} while (!found && i != this->getNextProjectileIndex());

	if (!found)
		projectile = NULL; //TODO: LOG SOME WARNING.

	this->setNextProjectileIndex(i);

	return projectile;
}

unsigned int model::ProjectileWeapon::getAmmo() {
	return this->ammo;
}

void model::ProjectileWeapon::setAmmo(unsigned int ammo) {
	this->ammo = ammo;
}

model::ProjectileWeapon::~ProjectileWeapon() {
	for (unsigned int i = 0; i < PROJECTILE_POOL_SIZE; i++) {
		delete this->getProjectiles()[i];
	}
}