#include <cstdlib>
#include "Logger.h"
#include "PersonajeModelo.h"
#include "Pathfinder.h"
#include "PersonajeConstantes.h"
#include "OpcionesJuego.h"
#include "Game.h"
#include "../View/GameView.h"
#include "../View/Personaje.h"

using namespace common;

PersonajeModelo::PersonajeModelo() {
	this->initialize(DEFAULT_MAIN_CHARACTER_X, DEFAULT_MAIN_CHARACTER_Y);
}

PersonajeModelo::PersonajeModelo(int ActualX, int ActualY) {
	this->initialize(ActualX, ActualY);
}

void PersonajeModelo::initialize(int pos_x, int pos_y) {
	current.first = pos_x;
	current.second = pos_y;
	startPosition.first = pos_x;
	startPosition.second = pos_y;
	target.first = current.first;
	target.second = current.second;
	targetParcial.first = current.first;
	targetParcial.second = current.second;
	xPath = NULL;
	yPath = NULL;
	posMov = 0;
	caminoSize = 0;
	estado = PARADO;
	velocidad = DEFAULT_MAIN_CHARACTER_SPEED;
	this->setActive(true);
	orientacion = SUR;
	this->setAnimating(false);
	animacionActual = SIN_CAMBIO;
	this->vision = NULL;
	following = false;
	precisionMinima = DEFAULT_CHARACTER_MIN_PRECISION;
	danoMaximo = DEFAULT_CHARACTER_MAX_DAMAGE;
	vidaMaxima = DEFAULT_CHARACTER_MAX_LIFE;
	magiaMaxima = DEFAULT_CHARACTER_MAX_MAGIC;
	isReseting = false;
}


void PersonajeModelo::setCurrent(int x, int y) {
	current.first = x;
	current.second = y;
}

void PersonajeModelo::setAnimation(AnimatedEntity* ae) {
	animation = ae;
}

void PersonajeModelo::herir() {
	animacionActual = HERIR;
	this->resolverAnimacion(animacionActual);
}

void PersonajeModelo::morir() {
	animacionActual = MORIR;
	this->resolverAnimacion(animacionActual);
}

void PersonajeModelo::resolverAnimacion(int nuevaAnimacion) {
	this->setAnimating(true);
	targetParcial = target = current;
	if (estado >= MOVIMIENTO) {
		estado = estado + nuevaAnimacion - MOVIMIENTO;
	} else {
		estado = estado + nuevaAnimacion - PARADO;
	}
}

float PersonajeModelo::getDanoMaximo() {
	return this->danoMaximo;
}

float PersonajeModelo::getPrecisionMinima() {
	return this->precisionMinima;
}

void PersonajeModelo::atacar() {
		animacionActual = ATACAR;
		this->resolverAnimacion(animacionActual);
}

void PersonajeModelo::defender() {
	animacionActual = DEFENDER;
	targetParcial = target = current;
	if (estado >= MOVIMIENTO) {
		estado = estado + DEFENDER - MOVIMIENTO;
	} else {
		estado = estado + DEFENDER - PARADO;
	}
}

bool PersonajeModelo::estaAnimandose() {
	return this->isAnimating;
}

void PersonajeModelo::animar(char opcion) {
	if ((this->isActive()) && (animacionActual == SIN_CAMBIO)) {
		
		switch (opcion) {
		/*case (OPCION_ATACAR): {
			this->setAnimating(true);
			this->atacar();
			break;
				  }*/
		case (OPCION_DEFENDER): {
			this->setAnimating(true);
			this->defender();
			break;
				  }
		default:;
		}
	}
}

