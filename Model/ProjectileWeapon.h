#ifndef _PROJECTILE_WEAPON_H_
#define _PROJECTILE_WEAPON_H_

#ifndef PROJECTILE_POOL_SIZE
#define PROJECTILE_POOL_SIZE 5
#endif//PROJECTILE_POOL_SIZE

#include "Weapon.h"

#include <vector>

#include "Projectile.h"

namespace model {
	class ProjectileWeapon : public model::Weapon {
	private:
		unsigned int ammo;

		std::vector<model::Projectile*> projectiles;

		unsigned int nextProjectileIndex;

		std::vector<model::Projectile*>& getProjectiles();

		unsigned int getNextProjectileIndex();

		void setNextProjectileIndex(unsigned int nextProjectileIndex);

	protected:
		model::Projectile* getAvailableProjectile();

	public:
		ProjectileWeapon();

		unsigned int getAmmo();

		void setAmmo(unsigned int ammo);

		virtual void strike(int x, int y) = 0;

		virtual ~ProjectileWeapon();
	};
} // namespace model


#endif // _PROJECTILE_WEAPON_H_