#include "ItemView.h"
#include "GameView.h"
#include "Game.h"
#define VARIABLE_REGENERATION_TIME 60000
#define CONST_REGENERATION_TIME 30000


ItemView::ItemView(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself):Entity(_pos.first,_pos.second,sprite)
{
	//this->alive=true;
	this->name=_name;
	//this->tileActual=_pos;
	this->setPosition(_pos);
	//this->state=_state;
	this->setStatus(_state);
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
		if(this->getStatus()==DEATH_ITEM)
			{
				int delta=static_cast <int>(GameView::instance().getTimer()->getDeltaTime());
				if(regenerationTime-delta>0)
					this->regenerationTime-=delta;
				else
					{
						this->regenerationTime=0;
						if(!GameView::instance().isThereAPlayerInTile(this->getPosition()))
						{
							GameView::instance().getWorldView()->getTileAt(this->getPosition())->setOtherEntity(this);
							this->revive(HIDDEN_ITEM,this->getPosition());//Aca tendria que meter logica para que cambie de lugar el item
						}
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

bool ItemView::getCanReviveForHimself()
{
	return this->canReviveForHimself;
}

void ItemView::kill()
{
	//this->state=DEATH_ITEM;
	this->setStatus(DEATH_ITEM);
	//EMPEZAR A CONTAR EL TIEMPO
	GameView::instance().getWorldView()->getTileAt(this->getPosition())->setOtherEntity(NULL);
	GameView::instance().getWorldView()->addItemChange(itemChangeToString());
	regenerationTime=CONST_REGENERATION_TIME+rand()%VARIABLE_REGENERATION_TIME;
}

void ItemView::revive(unsigned _state,std::pair <int,int> _pos)
{
	//this->alive=true;
	//this->tileActual=_pos;
	this->setPosition(_pos);
	//this->state=_state;
	this->setStatus(_state);
	GameView::instance().getWorldView()->addItemChange(itemChangeToString());
	if(_state==REVIVE_UNCOVER_ITEM)
	{
		this->setStatus(UNCOVER_ITEM);
	}
	else{
		this->setStatus(HIDDEN_ITEM);
		//GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_HIDDEN_ITEM));
		Game::instance().world()->getTileAt(this->getPosition())->setHasHiddenItem(true);
	}
	//else{
	//	GameView::instance().getWorldView()->addItemChange(itemChangeToString(REVIVE_UNCOVER_ITEM));
	//}
}

bool ItemView::isAlive()
{
	return (this->isHidden() || this->getStatus()==UNCOVER_ITEM);
}

bool ItemView::isHidden()
{
	return (this->getStatus()==HIDDEN_ITEM);
}

void ItemView::uncover()
{
	//this->hidden=false;
	//this->state=UNCOVER_ITEM;
	this->setStatus(UNCOVER_ITEM);
	GameView::instance().getWorldView()->addItemChange(itemChangeToString());
}

string ItemView::getName()
{
	return this->name;
}

//std::pair<int,int> ItemView::getPos()
//{
//	return pos;
//}
//
//void ItemView::setPos(std::pair<int,int> position)
//{
//	this->tileActual=position;
//}

string ItemView::itemChangeToString() //unsigned _state
{
	string modification=this->statusToString()+";"+this->name+";"+stringUtilities::pairIntToString(this->getPosition());
	return modification;
}

void ItemView::recibirDano(float dano)
{
	if(dano>0 && this->isHidden())
	{
		this->uncover();
		GameView::instance().getWorldView()->getTileAt(this->getPosition())->setItemUncover();
	}
}

void ItemView::modifyCharacter(Personaje* )
{
	//MetodoAbstracto
}
//
//unsigned ItemView::getState()
//{
//	return this->state;
//}