void PersonajeModelo::resetChar() {
	current = target = targetParcial = startPosition;
	if (xPath == NULL) {
		delete [] xPath;
		xPath = NULL;
	}
	if (yPath == NULL) {
		delete [] yPath;
		yPath = NULL;
	}
	posMov = 0;
	caminoSize = 0;
	estado = PARADO;
	velocidad = DEFAULT_MAIN_CHARACTER_SPEED;
	orientacion = SUR;
	this->setAnimating(false);
	animacionActual = SIN_CAMBIO;
	following = false;
	precisionMinima = DEFAULT_CHARACTER_MIN_PRECISION;
	danoMaximo = DEFAULT_CHARACTER_MAX_DAMAGE;
	vidaMaxima = DEFAULT_CHARACTER_MAX_LIFE;
	magiaMaxima = DEFAULT_CHARACTER_MAX_MAGIC;
	isReseting = true;
}

bool PersonajeModelo::getIsReseting(){
	return isReseting;
}

void PersonajeModelo::setIsReseting(){
	isReseting = false;
}

void PersonajeModelo::terminarAnimacion() {
	this->setAnimating(false);
	if (animacionActual == MORIR) {
		this->resetChar();
	} else {
		estado = estado - animacionActual + PARADO;
		animacionActual = SIN_CAMBIO;
	}
}


bool PersonajeModelo::hasDirectoryRemaining(){
	return animation->hasNextDir();
}

AnimatedEntity* PersonajeModelo::getAnimation() {
	return animation;
}

string PersonajeModelo::nextDirectory() {
	return animation->nextDirectory();
}

int PersonajeModelo::fps() {
	return animation->fps();
}

int PersonajeModelo::delay() {
	return animation->delay();
}

bool PersonajeModelo::isThereAnEnemy(int tileX, int tileY) {
	std::pair<int, int> tileDestino(tileX, tileY);

	if ((!this->isActive())||(this->estaAnimandose())) {
		return false;
	}
	if ((vision != NULL) && (vision->isInsideVision(tileDestino)) && (GameView::instance().getDaniableInTile(tileDestino) != NULL)) {
		return true;
	}
	return false;
}

void PersonajeModelo::setDestino(int x, int y) {
	if ((this->isActive())&&(!this->estaAnimandose())) {
		target.first = x;
		target.second = y;
		targetParcial.first = x;
		targetParcial.second = y;
	}
}

void PersonajeModelo::setEstado(int state) {
	this->estado = state;
}

void PersonajeModelo::setActive(bool value) {
		this->active = value;
}

bool PersonajeModelo::isActive() {
	return this->active;
}

void PersonajeModelo::setVelocidad(float vel) {
	this->velocidad = vel;
}

int PersonajeModelo::getEstado() {
	return this->estado;
}

float PersonajeModelo::getVelocidad() {
	return velocidad;
}

int PersonajeModelo::siCaminaDetenerse() {
	int cambio = SIN_CAMBIO;
	
	if (estado == MOVIMIENTO) { //si esta caminando
		cambio = ESTADO_MOVIMIENTO; //que se quede quieto
	}
	return cambio;
}

float PersonajeModelo::getVidaMaxima() {
	return vidaMaxima;
}

float PersonajeModelo::getMagiaMaxima() {
	return magiaMaxima;
}

void PersonajeModelo::setDanoMaximo(float dano) {
	danoMaximo = dano;
}

void PersonajeModelo::setPrecisionMinima(float precision) {
	precisionMinima = precision;
}

void PersonajeModelo::setMagiaMaxima(float magia) {
	magiaMaxima = magia;
}

void PersonajeModelo::setVidaMaxima(float vida) {
	vidaMaxima = vida;
}

int PersonajeModelo::mover(std::pair<int, int>& destino, float& velocidadAni) {
	Pathfinder pathF;
	int cambio = SIN_CAMBIO;
	double coste = 0;
	float costeF = 0;

	if (target == current) {
		return (this->quedarseQuieto(velocidadAni));
	}
	if (esNecesarioCalcularNuevoPath()) {
		posMov = 0;
		caminoSize = 0;
		limpiarPath();
		targetParcial.first = target.first;
		targetParcial.second = target.second;
		caminoSize = pathF.getPath(current.first, current.second, targetParcial.first, targetParcial.second, xPath, yPath);
		if (caminoSize == 0) { //Si no se tiene que mover, seteo el destino en los parciales
			this->orientar(target);
			target.first = targetParcial.first;
			target.second = targetParcial.second;
		}
		if (caminoSize <  0) {
			return (this->quedarseQuieto(velocidadAni));
		}
	}
	if (posMov < caminoSize) {
		this->moverse(destino, velocidadAni);
	} else {
		return (this->quedarseQuieto(velocidadAni));
	}
	cambio = ESTADO_MOVIMIENTO;
	estado = cambiarEstado(destino.first, destino.second, cambio);
	return estado;
}

