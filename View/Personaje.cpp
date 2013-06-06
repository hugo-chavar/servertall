#include "Personaje.h"
#include "../Model/PersonajeConstantes.h"
#include "GameView.h"
#include "Logger.h"
#include "StringUtilities.h"

#include "../Model/Game.h"


Personaje::Personaje(PersonajeModelo* pj,std::string char_id) {
	modelo = pj;
	this->character_id=char_id;
	tileActual = pj->getPosition();
	this->setCurrentSpritePosition(this->calculateSpritePosition(pj->getEstado()));
	velocidad = pj->getVelocidad();
	delta.first = 0;
	delta.second = 0;
	ePot.first = 0;
	ePot.second = 0;
	serr = 0;
	currentEnemy = NULL;
	//crearNombre(modelo->getName());

	//this->modelo->getAnimation()->fps(static_cast<int>(this->modelo->getAnimation()->fps() * (this->modelo->getVelocidad()/2)));

	this->setFreezed(false);
	//this->setCenteredInTile(true);
	this->resetSpriteState();
}

void Personaje::loadSprites() {
	AnimatedEntity* animatedEntity;
	animatedEntity = new AnimatedEntity();
	animatedEntity->copy(modelo->getAnimation());
	modelo->restartDirectories();
	animatedEntity->loadImages(modelo->nextDirectory());
	this->addFirstSprite(animatedEntity);

	while (modelo->hasDirectoryRemaining()) {
		animatedEntity->loadImages(modelo->nextDirectory());
		this->addNextSprite(animatedEntity);
		animatedEntity->clearImages();
	}

	delete animatedEntity;
	animatedEntity = NULL;
}

void Personaje::clearSprites() {
	vector<SpriteAnimado*>::iterator it;
	for (it = sprites.begin(); it != sprites.end(); it++){
		delete *it;
	}
	sprites.clear();
}

void Personaje::addNextSprite(AnimatedEntity* entity) {
	SpriteAnimado* newSprite = new SpriteAnimado(entity);
	sprites.push_back(newSprite);
}

void Personaje::addFirstSprite(AnimatedEntity* entity) {
	SpriteAnimado* newSprite = new SpriteAnimado(entity);
	sprites.push_back(newSprite);
	this->setRectangle(tileActual, newSprite);
}

bool Personaje::isCenteredInTile() {
	return ((delta.first == 0) && (delta.second == 0));
}
//
//#pragma warning( push )
//#pragma warning( disable : 4100)
//void Personaje::setCenteredInTile(bool centroTile) {
//	centeredInTile = ((delta.first == 0) && (delta.second == 0));
//}
//#pragma warning( pop )

void Personaje::setFreezed(bool value) {
		//TODO:
	this->freezed = value;
	this->freezedSpriteState = -1;
	//Entity::setFreezed(value);
	//if (this->freezed == value)
	//	return;
	//this->freezed = value;
	//if (!this->isFreezed())
	//	this->freezedSpriteState = -1;
}
//
//void Personaje::resetSpriteState() {
//	this->freezedSpriteState = -1;
//}

void Personaje::detenerAnimacion() {
	modelo->terminarAnimacion();
	int currentAnimationNumber = modelo->getEstado();
	this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
}

void Personaje::animar() {

	if (!this->modelo->estaAnimandose() || this->freezed)
		return;
	int currentAnimationNumber = modelo->getEstado();
	if (this->calculateSpritePosition(currentAnimationNumber) != this->getCurrentSpritePosition()) {
		if (this->getCurrentSpritePosition() > static_cast<int>(sprites.size()-1)) {
			GameView::instance().getErrorImage()->restart();
		} else {
			sprites[this->getCurrentSpritePosition()]->restart();
		}
		if (this->calculateSpritePosition(currentAnimationNumber) > static_cast<int>(sprites.size()-1)) {
			GameView::instance().getErrorImage()->restart();
		} else {
			sprites[this->calculateSpritePosition(currentAnimationNumber)]->restart();
		}
	}
	this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
	if (this->currentSpritePosition > static_cast<int>(sprites.size()-1)) {
		if (GameView::instance().getErrorImage()->lastFrame())
			this->detenerAnimacion();
	} else {
		if (sprites[this->getCurrentSpritePosition()]->lastFrame())
			this->detenerAnimacion();
	}
}

void Personaje::update() {
	this->setFreezed(!modelo->isActive());
	this->mover();
	if (this->isCenteredInTile()) {
		this->animar();
	}
	modelo->update();
	if (this->freezed)
		return;
	if (this->getCurrentSpritePosition() > static_cast<int>(sprites.size()-1)) {
		GameView::instance().getErrorImage()->updateFrame();
	} else {
		sprites[this->currentSpritePosition]->updateFrame();
	}
}

