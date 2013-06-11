#include "Ammunition.h"
#include "GameView.h"
#include "Game.h"


Ammunition::Ammunition(string _name,Sprite* sprite)//:Entity(_pos.first,_pos.second,sprite)
{
	//this->alive=true;
	this->name=_name;
	//this->tileActual=_pos;
}

Ammunition::~Ammunition(){
}


string Ammunition::getName()
{
	return this->name;
}

bool Ammunition::isItem()
{
	return false;
}

void Ammunition::setHiddenRectangle(std::pair<int, int> pos, Sprite* sprite ) {
	hiddenSpriteRect = posicionIsometricaPorTiles(pos.first, pos.second, sprite);
}

void Ammunition::update()
{
	//if (this->canReviveForHimself)
	//{
	//	if(this->state==DEATH_ITEM)
	//		{
	//			int delta=static_cast <int>(GameView::instance().getTimer()->getDeltaTime());
	//			if(regenerationTime-delta>0)
	//				this->regenerationTime-=delta;
	//			else
	//				{
	//					this->regenerationTime=0;
	//					if(!GameView::instance().getWorldView()->isThereAPlayerInTile(this->getPosicionActualEnTiles()))
	//					{
	//						GameView::instance().getWorldView()->getTileAt(this->tileActual)->setOtherEntity(this);
	//						this->revive(HIDDEN_ITEM,this->tileActual);//Aca tendria que meter logica para que cambie de lugar el item
	//					}
	//					else
	//						regenerationTime=CONST_REGENERATION_TIME+rand()%VARIABLE_REGENERATION_TIME;
	//				}
	//		}
	//}
}


bool Ammunition::getCanReviveForHimself()
{
	return this->canReviveForHimself;
}

void Ammunition::kill()
{
	//this->state=DEATH_ITEM;
	////EMPEZAR A CONTAR EL TIEMPO
	//GameView::instance().getWorldView()->getTileAt(this->tileActual)->setOtherEntity(NULL);
	//GameView::instance().getWorldView()->addItemChange(itemChangeToString(DEATH_ITEM));
	//regenerationTime=CONST_REGENERATION_TIME+rand()%VARIABLE_REGENERATION_TIME;
}

void Ammunition::revive(unsigned _state,std::pair <int,int> _pos)
{
	//this->alive=true;
//	this->tileActual=_pos;
	this->state=_state;
	//if(state==HIDDEN_ITEM){
	//	GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_HIDDEN_ITEM));
	//	Game::instance().world()->getTileAt(this->getPosicionActualEnTiles())->setHasHiddenItem(true);
	//}
	//else{
	//	GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_UNCOVER_ITEM));
	//}
}

bool Ammunition::isAlive()
{
	return (this->state==HIDDEN_ITEM || this->state==UNCOVER_ITEM);
}

bool Ammunition::isHidden()
{
	return (this->state==HIDDEN_ITEM);
}

void Ammunition::uncover()
{
	//this->hidden=false;
	this->state=UNCOVER_ITEM;
	GameView::instance().getWorldView()->addItemChange(itemChangeToString(UNCOVER_ITEM));
}



void Ammunition::setPos(std::pair<int,int> position)
{
	//this->tileActual=position;
}

//string ItemView::itemChangeToString(unsigned _state)
//{
//	string modification=stringUtilities::unsignedToString(_state)+";"+this->name+";"+stringUtilities::pairIntToString(this->getPosicionActualEnTiles());
//	return modification;
//}
//
//void ItemView::recibirDano(float dano)
//{
//	if(dano>0 && this->state==HIDDEN_ITEM)
//	{
//		this->uncover();
//		GameView::instance().getWorldView()->getTileAt(this->getPosicionActualEnTiles())->setItemUncover();
//	}
//}
//
//void ItemView::modifyCharacter(Personaje* personaje)
//{
//	//MetodoAbstracto
//}
