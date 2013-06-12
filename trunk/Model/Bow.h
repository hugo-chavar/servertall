#ifndef _BOW_H_
#define _BOW_H_

#include "ProjectileWeapon.h"

namespace model {
	class Bow : public model::ProjectileWeapon {

	public:
		Bow();

		virtual void strike(int x, int y);

		virtual ~Bow();
	};
} // namespace model


#endif // _BOW_H_