void Personaje::mover() {
	if (this->isCenteredInTile() && (this->isFreezed() || this->modelo->estaAnimandose()))
		return;

	std::pair<float, float> factor;	//Cuantos pixels se mueve por ciclo
	factor.first = 0;
	factor.second = 0;
	
	perseguirEnemigo();
	calcularSigTileAMover();
	calcularvelocidadRelativa(factor);
	if (this->getCurrentSpritePosition() != ESTADO_ERROR) {
		moverSprite(factor);
	}
}


void Personaje::calcularSigTileAMover(){
	int currentAnimationNumber = 0;	//animacion del personaje en el sistema de PersonajeModelo
	std::pair<int, int> tile;	//Un tile
	int previousSpritePosition = this->getCurrentSpritePosition();

	if (this->isCenteredInTile()) {
		serr = 0;
		tileActual = modelo->getPosition();
		//modelo->setIsInCenterTile(true);
		currentAnimationNumber = modelo->mover(tile, velocidad);
		if (this->modelo->estaAnimandose())
			return;
		this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
		if (previousSpritePosition != this->currentSpritePosition) {
			ePot.first = 0;
			ePot.second = 0;
		} 
		//std::string aux = stringUtilities::floatToString(velocidad);
		//common::Logger::instance().log("Velocidad: "+ aux);
		if (velocidad != 0) {
			//modelo->setIsInCenterTile(false);
			modelo->setCurrent(tile.first, tile.second);
		} else {
			this->atacar();
		}
		if (modelo->getIsReseting()) {
			this->setRectangle(tileActual, sprites[this->currentSpritePosition]);
			currentEnemy = NULL;
			modelo->setIsReseting();
		}
	}
}

void Personaje::moverSprite(std::pair<float, float>& factor){
	
	if (delta.first != 0) { //Hay movimieto en x
		ePot.first = ePot.first + factor.first;	//Aumento la cantidad de movimiento, cuantos pixels se va a mover
		moverSpriteEnX(); //Mueve en x
	}
	if (delta.second != 0) { //Si hay movimiento en y, y no esta activada la corrección en diagonal
		ePot.second = ePot.second + factor.second;									//O si esta activada la corrección pero se completo el movimineto en x
		moverSpriteEnY();															//Caso en que la velocidad no es entera
	}
}

void Personaje::moverSpriteEnX() {
	float factorT = 0;	//El truncamiento de la variable factor
	if (ePot.first >= 1) {	//Si la cantidad de movimiento es mayor a un pixel o mas
		if (delta.second != 0) { //Si también hay movimiento en y seteo el control del movimiento diagonal
			serr++;
		}
		factorT = std::floor(ePot.first);	//Trunco para obtener una cantidad entera
		ePot.first -= factorT;	//Saco la cantidad entera de la cantidad de movimiento
		if (delta.first < 0) {	//Si me muevo hacia las x negativas
			delta.first += factorT;
			if (delta.first > 0) {	//Si me paso, por los decimales
				spriteRect.x -= (Sint16) (factorT - delta.first); //Muevo el sprite en x
				ePot.first += delta.first;
				delta.first = 0;	//Termino el movimietno en x
			} else {				//Mientras siga pudiendo moverse
				spriteRect.x -= (Sint16) factorT;
			}
		} else {
			delta.first -= factorT;
			if (delta.first < 0) {
				spriteRect.x += (Sint16)(factorT + delta.first);
				ePot.first -= delta.first;
				delta.first = 0;
			} else {
				spriteRect.x += (Sint16)factorT;
			}
		}
	}
}

void Personaje::moverSpriteEnY() {
	float factorT = 0;	//El truncamiento de la variable factor
	
	if (((ePot.second >= 1)/*&&(serr != 1))||((serr == 1)&&(delta.first == 0)&&(ePot.second >= 1)*/)) {
		serr = 0;
		factorT = std::floor(ePot.second);
		ePot.second -= factorT;
		if (delta.second < 0) {
			delta.second += factorT;
			if (delta.second > 0) {
				spriteRect.y -= (Sint16)(factorT - delta.second);
				ePot.second += delta.second;
				delta.second = 0;
			} else {
				spriteRect.y -= (Sint16)factorT;
			}
		} else {
			delta.second -= factorT;
			if (delta.second < 0) {
				spriteRect.y += (Sint16)(factorT + delta.second);
				ePot.second -= delta.second;
				delta.second = 0;
			} else {
				spriteRect.y += (Sint16)factorT;
			}
		}
	}
}



