#include "Hearth.h"


Hearth::Hearth(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite) : ItemView(_name,_hidden,_pos,_hiddenSprite,sprite)
{
}


Hearth::~Hearth()
{
}

void Hearth::modifyCharacter(Personaje* personaje)
{
	personaje->heal();
}