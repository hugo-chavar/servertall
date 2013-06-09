#include "Hearth.h"


Hearth::Hearth(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself) : ItemView(_name,_state,_pos,_hiddenSprite,sprite,_canReviveForHimself)
{
}


Hearth::~Hearth()
{
}

void Hearth::modifyCharacter(Personaje* personaje)
{
	personaje->heal();
}