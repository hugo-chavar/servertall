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
	ItemView* generateItem(float porcentajeAparicion,string _hidden,std::pair <int,int> pos);
};

