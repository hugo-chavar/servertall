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
	void kill();
	bool isAlive();
	bool isHidden();
	void uncover();
	string getName();
	//std::pair<int,int> getPos();
	void setPos(std::pair<int,int> position);
	void revive(char hidden);
	void recibirDano(float dano);
	bool isItem();
};