void PersonajeModelo::orientar(std::pair<int, int> destino) {
	int cambio = ESTADO_MOVIMIENTO;
	estado = cambiarEstado(destino.first, destino.second, cambio);
}

int PersonajeModelo::getOrientacion() {
	return this->orientacion;
}

bool PersonajeModelo::esNecesarioCalcularNuevoPath(){
	if ((xPath == NULL)&&(yPath == NULL)) { //Si no hay camino seteado
		return true;
	}
	if ((xPath[caminoSize-1]!=targetParcial.first)||(yPath[caminoSize-1]!=targetParcial.second)) { //Si cambio de destino durante el movimiento
		return true;
	}
	if (posMov >= 5/*this->getVision()->getRangeVision()*/) { //Si se movio el maximo antes de recalcular
		return true;
	}
	if ((posMov==caminoSize)&&((target.first!=targetParcial.first)||(target.second!=targetParcial.second))) { //Si completo el primer pedazo del camino
		return true;
	}
	if (followingEnemy()) {
		return true;
	}
	if ((Game::instance().world())->cost(xPath[posMov], yPath[posMov]) == 0) { //Hay un pj en el tile al que se va a mover
		return true;
	}
	return false;
}

bool PersonajeModelo::followingEnemy() {
	return following;
}

void PersonajeModelo::setFollowingEnemy(bool enemy) {
	following = enemy;
}

bool PersonajeModelo::canSee(std::pair<int, int> tile) {
	return vision->isInsideVision(tile);
}

std::pair<int, int> PersonajeModelo::getTarget() {
	return target;
}

void PersonajeModelo::moverse(std::pair<int, int>& destino, float &velocidad){
	double coste = 0;
	float costeF = 0;

	destino.first = xPath[posMov];
	destino.second = yPath[posMov];
	coste = (Game::instance().world())->cost(xPath[posMov], yPath[posMov]);
	costeF = (float) coste;
	velocidad = ((this->velocidad)*costeF);
	posMov++;
	//this->eatIfItem(destino);
}

int PersonajeModelo::quedarseQuieto(float &velocidad){
	int cambio = SIN_CAMBIO;
	cambio = this->siCaminaDetenerse();
	estado = cambiarEstado(targetParcial.first, targetParcial.second, cambio);
	velocidad = 0;
	return estado;
}

void PersonajeModelo::limpiarPath() {
	if (xPath != NULL) {
		delete [] xPath;
		xPath = NULL;
	}
	if (yPath != NULL) {
		delete [] yPath;
		yPath = NULL;
	}
}

int PersonajeModelo::cambiarEstado(int x, int y, int cambio) {
	if (cambio == SIN_CAMBIO) {
		return estado;
	}
	if((x==current.first)&&(y==current.second)&&(cambio==ESTADO_MOVIMIENTO)){
		return (estado-PARADO);
	}
	orientacion = obtenerOrientacionRespectoAUnTile(x, y);
	if ((orientacion >= 0)&&(orientacion <= 7)) {
		return MOVIMIENTO;
	} else {
		return ESTADO_ERROR;
	}
}

