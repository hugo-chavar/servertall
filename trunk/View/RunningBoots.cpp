#include "RunningBoots.h"


RunningBoots::RunningBoots(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite) : ItemView(_name,_hidden,_pos,_hiddenSprite,sprite)
{
}


RunningBoots::~RunningBoots(void)
{
}

void RunningBoots::modifyCharacter(Personaje* personaje)
{
	personaje->increaseSpeed(FACTORVEL);
}