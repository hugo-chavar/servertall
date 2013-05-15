#ifndef PERSONAJEMODELO_H
#define PERSONAJEMODELO_H

#include <utility>
#include "Constants.h"
#include "DirList.h"
#include "AnimatedEntity.h"
#include "CharacterVision.h"


class PersonajeModelo {

public:
	PersonajeModelo();
	PersonajeModelo(int ActualX, int ActualY);
	void setEstado(int state);
	void setCurrent(int x, int y);
	void setDestino(int x, int y);
	void setVelocidad(float vel);
	//void getCurrent(std::pair<int, int>& actual);
	void setIsActivo(bool active);
	bool getIsActivo();
	int getEstado();
	float getVelocidad();
	//pair <int, int> mover(std::pair<int, int> destino);
	/*int mover(std::pair<int, int>& destino, float &velocidadAni);*/	//Devuelve el estado, que son las orientaciones. Devuelve la velocidad
	void mover(unsigned int deltaTime);
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
	string toString();

	//Getters de pixel
	float getPixelX();
	float getPixelY();
	//void setRefPixelX(int);
	//void setRefPixelY(int);
	int getRefPixelX();
	int getRefPixelY();

private:

	int siCaminaDetenerse();
	int cambiarEstado(int x, int y, int cambio);
	int obtenerOrientacion(int x, int y);	//Obtiene el sentido según el signo de x e y
	bool esNecesarioCalcularNuevoPath();
	void limpiarPath();
	void quedarseQuieto(float &velocidadAni);
	//Modifica las variables del modelo para representar el movimiento
	//pair<int, int> moverse();
	int obtenerSigTile(std::pair<int, int>& destino, float &velocidadAni);
	void moverse(std::pair<int, int>& destino, float &velocidadAni);
	//void activarDesactivar();
	void atacar();
	void defender();

	void initialize(int pos_x, int pos_y);
	//Metodos que antes estaban en la vista
	bool isCenteredInTile();
	void calcularSigTileAMover();
	void moverSprite(std::pair<float, float>& factor);
	void moverSpriteEnX();
	void moverSpriteEnY();
	/*int procesarAnimacion(int animacion);*/
	void calcularvelocidadRelativa(std::pair<float, float>& factor, unsigned int deltaTime);


	std::pair<int, int> current;
	std::pair<int, int> target;
	std::pair<int, int> targetParcial;
	int* xPath;
	int* yPath;
	int posMov;
	int caminoSize;
	bool isAnimating;
	int animacionActual;
	int estado;
	int orientacion;
	float velocidad;
	bool isActivo;
	AnimatedEntity * animation;
	string name;
	CharacterVision* vision;
	//Atributos que antes estaban en la vista
	std::pair<float, float> delta; //Cuanto recorrio en x y en y del movimiento
	std::pair<float, float> ePot; //Cuanto se mueve por ciclo en x y en y
	int serr; //variable usada para coordinar el movimiento en x y en y en la diagonal
	float velocidadAnim;
	
	std::pair <float, float> pixelActual;
	float ultimoDeltaTime;
};


#endif