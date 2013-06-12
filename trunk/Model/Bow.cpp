#include "Bow.h"

#include <iostream>

model::Bow::Bow() {
}

void model::Bow::strike(int x, int y) {
	model::Projectile* arrow = NULL;

	if (this->getAmmo() > 0) {
		arrow = this->getAvailableProjectile();
		if (arrow != NULL) {
			arrow->setTarget(x,y);
			arrow->setDamage(this->getDamage());
			//put projectile into simulation entities list.
		}
	}
}

model::Bow::~Bow() {
}