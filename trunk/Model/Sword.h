#ifndef _SWORD_H_
#define _SWORD_H_

#include "MeleeWeapon.h"

namespace model {
	class Sword : public model::MeleeWeapon {

	public:
		Sword();

		virtual void strike(int x, int y);

		virtual ~Sword();
	};
} // namespace model


#endif // _SWORD_H_