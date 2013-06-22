#include "ItemFactoryView.h"
#include "GameView.h"


ItemFactoryView::ItemFactoryView(void)
{
}


ItemFactoryView::~ItemFactoryView(void)
{
}

//ItemView* ItemFactoryView::createItem(Sprite* itemSprite,Sprite* hiddenSprite,string isHidden,std::pair<int,int> pos)
//{
//	ItemView* item=new ItemView(pos.first,pos.second,itemSprite,hiddenSprite,isHidden);
//	return item;
//}

ItemView* ItemFactoryView::generateRandomItem(float porcentajeAparicion,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself)
{
	if(porcentajeAparicion>0)
	{
	int indice=static_cast <int> ((100.0/porcentajeAparicion) * NUMBERITEMS);
	int random= rand() % indice;
	if(random<NUMBERITEMS)
		{
		return this->generateItem(random,_state,pos,_canReviveForHimself);
		}
	}
	return NULL;
}

ItemView* ItemFactoryView::generateItem(int itemPos,unsigned _state,std::pair <int,int> pos,bool _canReviveForHimself)
{
	Sprite* hiddenSprite=GameView::instance().getWorldView()->getSprite("Chest");
	Sprite* sprite=NULL;
	switch (itemPos){
		case 0:
			sprite=GameView::instance().getWorldView()->getSprite("Lamp");
			if (sprite)
				return new Lamp("Lamp",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 1:
			sprite=GameView::instance().getWorldView()->getSprite("RunningBoots");
			if (sprite)
				return new RunningBoots("RunningBoots",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 2:
			sprite=GameView::instance().getWorldView()->getSprite("MapItem");
			if (sprite)
				return new MapItem("MapItem",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 3:
			sprite=GameView::instance().getWorldView()->getSprite("Heart");
			if (sprite)
				return new Heart("Heart",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 4:
			sprite=GameView::instance().getWorldView()->getSprite("MagicBottle");
			if (sprite)
				return new MagicBottle("MagicBottle",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 5:
			sprite=GameView::instance().getWorldView()->getSprite("Shield");
			if (sprite)
				return new Shield("Shield",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 6:
			sprite=GameView::instance().getWorldView()->getSprite("ItemHProteccion");
			if (sprite)
				return new ItemHProteccion("ItemHProteccion",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 7:
			sprite=GameView::instance().getWorldView()->getSprite("ItemTerremoto");
			if (sprite)
				return new ItemTerremoto("ItemTerremoto",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 8:
			sprite=GameView::instance().getWorldView()->getSprite("ItemFlecha");
			if (sprite)
				return new ItemFlecha("ItemFlecha",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 9:
			sprite=GameView::instance().getWorldView()->getSprite("crystalBall");
			if (sprite)
				return new ItemCrystallBall("crystalBall",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 10:
			sprite=GameView::instance().getWorldView()->getSprite("ItemIceSpell");
			if (sprite)
				return new ItemIceSpell("ItemIceSpell",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		case 11:
			sprite=GameView::instance().getWorldView()->getSprite("ItemMagicWand");
			if (sprite)
				return new ItemMagicWand("ItemMagicWand",_state,pos,hiddenSprite,sprite,_canReviveForHimself);
		}
	return NULL;
}