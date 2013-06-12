#ifndef _MELEE_WEAPON_H_
#define _MELEE_WEAPON_H_

#include "Weapon.h"

namespace model {
	class MeleeWeapon : public model::Weapon {

	public:
		MeleeWeapon();

		virtual void strike(int x, int y) = 0;

		virtual ~MeleeWeapon();
	};
} // namespace model


#endif // _MELEE_WEAPON_H_