void Personaje::recibirDano(float dano) {
	float vida = 0;
	
	this->modelo->reduceVidaActual(dano*(-1));
	vida = this->modelo->getVidaActual();
	if (vida > 0) {
		this->modelo->herir();
	} else {
		this->modelo->morir();
	}
}

void Personaje::resolverAtaque(){
	float precision = Game::instance().getRandom();
	if (precision >= this->modelo->getPrecisionMinima()) {
		float dano = Game::instance().getRandom() * this->modelo->getDanoMaximo();
		this->currentEnemy->recibirDano(dano);
	}
}

void Personaje::atacar() {
	if ((currentEnemy != NULL) && (currentEnemy->getPosicionAnteriorEnTiles() == this->modelo->obtenerFrentePersonaje()) && (!currentEnemy->isFreezed())) {
		this->resolverAtaque();
		this->modelo->atacar();
		currentEnemy = NULL;
	}
}

void Personaje::setCurrentEnemy(int tileX, int tileY) {
	std::pair<int, int> tileDestino(tileX, tileY);

	if (modelo->isThereAnEnemy(tileX, tileY)) {
		currentEnemy = GameView::instance().getCharInTile(tileDestino);
		if (currentEnemy == this) {
			currentEnemy = NULL;
		}
	}
}

void Personaje::perseguirEnemigo() {
	
	if (currentEnemy == NULL) {
		this->modelo->setFollowingEnemy(false);
		return;
	}
	if ((currentEnemy->getPosicionAnteriorEnTiles() != modelo->getTarget()) && (modelo->canSee(currentEnemy->getPosicionAnteriorEnTiles()))) {
		setDestino(currentEnemy->getPosicionAnteriorEnTiles().first, currentEnemy->getPosicionAnteriorEnTiles().second);
		this->modelo->setFollowingEnemy(true);
		return;
	}
	if (!modelo->canSee(currentEnemy->getPosicionAnteriorEnTiles())) {
		currentEnemy = NULL;
	}
	this->modelo->setFollowingEnemy(false);
}

void Personaje::setDestino(int xTile, int yTile){
	modelo->setDestino(xTile, yTile);
	setCurrentEnemy(xTile, yTile);
}

void Personaje::animateModel(char animacion) {
	modelo->animar(animacion);
}

void Personaje::calcularvelocidadRelativa(std::pair<float, float>& factor) {
	float deltaTime = GameView::instance().getTimer()->getDeltaTime()/10;
	if (delta.first != 0) { //Hay movimiento en x
		if (delta.second != 0) { //Diagonal
			factor.first = static_cast<float>((velocidad*deltaTime) *0.707);
			factor.second = static_cast<float>((velocidad*deltaTime) *0.707/2);
		} else { //Horizontal
			factor.first = (velocidad*deltaTime);
		}
	} else { //No hay movimiento en x
		if (delta.second != 0){ //Vertical
			factor.second = ((velocidad*deltaTime)/2);
		} else {//Quieto
			factor.first = 0;
			factor.second = 0;
		}

	}
}

int Personaje::calculateSpritePosition(int currentAnimationNumber) {
	int orientacion = modelo->getOrientacion();
	
	if ((currentAnimationNumber != MOVIMIENTO)) {
		delta.first = 0;
		delta.second = 0;
		if ((orientacion < 0)||(orientacion > 7)) {
			return ESTADO_ERROR;
		}
	} else {
		switch (orientacion) {
		case NORTE: {
			delta.first = 0;
			delta.second = -32;
			break;
					}
		case NORESTE: {
			delta.first = 32;
			delta.second = -16;
			break;
					  }
		case NOROESTE: {
			delta.first = -32;
			delta.second = -16;
			break;
						}
		case SUR: {
			delta.first = 0;
			delta.second = 32;
			break;
					}
		case SUDESTE: {
			delta.first = 32;
			delta.second = 16;
			break;
						}
		case SUDOESTE: {
			delta.first = -32;
			delta.second = 16;
			break;
						}
		case OESTE: {
			delta.first = -64;
			delta.second = 0;
			break;
					}
		case ESTE: {
			delta.first = 64;
			delta.second = 0;
			break;
					}
		default: return ESTADO_ERROR;
		}
	}
	return ((currentAnimationNumber - 1)*8 + orientacion);
}

Personaje::~Personaje(){
	if (sprites.size() > 0) {
		this->clearSprites();
	}
}

PersonajeModelo* Personaje::personajeModelo(){
	return modelo;
}

