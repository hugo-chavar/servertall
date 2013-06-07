#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <vector>
#include "Sprite.h"
#include "SpriteAnimado.h"
#include "PersonajeModelo.h"
#include "Entity.h"
#include "Daniable.h"

class Personaje : public Entity, public Daniable {
public:
	Personaje(PersonajeModelo*,std::string char_id);
	~Personaje();
	void update();
	PersonajeModelo* personajeModelo();
	void loadSprites();
	void clearSprites() ;
	void addNextSprite(AnimatedEntity*);
	void addFirstSprite(AnimatedEntity*);
	std::pair<int,int> getPosicionEnTiles();
	void setFreezed(bool value);
	void setAnimating(bool value);
	std::string updateToString();
	void updateFromString(std::string data);
	std::string initToString();
	void initFromString(std::string data);
	void setPixelPosition(std::pair<int, int> pixel);
	std::pair<int, int> getPixelPosition();
	int getCurrentSpritePosition();
	void setCurrentSpritePosition(int pos);
	void setPlayerName(std::string name);
	std::string getPlayerName();
	std::pair<int,int> getPosicionActualEnTiles();
	void setDestino(int xTile, int yTile);
	void animateModel(char animation);
	std::string getCharacterId();
	bool isCenteredInTile();
	std::string idToString();
	void increaseSpeed(float factor);
	void recibirDano(float dano);
	bool isItem();
	void eatIfItem(std::pair<int, int> destino);

private:
	//determina en que posicion del vector de sprites esta el sprite del estado actual
	int calculateSpritePosition(int currentAnimationNumber);
	void calcularvelocidadRelativa(std::pair<float, float>& factor);
	void mover();
	void calcularSigTileAMover();
	//realiza el desplazamiento en x y en y del sprite, de un tile a otro
	void moverSprite(std::pair<float, float>& factor);
	void moverSpriteEnX();
	void moverSpriteEnY();
	//void setCenteredInTile(bool centroTile);
	void animar();
	void detenerAnimacion();
	void setCurrentEnemy(int tileX, int tileY);
	void atacar();
	void resolverAtaque();
	void perseguirEnemigo();


	PersonajeModelo* modelo;
	std::vector<SpriteAnimado*> sprites;
	float velocidad; //velocidad actual
	int currentSpritePosition; //estado actual, si se mueve y en que direcci�n
	std::pair<float, float> delta; //Cuanto recorrio en x y en y del movimiento
	std::pair<float, float> ePot; //Cuanto se mueve por ciclo en x y en y
	int serr; //variable usada para coordinar el movimiento en x y en y en la diagonal
	//bool centeredInTile;
	Daniable* currentEnemy; //el destruible que esta atacando el pj


	std::string playerName;
	std::string character_id;
};

#endif