int PersonajeModelo::obtenerOrientacionRespectoAUnTile(int x, int y) {
	int xCurr = current.first;
	int yCurr = current.second;
	
	if ((x < xCurr)&&(y < yCurr)) {
		return NORTE;
	}
	if ((x == xCurr)&&(y < yCurr)) {
		return NORESTE;
	}
	if ((x > xCurr)&&(y < yCurr)) {
		return ESTE;
	}
	if ((x < xCurr)&&(y == yCurr)) {
		return NOROESTE;
	}
	if ((x > xCurr)&&(y == yCurr)) {
		return SUDESTE;
	}
	if ((x < xCurr)&&(y > yCurr)) {
		return OESTE;
	}
	if ((x == xCurr)&&(y > yCurr)) {
		return SUDOESTE;
	}
	if ((x > xCurr)&&(y > yCurr)) {
		return SUR;
	}
	return ESTADO_ERROR;
}

PersonajeModelo::~PersonajeModelo(){
	this->limpiarPath();
	if (this->vision)
		delete this->vision;
}

void PersonajeModelo::setName(string nombreJugador) {
	this->name = nombreJugador;
}

string PersonajeModelo::getName() {
	return this->name;
}

std::pair<int, int> PersonajeModelo::getPosition() {
	return this->current;
}

void PersonajeModelo::createVision(int range) {
	this->vision = new CharacterVision();
	this->vision->setRangeVision(range);
	this->vision->setPosition(this->getPosition());
	this->vision->initialize();
}

CharacterVision* PersonajeModelo::getVision() {
	return this->vision;
}

void PersonajeModelo::update() {
	this->vision->updatePosition(this->getPosition());
}

std::pair<int, int> PersonajeModelo::obtenerFrentePersonaje() {
	std::pair <int, int> posicionSig = current;

	if ((orientacion == NORTE) || (orientacion == NORESTE) || (orientacion == ESTE))
		posicionSig.second--;
	else if ((orientacion == SUDOESTE) || (orientacion == OESTE) || (orientacion == SUR))
		posicionSig.second++;

	if ((orientacion == NORTE) || (orientacion == NOROESTE) || (orientacion == OESTE))
		posicionSig.first--;
	else if ((orientacion == ESTE) || (orientacion == SUDESTE) || (orientacion == SUR))
		posicionSig.first++;
	
	return posicionSig;
}

void PersonajeModelo::setAnimating(bool value) {
	this->isAnimating = value;
}

void PersonajeModelo::restartDirectories() {
	this->animation->imagesPaths()->restartCurrentPosition();
}

void PersonajeModelo::increaseSpeed(float factor)
{
	this->velocidad=this->velocidad*factor;
	if(this->velocidad>MAX_MAIN_CHARACTER_SPEED)
	{
		this->velocidad=MAX_MAIN_CHARACTER_SPEED;
	}
}

void PersonajeModelo::setPosition(std::pair<int, int> pos) {
	this->current = pos;
}

//void PersonajeModelo::discoverMap()
//{
//	//this->getVision()->setHoleMapVision(); FALTA IMPLEMENTAR
//}
//
//void PersonajeModelo::increaseVisionRange(float factor)
//{
//	this->vision->setRangeVision(static_cast<int>(this->vision->getRangeVision()*factor));
//	if(this->vision->getRangeVision()>MAX_VISION_RANGE)
//	{
//		this->vision->setRangeVision(MAX_VISION_RANGE);
//	}
//}

//---------------------------Maybe useful in the future--------------------------------------

//void PersonajeModelo::setRefPixelX(int) {
//	this->isAnimating = value;
//}
//
//void PersonajeModelo::setRefPixelY(int) {
//	this->isAnimating = value;
//}
//
//int PersonajeModelo::getRefPixelX() {
//	return this->animation->pixelRefX();
//}
//
//int PersonajeModelo::getRefPixelY() {
//	return this->animation->pixelRefY();
//}

//void PersonajeModelo::eatIfItem(std::pair<int, int> destino)
//{	
//	Item * item=Game::instance().world()->getTileAt(destino)->getItem();
//	if(item!=NULL)
//	{
//	if(item->isAlive())
//		{
//		item->modifyCharacter(this);
//		item->kill();
//		}
//	}
//}