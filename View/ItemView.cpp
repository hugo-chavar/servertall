#include "ItemView.h"
#include "GameView.h"
#include "Game.h"
#define VARIABLE_REGENERATION_TIME 60000
#define CONST_REGENERATION_TIME 30000


ItemView::ItemView(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself):Entity(_pos.first,_pos.second,sprite)
{
	//this->alive=true;
	this->name=_name;
	this->tileActual=_pos;
	this->state=_state;
	this->canReviveForHimself=_canReviveForHimself;
	//if(_hidden=="H")
		//this->hidden=true;
	//else
	//	this->hidden=false;
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
	if (this->canReviveForHimself)
	{
		if(this->state==DEATH_ITEM)
			{
				int delta=static_cast <int>(GameView::instance().getTimer()->getDeltaTime());
				if(regenerationTime-delta>0)
					this->regenerationTime-=delta;
				else
					{
						this->regenerationTime=0;
						if(!GameView::instance().getWorldView()->isThereAPlayerInTile(this->getPosicionActualEnTiles()))
							this->revive(HIDDEN_ITEM);
						else
							regenerationTime=CONST_REGENERATION_TIME+rand()%VARIABLE_REGENERATION_TIME;
					}
			}
	}
}

bool ItemView::isItem()
{
	return true;
}

void ItemView::kill()
{
	this->state=DEATH_ITEM;
	//EMPEZAR A CONTAR EL TIEMPO
	GameView::instance().getWorldView()->addItemChange(itemChangeToString(DEATH_ITEM));
	regenerationTime=CONST_REGENERATION_TIME+rand()%VARIABLE_REGENERATION_TIME;
}

void ItemView::revive(unsigned _state)
{
	//this->alive=true;
	this->state=_state;
	if(state==HIDDEN_ITEM){
		GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_HIDDEN_ITEM));
		Game::instance().world()->getTileAt(this->getPosicionActualEnTiles())->setHasHiddenItem(true);
	}
	else{
		GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_UNCOVER_ITEM));
	}
}

bool ItemView::isAlive()
{
	return (this->state==HIDDEN_ITEM || this->state==UNCOVER_ITEM);
}

bool ItemView::isHidden()
{
	return (this->state==HIDDEN_ITEM);
}

void ItemView::uncover()
{
	//this->hidden=false;
	this->state=UNCOVER_ITEM;
	GameView::instance().getWorldView()->addItemChange(itemChangeToString(UNCOVER_ITEM));
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

string ItemView::itemChangeToString(unsigned _state)
{
	string modification=stringUtilities::unsignedToString(_state)+";"+this->name+";"+stringUtilities::pairIntToString(this->getPosicionActualEnTiles());
	return modification;
}

void ItemView::recibirDano(float dano)
{
	if(dano>0 && this->state==HIDDEN_ITEM)
	{
		this->uncover();
		GameView::instance().getWorldView()->getTileAt(this->getPosicionActualEnTiles())->setItemUncover();
	}
}

void ItemView::modifyCharacter(Personaje* personaje)
{
	//MetodoAbstracto
}

unsigned ItemView::getState()
{
	return this->state;
}