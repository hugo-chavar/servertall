#ifndef _PERSONAJEMODELO_H
#define _PERSONAJEMODELO_H

#include <utility>
#include <vector>
#include "Constants.h"
#include "DirList.h"
#include "AnimatedEntity.h"
#include "Positionable.h"
#include "CharacterVision.h"
#include "Weapon.h"


class PersonajeModelo: public Positionable {

public:
	PersonajeModelo();
	PersonajeModelo(int ActualX, int ActualY);
	void setEstado(int state);
	//void setCurrent(int x, int y);
	void setDestino(int x, int y);
	void setVelocidad(float vel);
	//void getCurrent(std::pair<int, int>& actual);
	void setActive(bool active);
	bool isActive();
	int getEstado();
	float getVelocidad();
	/*pair <int, int> mover(std::pair<int, int> destino);*/
	int mover(std::pair<int, int>& destino, float &velocidadAni);	//Devuelve el estado, que son las orientaciones. Devuelve la velocidad
	~PersonajeModelo();											//del movimiento, la correcci�n de la vista no se la hace
	string nextDirectory();
	bool hasDirectoryRemaining();
	void setAnimation(AnimatedEntity* entity);
	AnimatedEntity* getAnimation();
	int fps();
	int delay();
	bool estaAnimandose();
	void terminarAnimacion();
	void animar(char opcion);
	string getName();
	void setName(string nombreJugador);
	void createVision(int range);
	CharacterVision* getVision();
	void update();
	std::pair<int, int> obtenerFrentePersonaje();
	void setAnimating(bool value);
	//void setRefPixelX(int);
	//void setRefPixelY(int);
	//int getRefPixelX();
	//int getRefPixelY();
	void restartDirectories();
	int getOrientacion();
	void increaseSpeed(float factor);
	bool getIsReseting();
	void setIsReseting();
	bool isThereAnEnemy(int tileX, int tileY);
	void atacar();
	float getDanoMaximo();
	float getPrecisionMinima();
	float getVidaMaxima();
	float getMagiaMaxima();
	void setDanoMaximo(float dano);
	void setPrecisionMinima(float precision);
	void setVidaMaxima(float vida);
	void setMagiaMaxima(float magia);

	void setFollowingEnemy(bool enemy);
	std::pair <int, int> getTarget();
	bool canSee(std::pair<int, int> tile);

	void herir();
	void morir();
	void hacerMagia();
	//void setPosition(std::pair<int, int> pos);

	void setCurrentWeaponIndex(unsigned int currentWeaponIndex);


private:

	//void eatIfItem(std::pair<int, int> destino);
	int siCaminaDetenerse();
	int cambiarEstado(int x, int y, int cambio);
	int obtenerOrientacionRespectoAUnTile(int x, int y);	//Obtiene el sentido seg�n el signo de x e y
	void orientar(std::pair<int, int> destino); //orienta el personaje para que mire hacia destino
	bool esNecesarioCalcularNuevoPath();
	void limpiarPath();
	int quedarseQuieto(float &velocidadAni);
	//Modifica las variables del modelo para representar el movimiento
	/*pair<int, int> moverse();*/
	void moverse(std::pair<int, int>& destino, float &velocidadAni);
	//void activarDesactivar();
	void defender();
	bool followingEnemy();
	

	bool perseguirEnemigo();
	void resolverAnimacion(int animacionNueva);
	void resetChar();

	void initialize(int pos_x, int pos_y);
	
	std::pair<int, int> startPosition;
	//std::pair<int, int> current;
	std::pair<int, int> target;
	std::pair<int, int> targetParcial;
	int* xPath;
	int* yPath;
	int posMov;
	int caminoSize;
	bool isReseting;
	bool isAnimating;
	int animacionActual;
	int estado;
	int orientacion;
	float velocidad;
	float velocidadInicial;
	bool active;
	AnimatedEntity * animation;
	string name;
	CharacterVision* vision;
	//bool isInCenterTile;
	bool following;

	std::vector<model::Weapon*> weapons;

	unsigned int currentWeaponIndex;

	std::vector<model::Weapon*>& getWeapons();

	//------------------------ATRIBUTOS (PONGAN ACA LOS ATRIBUTOS DEL PJ: VIDA, MAGIA, DA�O, ETC)------------------------
	float precisionMinima;
	float vidaMaxima;
	float magiaMaxima;
	float danoMaximo;
};


#endif