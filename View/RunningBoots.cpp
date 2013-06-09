#include "RunningBoots.h"


RunningBoots::RunningBoots(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself) : ItemView(_name,_state,_pos,_hiddenSprite,sprite,_canReviveForHimself)
{
}


RunningBoots::~RunningBoots(void)
{
}

void RunningBoots::modifyCharacter(Personaje* personaje)
{
	personaje->increaseSpeed(FACTORVEL);
}