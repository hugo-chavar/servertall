#include <cstdlib>
#include "Logger.h"
#include "PersonajeModelo.h"
#include "Pathfinder.h"
#include "PersonajeConstantes.h"
#include "Game.h"

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
	target.first = current.first;
	target.second = current.second;
	targetParcial.first = current.first;
	targetParcial.second = current.second;
	xPath = NULL;
	yPath = NULL;
	posMov = 0;
	caminoSize = 0;
	estado = PARADO_S;
	velocidad = DEFAULT_MAIN_CHARACTER_SPEED;
	this->setIsActivo(true);
	orientacion = SUR;
	this->setAnimating(false);
	animacionActual = SIN_CAMBIO;
	this->vision = NULL;
	//Atributos que antes estaban en la vista
	delta.first = 0;
	delta.second = 0;
	ePot.first = 0;
	ePot.second = 0;
	serr = 0;
	velocidadAnim = velocidad;
	ultimoDeltaTime = 0;
}

// IMPLEMENTAR
string PersonajeModelo::toString() {
	return "";
}

void PersonajeModelo::setCurrent(int x, int y) {
	current.first = x;
	current.second = y;
}

void PersonajeModelo::setAnimation(AnimatedEntity* ae) {
	animation = ae;
}

void PersonajeModelo::atacar() {
	animacionActual = ATACAR;
	targetParcial = target = current;
	if (estado >= MOVIMIENTO) {
		estado = estado + ATACAR - MOVIMIENTO;
	} else {
		estado = estado + ATACAR - PARADO;
	}
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
	if ((isActivo)&&(animacionActual == SIN_CAMBIO)) {
		
		switch (opcion) {
		case ('a'): {
			this->setAnimating(true);
			this->atacar();
			break;
				  }
		case ('s'): {
			this->setAnimating(true);
			this->defender();
			break;
				  }
		default:;
		}
	}
}

