#include "ItemView.h"
#include "GameView.h"


ItemView::ItemView(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite):Entity(_pos.first,_pos.second,sprite)
{
	this->alive=true;
	this->name=_name;
	this->tileActual=_pos;
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

void ItemView::kill()
{
	this->alive=false;
	GameView::instance().getWorldView()->addItemChange(itemChangeToString("D"));
}

void ItemView::revive(char hidden)
{
	this->alive=true;
	GameView::instance().getWorldView()->addItemChange(itemChangeToString("A"+hidden));
}

bool ItemView::isAlive()
{
	return this->alive;
}

bool ItemView::isHidden()
{
	return this->hidden;
}

void ItemView::uncover()
{
	this->hidden=false;
	GameView::instance().getWorldView()->addItemChange(itemChangeToString("U"));
}

string ItemView::getName()
{
	return this->name;
}

//std::pair<int,int> ItemView::getPos()
//{
//	return pos;
//}

void ItemView::setPos(std::pair<int,int> position)
{
	this->tileActual=position;
}

string ItemView::itemChangeToString(string change)
{
	string modification=change+";"+this->name+";"+stringUtilities::pairIntToString(this->getPosicionActualEnTiles());
	return modification;
}

void ItemView::recibirDano(float dano)
{
	if(dano>0 && this->hidden)
	{
		this->uncover();
		GameView::instance().getWorldView()->getTileAt(this->getPosicionActualEnTiles())->setItemUncover();
	}
}

void ItemView::modifyCharacter(Personaje* personaje)
{
	//MetodoAbstracto
}