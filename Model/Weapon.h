#ifndef _WEAPON_H_
#define _WEAPON_H_

namespace model {
	class Weapon {
	private:
		bool active;

		unsigned int range;

		float damage;

		float precision;

		void setActive(bool active);

		void setRange(unsigned int range);

		void setDamage(float damage);

		void setPrecision(float precision);
	public:
		Weapon();

		void initialize(bool active, unsigned int range, float damage, float precision);

		bool getActive();

		unsigned int getRange();

		float getDamage();

		float getPrecision();

		virtual void strike(int x, int y) = 0;

		virtual ~Weapon();
	};
} // namespace model


#endif // _WEAPON_H_