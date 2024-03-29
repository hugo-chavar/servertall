#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <vector>
#include "Sprite.h"
#include "SpriteAnimado.h"
#include "PersonajeModelo.h"
#include "Entity.h"
#include "Daniable.h"
#include "Hechizo.h"
#include "Weapon.h"

class Personaje : public Daniable {
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
	void processKeyCommand(char animation);
	std::string getCharacterId();
	bool isCenteredInTile();
	std::string idToString();
	void increaseSpeed(float factor);
	void heal();
	void rechargeMagic();
	void recibirDano(float dano);
	bool isItem();
	void eatIfItem(std::pair<int, int> destino);
	bool hasValidSprite();
	void setShield(float resistance,float absortion);
	bool hasShield();
	bool useMagic(float usedMagic); //Devuelve si pudo usar esa cantidad de magia
	void setInvulnerable(bool inv);
	void invocarMagia();
	void detenerMagia();
	void setProtCost(float cost);
	void setHechizo(Hechizo* hechizo);
	void setProtTime(float time);
	void setVidCost(float cost);
	void setVidencia(bool vid);
	void setVidTime(float time);
	bool hasIceSpell();
	void setIceSpell(bool value);
	bool isWood();
	std::vector<Weapon*>& getWeapons();
	void setSelectedWeapon(unsigned value);
	unsigned getSelectedWeapon();
	void loadWeapons();
	bool isValidWeapon();
	bool repositionToStrike();
	bool isThisSpriteValid(int currentAnimationNumber);

private:
	int calculateSpritePosition(int currentAnimationNumber);
	void calcularvelocidadRelativa(std::pair<float, float>& factor);
	void mover();
	void calcularSigTileAMover();
	void moverSprite(std::pair<float, float>& factor);
	void moverSpriteEnX();
	void moverSpriteEnY();
	//void setCenteredInTile(bool centroTile);
	void animar();
	void detenerAnimacion();
	void setCurrentEnemy(int tileX, int tileY);
	void atacar();
	void manejarDano(float dano);
	void perseguirEnemigo();
	void reset();
	void stopProtectionSpell();
	void updateProtectionSpell();
	void updateCrystallBall();
	void stopCrystallBall();
	void stopMagic();
	void changeWeapon();

	PersonajeModelo* modelo;
	std::vector<SpriteAnimado*> sprites;
	float velocidad;
	int currentSpritePosition;
	std::pair<float, float> delta;
	std::pair<float, float> ePot;
	int serr;
	//bool centeredInTile;
	Daniable* currentEnemy;
	Hechizo* hechizoActual;
	float protCost;
	float protTime;
	float magiaActual;
	float shieldResistance;
	bool invulnerable;
	bool videncia;
	bool iceSpell;
	bool wandSpell;
	float vidCost;
	float vidTime;
	float shieldAbsortion;
	
	unsigned selectedWeapon;
	std::string playerName;
	std::string character_id;
	std::vector<Weapon*> weapons;
};

#endif