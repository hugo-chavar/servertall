#include "ItemView.h"
#include "GameView.h"


ItemView::ItemView(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite):Entity(_pos.first,_pos.second,sprite)
{
	this->alive=true;
	this->name=_name;
	this->pos=_pos;
	if(_hidden=="H")
		this->hidden=true;
	else
		this->hidden=false;
	this->hiddenSprite = _hiddenSprite;
	this->setHiddenRectangle(_pos,this->hiddenSprite);
}


ItemView::~ItemView(void)
{

}

void ItemView::setHiddenRectangle(std::pair<int, int> pos, Sprite* sprite ) {
	hiddenSpriteRect = posicionIsometricaPorTiles(pos.first, pos.second, sprite);
}

void ItemView::update()
{

}

bool ItemView::isItem()
{
	return true;
}

string ItemView::kill()
{
	this->alive=false;
	return	itemChangeToString('D');
}

string ItemView::revive()
{
	this->alive=true;
	return(itemChangeToString('A'));
}

bool ItemView::isAlive()
{
	return this->alive;
}

bool ItemView::isHidden()
{
	return this->hidden;
}

string ItemView::uncover()
{
	this->hidden=false;
	return itemChangeToString('H');
}

string ItemView::getName()
{
	return this->name;
}

std::pair<int,int> ItemView::getPos()
{
	return pos;
}

void ItemView::setPos(std::pair<int,int> position)
{
	this->pos=position;
}

string ItemView::itemChangeToString(char change)
{
	string modification=stringUtilities::pairIntToString(this->getPos())+","+change;
	return modification;
}

void ItemView::recibirDano(float dano)
{
	if(dano>0 && this->hidden)
	{
		this->uncover();
		GameView::instance().getWorldView()->getTileAt(this->getPos())->setItemUncover();//Enviar Cliente Change
	}
}

void ItemView::modifyCharacter(Personaje* personaje)
{
	//MetodoAbstracto
}