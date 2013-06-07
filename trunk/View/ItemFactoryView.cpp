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

ItemView* ItemFactoryView::generateItem(float porcentajeAparicion,string _hidden,std::pair <int,int> pos)
{
	if(porcentajeAparicion>0)
	{
	int indice=static_cast <int> ((100.0/porcentajeAparicion) * NUMBERITEMS);
	int random= rand() % indice;
	ItemView* itemView=NULL;
	if(random<NUMBERITEMS)
	{
	Sprite* hiddenSprite=GameView::instance().getWorldView()->getSprite("Chest");
	Sprite* sprite=NULL;
	switch (random){
		case 0:
			sprite=GameView::instance().getWorldView()->getSprite("Lamp");
			return new Lamp("Lamp",_hidden,pos,hiddenSprite,sprite);
		case 1:
			sprite=GameView::instance().getWorldView()->getSprite("RunningBoots");
			return new RunningBoots("RunningBoots",_hidden,pos,hiddenSprite,sprite);
		case 2:
			sprite=GameView::instance().getWorldView()->getSprite("MapItem");
			return new MapItem("MapItem",_hidden,pos,hiddenSprite,sprite);
		}
	}
	return NULL;
	}
}