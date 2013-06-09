#include "MagicBottle.h"


MagicBottle::MagicBottle(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite) : ItemView(_name,_hidden,_pos,_hiddenSprite,sprite)
{
}


MagicBottle::~MagicBottle()
{
}

void MagicBottle::modifyCharacter(Personaje* personaje)
{
	personaje->rechargeMagic();
}