void PersonajeModelo::terminarAnimacion() {
	this->setAnimating(false);
	estado = estado - animacionActual + PARADO;
	animacionActual = SIN_CAMBIO;
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

void PersonajeModelo::setDestino(int x, int y) {
	if ((isActivo)&&(!this->estaAnimandose())) {
		target.first = x;
		target.second = y;
		targetParcial.first = x;
		targetParcial.second = y;
	}
}

void PersonajeModelo::setEstado(int state) {
	estado = state;
}

void PersonajeModelo::setIsActivo(bool active) {
		isActivo = active;
}

//void PersonajeModelo::setIsActivo() {
//	activarDesactivar();
//	if (isActivo) {
//		isActivo = false;
//	} else {
//		isActivo = true;
//	}
//}
//
//void PersonajeModelo::activarDesactivar() {
//	if ((this->estaAnimando())) {
//		this->terminarAnimacion();
//	}
//	if (isActivo) {
//		if (estado >= MOVIMIENTO) {
//			estado = estado + FREEZAR - MOVIMIENTO;
//		} else {
//			estado = estado + FREEZAR - PARADO;
//		}
//		targetParcial = target = current;
//	} else {
//		estado = estado - FREEZAR + PARADO;
//	}
//}


bool PersonajeModelo::getIsActivo() {
	return isActivo;
}

void PersonajeModelo::setVelocidad(float vel) {
	velocidad = vel;
}

//void PersonajeModelo::getCurrent(std::pair<int, int>& actual) {
//	actual.first = current.first;
//	actual.second = current.second;
//}

int PersonajeModelo::getEstado() {
	return estado;
}

float PersonajeModelo::getVelocidad() {
	return velocidad;
}

int PersonajeModelo::siCaminaDetenerse() {
	int cambio = SIN_CAMBIO;
	
	if ((estado<10) || (estado>19)) { //si esta caminando
		cambio = ESTADO_MOVIMIENTO; //que se quede quieto
	}
	return cambio;
}

//pair <int, int> PersonajeModelo::mover(std::pair<int, int> destino) {
//	Pathfinder pathF;
//	int cambio = SIN_CAMBIO;
//	double coste = 0;
//	float costeF = 0;
//	pair <int, int> nextTile;
//
//	if (target == current) {
//		nextTile.first = -1;
//		nextTile.second = -1;
//		return nextTile;
//	}
//	if (esNecesarioCalcularNuevoPath()) {
//		posMov = 0;
//		caminoSize = 0;
//		limpiarPath();
//		targetParcial.first = target.first;
//		targetParcial.second = target.second;
//		caminoSize = pathF.getPath(current.first, current.second, targetParcial.first, targetParcial.second, xPath, yPath);
//		if (caminoSize == 0) { //Si no se tiene que mover, seteo el destino en los parciales
//			target.first = targetParcial.first;
//			target.second = targetParcial.second;
//		}
//		if (caminoSize <  0) {
//			nextTile.first = -1;
//			nextTile.second = -1;
//			return nextTile;
//		}
//	}
//	if (posMov < caminoSize) {
//		nextTile = this->moverse();
//		return nextTile;
//	} else {
//		nextTile.first = -1;
//		nextTile.second = -1;
//		return nextTile;
//	}
//}

int PersonajeModelo::obtenerSigTile(std::pair<int, int>& destino, float& velocidadAni) {
	Pathfinder pathF;
	int cambio = SIN_CAMBIO;
	double coste = 0;
	float costeF = 0;
	int estadoAnterior = estado;

	if (target == current) {
		this->quedarseQuieto(velocidadAni);
		return estadoAnterior;
	}
	if (esNecesarioCalcularNuevoPath()) {
		posMov = 0;
		caminoSize = 0;
		limpiarPath();
		targetParcial.first = target.first;
		targetParcial.second = target.second;
		caminoSize = pathF.getPath(current.first, current.second, targetParcial.first, targetParcial.second, xPath, yPath);
		if (caminoSize == 0) { //Si no se tiene que mover, seteo el destino en los parciales
			target.first = targetParcial.first;
			target.second = targetParcial.second;
		}
		if (caminoSize <  0) {
			this->quedarseQuieto(velocidadAni);
			return estadoAnterior;
		}
	}
	if (posMov < caminoSize) {
		this->moverse(destino, velocidadAni);
	} else {
		this->quedarseQuieto(velocidadAni);
		return estadoAnterior;
	}
	cambio = ESTADO_MOVIMIENTO;
	estado = cambiarEstado(destino.first, destino.second, cambio);
	return estadoAnterior;
}

bool PersonajeModelo::esNecesarioCalcularNuevoPath(){
	if ((xPath == NULL)&&(yPath == NULL)) { //Si no hay camino seteado
		return true;
	}
	if ((xPath[caminoSize-1]!=targetParcial.first)||(yPath[caminoSize-1]!=targetParcial.second)) { //Si cambio de destino durante el movimiento
		return true;
	}
	if ((posMov==caminoSize)&&((target.first!=targetParcial.first)||(target.second!=targetParcial.second))) { //Si completo el primer pedazo del camino
		return true;
	}
	return false;
}

//pair<int, int> PersonajeModelo::moverse(){
//	pair <int, int> destino;
//	destino.first = xPath[posMov];
//	destino.second = yPath[posMov];
//	posMov++;
//	return destino;
//}

void PersonajeModelo::moverse(std::pair<int, int>& destino, float &velocidad){
	double coste = 0;
	float costeF = 0;

	destino.first = xPath[posMov];
	destino.second = yPath[posMov];
	coste = (Game::instance().world())->cost(xPath[posMov], yPath[posMov]);
	costeF = (float) coste;
	velocidad = ((this->velocidad)*costeF);
	posMov++;
}

void PersonajeModelo::quedarseQuieto(float &velocidad){
	int cambio = SIN_CAMBIO;
	cambio = this->siCaminaDetenerse();
	estado = cambiarEstado(targetParcial.first, targetParcial.second, cambio);
	velocidad = 0;
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
		return (estado-FACTOR_ORIENTACION);
	}
	orientacion = obtenerOrientacion(x, y);
	switch (orientacion) {
	case NORTE: return CAMINANDO_N;
	case NORESTE: return CAMINANDO_NE;
	case NOROESTE: return CAMINANDO_NOE;
	case SUR: return CAMINANDO_S;
	case SUDESTE: return CAMINANDO_SE;
	case SUDOESTE: return CAMINANDO_SOE;
	case ESTE: return CAMINANDO_E;
	case OESTE: return CAMINANDO_O;
	default: return ESTADO_ERROR;
	}
}

