#include "Lamp.h"
//#define FACTORVISION 1.5 //factor de aumento del rango de vision

Lamp::Lamp(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself) : ItemView(_name,_state,_pos,_hiddenSprite,sprite,_canReviveForHimself)
{
}


Lamp::~Lamp(void)
{
}

void Lamp::modifyCharacter(Personaje* personaje)
{
	int incrementar = 3; //TODO: Yami, traelo del parser
	personaje->personajeModelo()->getVision()->increaseRangeVision(incrementar);
}