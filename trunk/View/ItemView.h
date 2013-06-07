#pragma once
#include "Entity.h"
#include "Personaje.h"
#include "StringUtilities.h"
#include "Daniable.h"

class ItemView : public Entity,public Daniable
{

//public:
//
//	ItemView(int tileX,int tileY,Sprite* spriteCargado,Sprite * _hiddenSprite,std::string _hidden);
//	~ItemView();
//	//void render(Camera& camera);
//	void changeState();

protected:
	bool alive;
	bool hidden;
	string name;
	std::pair<int,int> pos;
	string ItemView::itemChangeToString(char change);
	SDL_Rect hiddenSpriteRect;
	Sprite * hiddenSprite;
	void setHiddenRectangle(std::pair<int, int> pos, Sprite* sprite );

public:
	void update();
	ItemView(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~ItemView(void);
	virtual void modifyCharacter(Personaje* personaje);
	string kill();
	bool isAlive();
	bool isHidden();
	string uncover();
	string getName();
	std::pair<int,int> getPos();
	void setPos(std::pair<int,int> position);
	string revive();
	void recibirDano(float dano);
	bool isItem();
};

