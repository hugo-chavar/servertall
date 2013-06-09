#pragma once
#include "ItemView.h"
#include "Lamp.h"
#include "MapItem.h"
#include "RunningBoots.h"


#define NUMBERITEMS 3 // Cantidad de items implementados

class ItemFactoryView
{
public:
	ItemFactoryView(void);
	~ItemFactoryView(void);
	//ItemView* createItem(Sprite* itemSprite,Sprite* chestSprite,string isHidden,std::pair<int,int> pos);
	ItemView* generateRandomItem(float porcentajeAparicion,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
	ItemView* generateItem(int itemPos,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
};

