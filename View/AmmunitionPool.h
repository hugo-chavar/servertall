#ifndef _AMMUNITION_POOL_H_
#define _AMMUNITION_POOL_H_

#include "Ammunition.h"

#define NUMBERITEMS 6 // Cantidad de items implementados

class AmmunitionPool {
public:
	AmmunitionPool();
	~AmmunitionPool();
	//ItemView* createItem(Sprite* itemSprite,Sprite* chestSprite,string isHidden,std::pair<int,int> pos);
	//ItemView* generateRandomItem(float porcentajeAparicion,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
	//ItemView* generateItem(int itemPos,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
};

#endif // _AMMUNITION_POOL_H_

