#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <utility>

namespace model {
	class Projectile /* Should inherit of whatever entities base class the simulation is using. Cannot find it. */ {
	private:
		bool available;

		std::pair<int,int> target;

		float damage;

	public:
		Projectile();

		bool isAvailable();

		void setAvailable(bool available);

		std::pair<int,int>& getTarget();

		void setTarget(int x, int y);

		float getDamage();

		void setDamage(float damage);

		void update();

		virtual ~Projectile();
	};
} // namespace model


#endif // _PROJECTILE_H_