std::pair<int,int> Personaje::getPosicionEnTiles(){
	return modelo->getPosition();
}

std::pair<int,int> Personaje::getPosicionAnteriorEnTiles(){
	float deltaAbsX = std::abs(delta.first);
	float deltaAbsY = std::abs(delta.second);

	if ((deltaAbsX <= 32) && (deltaAbsY == 0)) {
		return this->getPosicionEnTiles();
	}
	if ((deltaAbsX == 0) && (deltaAbsY <= 16)) {
		return this->getPosicionEnTiles();
	}
	if ((deltaAbsX <= 16) && (deltaAbsY <= 8)) {
		return this->getPosicionEnTiles();
	}
	return tileActual;
}

//tilex, tiley; pixelx, pixely; isFreezed; nro_status; nro_surface; character_id
std::string Personaje::updateToString() {
	std::string out = "";
	if (this->getCurrentSpritePosition() >= 0){
		out = stringUtilities::pairIntToString(modelo->getPosition());
		out.append(";");
		out.append(stringUtilities::pairIntToString(this->getPixelPosition()));
		out.append(";");
		if (this->isFreezed()) {
			out.append("F");
		} else {
			out.append("N");
		}
		out.append(";");
		out.append(stringUtilities::intToString(this->getCurrentSpritePosition()));
		out.append(";");
		if (this->getCurrentSpritePosition() > static_cast<int>(sprites.size()-1)) {
			out.append(stringUtilities::intToString(GameView::instance().getErrorImage()->getCurrentState()));
		} else {
			out.append(stringUtilities::intToString(sprites[this->getCurrentSpritePosition()]->getCurrentState()));
		}
		//out.append(";");
		//out.append(this->character_id);
		out.append(";");
		if (this->isCenteredInTile()) {
			out.append("T");
		} else {
			out.append("F");
		}
	}
	return out;
}

//tilex, tiley; pixelx, pixely; isFreezed; nro_status; nro_surface,character_id
void Personaje::updateFromString(std::string data) {
	vector <std::string> splittedData;
	stringUtilities::splitString(data, splittedData, ';');
	std::pair<int,int> tilePosition = stringUtilities::stringToPairInt(splittedData[0]);
	std::pair<int,int> pixels = stringUtilities::stringToPairInt(splittedData[1]);
	this->setFreezed(splittedData[2] == "F");
	//this->setCurrentSpritePosition(stringUtilities::stringToInt(splittedData[3]));
	if (this->getCurrentSpritePosition() > static_cast<int>(sprites.size()-1)) {
		GameView::instance().getErrorImage()->setCurrentState(stringUtilities::stringToInt(splittedData[3]));
	} else {
		sprites[this->getCurrentSpritePosition()]->setCurrentState(stringUtilities::stringToInt(splittedData[3]));
	}
	//this->setCenteredInTile(splittedData[4] == "T");
}

int Personaje::getCurrentSpritePosition() {
	return this->currentSpritePosition;
}

void Personaje::setCurrentSpritePosition(int pos) {
	if (pos == ESTADO_ERROR) {
		this->currentSpritePosition = 0;
	}
	this->currentSpritePosition = pos;
}

std::pair<int,int> Personaje::getPixelPosition() {
	return std::make_pair< int, int >(spriteRect.x, spriteRect.y);
}

void Personaje::setPixelPosition(std::pair<int,int> pixel) {
	spriteRect.x = static_cast< Sint16 >(pixel.first);
	spriteRect.y = static_cast< Sint16 >(pixel.second);
}

//tilex,tiley;pixelx,pixely;isFreezed;nro_status;nro_surface~datos_vision
std::string Personaje::initToString() {
	std::string out = this->updateToString();
	out.append("~");
	out.append(this->modelo->getVision()->toString());
	return out;
}

//tilex,tiley;pixelx,pixely;isFreezed;nro_status;nro_surface~datos_vision
void Personaje::initFromString(std::string data) {
	vector <std::string> splittedData;
	stringUtilities::splitString(data, splittedData, '~');
	this->updateFromString(splittedData[0]);
	this->modelo->getVision()->fromString(splittedData[1]);
}

void Personaje::setPlayerName(std::string name) {
	this->playerName = name;
}

std::string Personaje::getPlayerName() {
	return this->playerName;
}

std::string Personaje::getCharacterId() {
	return this->character_id;
}

std::string Personaje::idToString() {
	std::string out = this->getPlayerName();
	out.append(";");
	out.append(modelo->getName());
	//out.append(";");
	//out.append(stringUtilities::pairIntToString(modelo->getPosition()));
	return out;
}