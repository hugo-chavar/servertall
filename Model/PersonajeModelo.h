#ifndef PERSONAJEMODELO_H
#define PERSONAJEMODELO_H

#include <utility>
#include "Constants.h"
#include "DirList.h"
#include "AnimatedEntity.h"
#include "CharacterVision.h"

class Personaje;

class PersonajeModelo {

public:
	PersonajeModelo();
	PersonajeModelo(int ActualX, int ActualY);
	void setEstado(int state);
	void setCurrent(int x, int y);
	void setDestino(int x, int y);
	void setVelocidad(float vel);
	//void getCurrent(std::pair<int, int>& actual);
	void setActive(bool active);
	bool isActive();
	int getEstado();
	float getVelocidad();
	/*pair <int, int> mover(std::pair<int, int> destino);*/
	int mover(std::pair<int, int>& destino, float &velocidadAni);	//Devuelve el estado, que son las orientaciones. Devuelve la velocidad
	~PersonajeModelo();											//del movimiento, la corrección de la vista no se la hace
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
	std::pair<int, int> getPosition();
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
	void increaseVisionRange(float factor);
	void PersonajeModelo::discoverMap();

	bool getIsReseting();
	void setIsReseting();

private:

	int siCaminaDetenerse();
	int cambiarEstado(int x, int y, int cambio);
	int obtenerOrientacionRespectoAUnTile(int x, int y);	//Obtiene el sentido según el signo de x e y
	void orientar(std::pair<int, int> destino); //orienta el personaje para que mire hacia destino
	bool esNecesarioCalcularNuevoPath();
	void limpiarPath();
	int quedarseQuieto(float &velocidadAni);
	//Modifica las variables del modelo para representar el movimiento
	/*pair<int, int> moverse();*/
	void moverse(std::pair<int, int>& destino, float &velocidadAni);
	//void activarDesactivar();
	void atacar();
	void resolverAtaque();
	void recibirDano(float dano);
	void defender();
	void setCurrentEnemy(int tileX, int tileY);
	bool perseguirEnemigo();
	void resolverAnimacion(int animacionNueva);
	void herir();
	void morir();
	void resetChar();

	void initialize(int pos_x, int pos_y);
	
	std::pair<int, int> startPosition;
	std::pair<int, int> current;
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
	bool active;
	AnimatedEntity * animation;
	string name;
	CharacterVision* vision;
	Personaje* currentEnemy; //el nombre del enemigo que esta atacando el pj
	//bool isInCenterTile;

	//------------------------ATRIBUTOS (PONGAN ACA LOS ATRIBUTOS DEL PJ: VIDA, MAGIA, DAÑO, ETC)------------------------
	float precisionMinima;
	float danoMaximo;
	float vidaMaxima;
	float vidaActual;
};


#endif