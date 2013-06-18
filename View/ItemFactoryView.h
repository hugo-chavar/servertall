#pragma once
#include "ItemView.h"
#include "Lamp.h"
#include "MapItem.h"
#include "RunningBoots.h"
#include "Heart.h"
#include "ItemHProteccion.h"
#include "MagicBottle.h"
#include "Shield.h"
#include "ItemTerremoto.h"
#include "ItemFlecha.h"

#define NUMBERITEMS 9 // Cantidad de items implementados

class ItemFactoryView
{
public:
	ItemFactoryView(void);
	~ItemFactoryView(void);
	//ItemView* createItem(Sprite* itemSprite,Sprite* chestSprite,string isHidden,std::pair<int,int> pos);
	ItemView* generateRandomItem(float porcentajeAparicion,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
	ItemView* generateItem(int itemPos,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself);
};