int PersonajeModelo::obtenerOrientacion(int x, int y) {
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

//
//void PersonajeModelo::setRefPixelX(int) {
//	this->isAnimating = value;
//}
//
//void PersonajeModelo::setRefPixelY(int) {
//	this->isAnimating = value;
//}

int PersonajeModelo::getRefPixelX() {
	return this->animation->pixelRefX();
}

int PersonajeModelo::getRefPixelY() {
	return this->animation->pixelRefY();
}

bool PersonajeModelo::isCenteredInTile() {
	return ((delta.first == 0) && (delta.second == 0));
}

void PersonajeModelo::mover(unsigned int deltaTime) {
	if (this->isCenteredInTile() && ((this->isActivo) || !(this->estaAnimandose()))) {

		std::pair<float, float> factor;	//Cuantos pixels se mueve por ciclo
		factor.first = 0;
		factor.second = 0;
	
		calcularSigTileAMover();
		calcularvelocidadRelativa(factor, deltaTime);
		if (estado != ESTADO_ERROR) {
			moverSprite(factor);
		}
	}
}


void PersonajeModelo::calcularSigTileAMover(){
	//int animacion = 0;	//animacion del personaje en el sistema de PersonajeModelo
	std::pair<int, int> tile;	//Un tile
	int estadoAnterior = 0;

	if (this->isCenteredInTile()) {
		serr = 0;
		//tileActual = this->getPosition();
		//this->setIsInCenterTile(true);
		estadoAnterior = this->obtenerSigTile(tile, velocidadAnim);
		//estado = procesarAnimacion(animacion);
		if (estadoAnterior != estado) {
			ePot.first = 0;
			ePot.second = 0;
		} 
		//std::string aux = stringUtilities::floatToString(velocidad);
		//common::Logger::instance().log("Velocidad: "+ aux);
		if (velocidadAnim != 0) {
			//modelo->setIsInCenterTile(false);
			this->setCurrent(tile.first, tile.second);
		}
	}
}

void PersonajeModelo::moverSprite(std::pair<float, float>& factor){
	
	if (delta.first != 0) { //Hay movimieto en x
		ePot.first = ePot.first + factor.first;	//Aumento la cantidad de movimiento, cuantos pixels se va a mover
		moverSpriteEnX(); //Mueve en x
	}
	if (delta.second != 0) { //Si hay movimiento en y, y no esta activada la corrección en diagonal
		ePot.second = ePot.second + factor.second;									//O si esta activada la corrección pero se completo el movimineto en x
		moverSpriteEnY();															//Caso en que la velocidad no es entera
	}
}

void PersonajeModelo::calcularvelocidadRelativa(std::pair<float, float>& factor, unsigned int deltaTime) {
	float deltaTimeFlo = (float) deltaTime;
	float deltaTimeR = deltaTimeFlo;
	if (ultimoDeltaTime == 0) {
		ultimoDeltaTime = deltaTimeFlo;
	} else {
		deltaTimeR = deltaTimeFlo - ultimoDeltaTime;
		ultimoDeltaTime = deltaTimeFlo;
	}
	
	if (delta.first != 0){ //Hay movimiento en x
		if (delta.second != 0) { //Diagonal
			factor.first = static_cast<float>((velocidadAnim*deltaTimeR) *0.707);
			factor.second = static_cast<float>((velocidadAnim*deltaTimeR) *0.707/2);
		} else { //Horizontal
			factor.first = (velocidadAnim*deltaTimeR);
		}
	} else { //No hay movimiento en xS
		if (delta.second != 0){ //Vertical
			factor.second = ((velocidadAnim*deltaTimeR)/2);
		} else {//Quieto
			factor.first = 0;
			factor.second = 0;
		}

	}
}

void PersonajeModelo::moverSpriteEnX() {
	float factorT = 0;	//El truncamiento de la variable factor
	if (ePot.first >= 1) {	//Si la cantidad de movimiento es mayor a un pixel o mas
		//sprites[estado]->getNextFrame();
		if (delta.second != 0) { //Si también hay movimiento en y seteo el control del movimiento diagonal
			serr++;
		}
		factorT = std::floor(ePot.first);	//Trunco para obtener una cantidad entera
		ePot.first -= factorT;	//Saco la cantidad entera de la cantidad de movimiento
		if (delta.first < 0) {	//Si me muevo hacia las x negativas
			delta.first += factorT;
			if (delta.first > 0) {	//Si me paso, por los decimales
				pixelActual.first -= (factorT - delta.first); //Muevo el sprite en x
				ePot.first += delta.first;
				delta.first = 0;	//Termino el movimietno en x
			} else {				//Mientras siga pudiendo moverse
				pixelActual.first -= factorT;
			}
		} else {
			delta.first -= factorT;
			if (delta.first < 0) {
				pixelActual.first += (factorT + delta.first);
				ePot.first -= delta.first;
				delta.first = 0;
			} else {
				pixelActual.first += factorT;
			}
		}
	}
}

void PersonajeModelo::moverSpriteEnY() {
	float factorT = 0;	//El truncamiento de la variable factor
	
	if (((ePot.second >= 1)/*&&(serr != 1))||((serr == 1)&&(delta.first == 0)&&(ePot.second >= 1)*/)) {
		//if ((delta.first == 0)/*&&(serr != 1)*/) { // evito actualizar dos veces en diagonal
		//	sprites[estado]->getNextFrame();
		//}
		serr = 0;
		factorT = std::floor(ePot.second);
		ePot.second -= factorT;
		if (delta.second < 0) {
			delta.second += factorT;
			if (delta.second > 0) {
				pixelActual.second -= (factorT - delta.second);
				ePot.second += delta.second;
				delta.second = 0;
			} else {
				pixelActual.second -= factorT;
			}
		} else {
			delta.second -= factorT;
			if (delta.second < 0) {
				pixelActual.second += (factorT + delta.second);
				ePot.second -= delta.second;
				delta.second = 0;
			} else {
				pixelActual.second += factorT;
			}
		}
	}
}

//int PersonajeModelo::procesarAnimacion(int animacion) {
//	if ((animacion < MOVIMIENTO)||(animacion >= (MOVIMIENTO + FACTOR_ORIENTACION))) {
//		delta.first = 0;
//		delta.second = 0;
//	}
//	switch (animacion) {
//		//Parado
//	case PARADO_N: return STOP_N;
//	case PARADO_NE: return STOP_NE;
//	case PARADO_NOE: return STOP_NOE;
//	case PARADO_S: return STOP_S;
//	case PARADO_SE: return STOP_SE;
//	case PARADO_SOE: return STOP_SOE;
//	case PARADO_E: return STOP_E;
//	case PARADO_O: return STOP_O;
//		//Caminando
//	case CAMINANDO_N: { 
//						delta.first = 0;
//						delta.second = -32;
//						return WALK_N;
//					  }
//	case CAMINANDO_NE: {
//						delta.first = 32;
//						delta.second = -16;
//						return WALK_NE;
//					   }
//	case CAMINANDO_NOE: {
//						delta.first = -32;
//						delta.second = -16;
//						return WALK_NOE;
//						}
//	case CAMINANDO_S: {
//						delta.first = 0;
//						delta.second = 32;
//						return WALK_S;
//					  }
//	case CAMINANDO_SE: {
//						delta.first = 32;
//						delta.second = 16;
//						return WALK_SE;
//					   }
//	case CAMINANDO_SOE: {
//						delta.first = -32;
//						delta.second = 16;
//						return WALK_SOE;
//						}
//	case CAMINANDO_O: {
//						delta.first = -64;
//						delta.second = 0;
//						return WALK_O;
//					  }
//	case CAMINANDO_E: {
//						delta.first = 64;
//						delta.second = 0;
//						return WALK_E;
//					  }
//	case FREEZAR_N: return FREEZE_N;
//	case FREEZAR_NE: return FREEZE_NE;
//	case FREEZAR_NOE: return FREEZE_NOE;
//	case FREEZAR_S: return FREEZE_S;
//	case FREEZAR_SE: return FREEZE_SE;
//	case FREEZAR_SOE: return FREEZE_SOE;
//	case FREEZAR_E: return FREEZE_E;
//	case FREEZAR_O: return FREEZE_O;
//	case ATACAR_N: return ATTACK_N;
//	case ATACAR_NE: return ATTACK_NE;
//	case ATACAR_NOE: return ATTACK_NOE;
//	case ATACAR_S: return ATTACK_S;
//	case ATACAR_SE: return ATTACK_SE;
//	case ATACAR_SOE: return ATTACK_SOE;
//	case ATACAR_E: return ATTACK_E;
//	case ATACAR_O: return ATTACK_O;
//	case DEFENDER_N: return DEFEND_N;
//	case DEFENDER_NE: return DEFEND_NE;
//	case DEFENDER_NOE: return DEFEND_NOE;
//	case DEFENDER_S: return DEFEND_S;
//	case DEFENDER_SE: return DEFEND_SE;
//	case DEFENDER_SOE: return DEFEND_SOE;
//	case DEFENDER_E: return DEFEND_E;
//	case DEFENDER_O: return DEFEND_O;
//	default: return ESTADO_ERROR;
//	}
//}

float PersonajeModelo::getPixelX() {
	return (this->pixelActual.first);
}

float PersonajeModelo::getPixelY() {
	return (this->pixelActual.second);
}
//
//void PersonajeModelo::setRefPixelX(int) {
//	this->isAnimating = value;
//}
//
//void PersonajeModelo::setRefPixelY(int) {
//	this->isAnimating = value;
//}