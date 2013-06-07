#include "Lamp.h"
#define FACTORVISION 1.5 //factor de aumento del rango de vision

Lamp::Lamp(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite) : ItemView(_name,_hidden,_pos,_hiddenSprite,sprite)
{
}


Lamp::~Lamp(void)
{
}

void Lamp::modifyCharacter(Personaje* personaje)
{
	//TODO:Implement 
	
	//personaje->increaseVisionRange(FACTORVISION);
}