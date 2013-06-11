#include "Shield.h"


Shield::Shield(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself):ItemView(_name,_state,_pos,_hiddenSprite,sprite,_canReviveForHimself)
{
	this->resistance=50;//HARCODEO resistencia
	this->absortion=10;
}


Shield::~Shield(void)
{
}

void Shield::modifyCharacter(Personaje* personaje)
{
	personaje->setShield(this->resistance,this->absortion);
}