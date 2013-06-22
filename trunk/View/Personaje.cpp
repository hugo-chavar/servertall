#include "Personaje.h"
#include "../Model/PersonajeConstantes.h"
#include "GameView.h"
#include "Logger.h"
#include "StringUtilities.h"
#include "Sword.h"
#include "Bow.h"
#include "HandGrenade.h"
#include "BombDropper.h"
#include "WeaponIceIncantator.h"
#include "../Model/OpcionesJuego.h"
#include "../Model/Game.h"


Personaje::Personaje(PersonajeModelo* pj,std::string char_id) {
	modelo = pj;
	this->character_id=char_id;
	this->setPosition(pj->getPosition());
	this->setCurrentSpritePosition(this->calculateSpritePosition(pj->getEstado()));
	velocidad = pj->getVelocidad();
	delta.first = 0;
	delta.second = 0;
	ePot.first = 0;
	ePot.second = 0;
	serr = 0;
	currentEnemy = NULL;
	vidaActual = modelo->getVidaMaxima();
	magiaActual = modelo->getMagiaMaxima();
	this->shieldResistance=0;
	this->shieldAbsortion=0;
	hechizoActual = NULL;
	invulnerable = false;
	protCost = 0;
	protTime = 0;
	vidCost = 0;
	vidTime = 0;
	videncia = false;
	this->setIceSpell(false);

	this->setFogged(false);
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
	this->setRectangle(this->getPosition(), newSprite);
}

bool Personaje::isCenteredInTile() {
	return ((delta.first == 0) && (delta.second == 0));
}

void Personaje::detenerAnimacion() {
	modelo->terminarAnimacion();
	int currentAnimationNumber = modelo->getEstado();
	this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
}

void Personaje::animar() {
	if (!this->modelo->estaAnimandose() || this->isImmobilized())
		return;
	int currentAnimationNumber = modelo->getEstado();
	if (this->calculateSpritePosition(currentAnimationNumber) != this->getCurrentSpritePosition()) {
		if (!this->hasValidSprite()) {
			GameView::instance().getErrorImage()->restart();
		} else {
			sprites[this->getCurrentSpritePosition()]->restart();
		}
		if (!this->isThisSpriteValid(currentAnimationNumber)) {
			GameView::instance().getErrorImage()->restart();
		} else {
			sprites[this->calculateSpritePosition(currentAnimationNumber)]->restart();
		}
	}
	this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
	if (!this->hasValidSprite()) {
		if (GameView::instance().getErrorImage()->lastFrame())
			this->detenerAnimacion();
	} else {
		if (sprites[this->getCurrentSpritePosition()]->lastFrame())
			this->detenerAnimacion();
	}
}

void Personaje::stopCrystallBall() {
	if (this->videncia) {
		vidCost = 0;
		vidTime = 0;
		videncia = false;
	}
}

void Personaje::stopProtectionSpell() {
	if (this->invulnerable) {
		invulnerable = false;
		protCost = 0;
		protTime = 0;
	}
}

void Personaje::setVidCost(float cost) {
	vidCost = cost;
}

void Personaje::setVidTime(float time) {
	vidTime = time;
}

void Personaje::setVidencia(bool vid) {
	videncia = vid;
}

void Personaje::setProtTime(float time) {
	protTime = time;
}

void Personaje::stopMagic() {
	if (this->videncia) {
		stopCrystallBall();
	}
	if (this->invulnerable) {
		stopProtectionSpell();
	}
}

void Personaje::updateCrystallBall() {
	float tiempoTranscurrido = 0.0;
	
	if (this->videncia) {
		vidTime += GameView::instance().getTimer()->getDeltaTime()/1000;
	}
	if (vidTime > 1.0) {
		tiempoTranscurrido = std::floor(vidTime);
		vidTime -= tiempoTranscurrido;
		if (!(this->useMagic(vidCost*tiempoTranscurrido))) {
			this->stopCrystallBall();
		}
	}
}

void Personaje::updateProtectionSpell() {
	float tiempoTranscurrido = 0.0;
	
	if (this->invulnerable) {
		protTime += GameView::instance().getTimer()->getDeltaTime()/1000;
	}
	if (protTime > 1.0) {
		tiempoTranscurrido = std::floor(protTime);
		protTime -= tiempoTranscurrido;
		if (!(this->useMagic(protCost*tiempoTranscurrido))) {
			this->stopProtectionSpell();
		}
	}
}

void Personaje::detenerMagia() {
		modelo->hacerMagia();
		this->stopProtectionSpell();
}

void Personaje::setProtCost(float cost) {
	this->protCost = cost;
}

void Personaje::invocarMagia() {
	bool canActivate;
	
	if (this->hechizoActual != NULL) {
		canActivate = this->hechizoActual->startSpell(this->getPlayerName());
		if (canActivate) {
			GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_MAGIC)+";"+stringUtilities::pairIntToString(this->getPosicionActualEnTiles()));
			modelo->hacerMagia();
			delete (this->hechizoActual);
			this->hechizoActual = NULL;
		}
	}
}

void Personaje::update() {
	this->setFogged(!modelo->isActive());
	this->mover();
	if (this->isCenteredInTile()) {
		this->animar();
	}
	modelo->update();
	this->updateProtectionSpell();
	this->updateCrystallBall();
	this->getWeapons()[WEAPON_BOW]->setRange((this->personajeModelo()->getVision()->getRangeVision())/2 + 1);
	this->getWeapons()[WEAPON_HAND_GRENADE]->setRange((this->personajeModelo()->getVision()->getRangeVision())/2 + 1);
	if (this->isImmobilized())
		return;
	if (!this->hasValidSprite()) {
		GameView::instance().getErrorImage()->updateFrame();
	} else {
		sprites[this->currentSpritePosition]->updateFrame();
	}
}

bool Personaje::repositionToStrike() {
	std::pair<int, int> reposition = this->getWeapons()[this->getSelectedWeapon()]->calculateRepositionToStrike(this->currentEnemy->getPosition());
	if (reposition == this->getPosition())
		return false;
	this->modelo->setDestino(reposition.first, reposition.second);
	return true;
}

void Personaje::mover() {
	if (this->isCenteredInTile() && (this->isImmobilized() || this->modelo->estaAnimandose()))
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
		this->setPosition(modelo->getPosition());
		//tileActual = modelo->getPosition();
		//modelo->setIsInCenterTile(true);
		this->eatIfItem(this->getPosition());
		if (this->currentEnemy != NULL) {
			//prepare things to attack
			//common::Logger::instance().log(":calcularSigTileAMover()  enemy NOT NULL");
			//common::Logger::instance().log("Enemy:  " + ((Entity*)this->currentEnemy)->getName());
			this->getWeapons()[this->getSelectedWeapon()]->setPosition(this->getPosition());
			this->getWeapons()[this->getSelectedWeapon()]->setDirection(this->modelo->getDirection());
			if (this->getWeapons()[this->getSelectedWeapon()]->readyToStrike(this->currentEnemy->getPosition())) {
				if (this->getWeapons()[this->getSelectedWeapon()]->needsToReposition(this->currentEnemy->getPosition())) {
					this->repositionToStrike();
					currentAnimationNumber = modelo->mover(tile, velocidad);
				}
				else {
					this->modelo->setNoTarget();
					this->velocidad = 0;
				}
			} else {
				currentAnimationNumber = modelo->mover(tile, velocidad);
			}
		} else {
			//common::Logger::instance().log("calcularSigTileAMover()  ENEMY IS NULL ");
			currentAnimationNumber = modelo->mover(tile, velocidad);
		}
		//currentAnimationNumber = modelo->mover(tile, velocidad);
		if (this->modelo->estaAnimandose())
			return;
		this->setCurrentSpritePosition(this->calculateSpritePosition(currentAnimationNumber));
		//if (currentSpritePosition < 0)
		//	this->currentSpritePosition = 0;
		if (previousSpritePosition != this->currentSpritePosition) {
			ePot.first = 0;
			ePot.second = 0;
		} 
		//std::string aux = stringUtilities::floatToString(velocidad);
		//common::Logger::instance().log("Velocidad: "+ aux);
		if (velocidad != 0) {
			//modelo->setIsInCenterTile(false);
			modelo->setPosition(tile);
		} else {

			this->atacar();
		}
		//common::Logger::instance().log("if (modelo->getIsReseting()) "+ stringUtilities::intToString(this->currentSpritePosition));
		if (modelo->getIsReseting()) {
			this->reset();
		}
	}
}

void Personaje::reset() {
	this->setRectangle(this->getPosition(), sprites[this->currentSpritePosition]);
	currentEnemy = NULL;
	this->heal();
	this->rechargeMagic();
	if (this->hechizoActual != NULL) {
		delete hechizoActual;
		hechizoActual = NULL;
	}
	invulnerable = false;
	protCost = 0;
	protTime = 0;
	vidCost = 0;
	vidTime = 0;
	videncia = false;
	modelo->setIsReseting();
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

void Personaje::manejarDano(float danoRecibido)
{
	if(!this->hasShield())
		vidaActual -= danoRecibido;
	else
	{
		float diference=danoRecibido-this->shieldAbsortion;
		if(diference>=0)
		{
			this->shieldResistance-=this->shieldAbsortion;
			vidaActual -=diference;
		}
		else
		{
		this->shieldResistance-=this->shieldAbsortion-diference;
		}
	}
}

void Personaje::recibirDano(float dano) {
	
	if ((this->isFogged()) || (this->invulnerable)) {
		return;
	}
	float danoRecibido = Game::instance().getRandom() * dano;
	this->manejarDano(danoRecibido);
	//vidaActual -= danoRecibido;
	if (vidaActual > 0) {
		this->modelo->herir();
	} else {
		this->modelo->morir();
		GameView::instance().getWorldView()->relocateItem(this->getPosition());
	}
}

void Personaje::atacar() {
if (currentEnemy != NULL) {
		//common::Logger::instance().log("Enemy: going to attack  " );
		this->getWeapons()[this->selectedWeapon]->setPosition(this->getPosition());
		this->getWeapons()[this->selectedWeapon]->setDirection(this->modelo->getDirection());
		if (!this->getWeapons()[this->selectedWeapon]->sameDirection(currentEnemy->getPosition()))
			return;
		if (!this->getWeapons()[this->selectedWeapon]->isInsideRange(currentEnemy->getPosition()))
			return;
		//if(currentEnemy->isWood())
		//	GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_WOOD)+";"+this->positionToString());//AGREGO SONIDO
		//else
		//	GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_SHIELD)+";"+this->positionToString());//AGREGO SONIDO
		
		
		switch (this->selectedWeapon) {
			case WEAPON_SWORD: {
			//ataque con espada
				this->getWeapons()[this->selectedWeapon]->strike(currentEnemy);
				this->modelo->atacar();
					//if (!(this->currentEnemy->isAlive()))
					//	GameView::instance().getMission()->missionUpdate(currentEnemy, this->getPlayerName());
					//currentEnemy = NULL;
				break;
			}
			case WEAPON_BOW: {
				//ataque con arco y flecha
				this->getWeapons()[this->selectedWeapon]->strike(currentEnemy);
				this->modelo->defender();
				break;
			}
			case WEAPON_HAND_GRENADE: {
				//ataque con granada
				this->getWeapons()[this->selectedWeapon]->strike(currentEnemy);
				this->modelo->defender();
				break;
			}
			case WEAPON_BOMB_DROPPER: {
				//ataque con bomba
				this->getWeapons()[this->selectedWeapon]->strike(currentEnemy);
				this->modelo->defender();
				break;
			}
			case WEAPON_ICE_INCANTATOR: {
				//ataque con varita magica
				this->getWeapons()[this->selectedWeapon]->strike(currentEnemy);
				this->modelo->defender();
				break;
			}
		}
		currentEnemy = NULL;
	}
}

void Personaje::setInvulnerable(bool inv) {
	this->invulnerable = inv;
}

void Personaje::setCurrentEnemy(int tileX, int tileY) {
	std::pair<int, int> tileDestino(tileX, tileY);
	if (modelo->isThereAnEnemy(tileX, tileY)) {
		this->modelo->orientar(tileDestino);
		currentEnemy = GameView::instance().getDaniableInTile(tileDestino);
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
	if ((currentEnemy->getPosition() != modelo->getTarget()) && (modelo->canSee(currentEnemy->getPosition()))) {
		setDestino(currentEnemy->getPosition().first, currentEnemy->getPosition().second);
		this->modelo->setFollowingEnemy(true);
		return;
	}
	if (!modelo->canSee(currentEnemy->getPosition())) {
		currentEnemy = NULL;
	}
	this->modelo->setFollowingEnemy(false);
}

void Personaje::setDestino(int xTile, int yTile){
	modelo->setDestino(xTile, yTile);
	setCurrentEnemy(xTile, yTile);
}

void Personaje::changeWeapon() {
	//this->setSelectedWeapon(this->getSelectedWeapon()+1);
	if ((this->getWeapons().size() - 1) == (this->getSelectedWeapon())) {
		this->setSelectedWeapon(0);
	} else {
		this->setSelectedWeapon(this->getSelectedWeapon()+1);
	}
}

bool Personaje::isValidWeapon() {
	switch (this->getSelectedWeapon()) {
	case WEAPON_ICE_BOMB_SPELL:
		return this->hasIceSpell();
		break;
	}
	return true;
}

void Personaje::processKeyCommand(char animacion) {
	switch (animacion) {
		case (OPCION_MAGIA): {
			this->invocarMagia();
			break;
				  }
		case (OPCION_TERMINAR_MAGIA): {
			this->stopMagic();
			break;
				  }
		case (OPCION_CAMBIAR_ARMA): {
			this->changeWeapon();
			while (!this->isValidWeapon())
				this->changeWeapon();
			break;
				  }
		default:;
		}
}

void Personaje::setHechizo(Hechizo* hechizo) {
	if (this->hechizoActual != NULL) {
		delete (this->hechizoActual);
		this->hechizoActual = NULL;
	}
	this->hechizoActual = hechizo;
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
	if (hechizoActual != NULL) {
		delete hechizoActual;
	}

	//Destroying weapons
	for (unsigned int i = 0; i < this->getWeapons().size(); i++) {
		delete this->getWeapons()[i];
	}
}

PersonajeModelo* Personaje::personajeModelo(){
	return modelo;
}

std::pair<int,int> Personaje::getPosicionEnTiles(){
	return modelo->getPosition();
}

std::pair<int,int> Personaje::getPosicionActualEnTiles(){
	float deltaAbsX = std::abs(delta.first);
	float deltaAbsY = std::abs(delta.second);

	if ((deltaAbsX <= 32) && (deltaAbsY == 0)) {
		return modelo->getPosition();
	}
	if ((deltaAbsX == 0) && (deltaAbsY <= 16)) {
		return modelo->getPosition();
	}
	if ((deltaAbsX <= 16) && (deltaAbsY <= 8)) {
		return modelo->getPosition();
	}
	return this->getPosition();
}

//tilex, tiley; pixelx, pixely; isFreezed; nro_status; nro_surface
std::string Personaje::updateToString() {

	std::string out = stringUtilities::pairIntToString(modelo->getPosition());
	out.append(";");
	out.append(stringUtilities::pairIntToString(this->getPixelPosition()));
	out.append(";");
	if (this->isFogged()) {
		out.append("F");
	} else {
		out.append("N");
	}
	out.append(";");
	out.append(stringUtilities::intToString(this->getCurrentSpritePosition()));
	out.append(";");
	if (this->hasValidSprite()) {
		out.append(stringUtilities::intToString(sprites[this->getCurrentSpritePosition()]->getCurrentSurfaceNumber()));
	} else {
		// en caso de problemas con el sprite mando un 0
		out.append(stringUtilities::intToString(0));
	}
	out.append(";");
	if (this->isCenteredInTile()) {
		out.append("T");
	} else {
		out.append("F");
	}
	out.append(";");
	out.append(stringUtilities::floatToString(this->vidaActual));
	out.append(";");
	out.append(stringUtilities::floatToString(this->magiaActual));
	out.append(";");
	out.append(stringUtilities::floatToString(this->modelo->getVidaMaxima()));
	out.append(";");
	out.append(stringUtilities::floatToString(this->modelo->getMagiaMaxima()));
	out.append(";");
	out.append(stringUtilities::floatToString(this->shieldResistance));
	out.append(";");
	if (this->invulnerable) {
	out.append("T");
	} else {
		out.append("F");
	}
	out.append(";");
	if (hechizoActual != NULL) {
		out.append(hechizoActual->getSpellId());
	} else {
		out.append("");
	}
	out.append(";");
	if(this->videncia){
		out.append("T");
	} else {
		out.append("F");
	}
	out.append(";");
	out.append(stringUtilities::intToString(this->getSelectedWeapon())+","+stringUtilities::unsignedToString(this->weapons[this->getSelectedWeapon()]->getAmmo()));
	out.append(";");
	out.append(this->modelo->getVision()->updateToString());
	return out;
}

//tilex, tiley; pixelx, pixely; isFreezed; nro_status; nro_surface
void Personaje::updateFromString(std::string data) {
	vector <std::string> splittedData;
	stringUtilities::splitString(data, splittedData, ';');
	std::pair<int,int> tilePosition = stringUtilities::stringToPairInt(splittedData[0]);
	this->setPosition(this->modelo->getPosition());
	this->modelo->setPosition(tilePosition);
	this->modelo->getVision()->updatePosition(modelo->getPosition());
	std::pair<int,int> pixels = stringUtilities::stringToPairInt(splittedData[1]);
	this->setPixelPosition(pixels);
	this->setFogged(splittedData[2] == "F");
	this->setCurrentSpritePosition(stringUtilities::stringToInt(splittedData[3]));
	if (this->hasValidSprite()) {
		this->sprites[this->getCurrentSpritePosition()]->setCurrentSurfaceNumber(stringUtilities::stringToInt(splittedData[4]));
	}
	//this->setCenteredInTile(splittedData[5] == "T");
	this->modelo->getVision()->updateFromString(splittedData[6]);
	this->update();
	//this->setActive(true);
}

int Personaje::getCurrentSpritePosition() {
	return this->currentSpritePosition;
}

void Personaje::setCurrentSpritePosition(int pos) {
	if (pos < 0) {
		this->currentSpritePosition = 0;
		return;
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
	out.append(this->modelo->getVision()->initToString());
	return out;
}

//tilex,tiley;pixelx,pixely;isFreezed;nro_status;nro_surface~datos_vision
void Personaje::initFromString(std::string data) {
	vector <std::string> splittedData;
	stringUtilities::splitString(data, splittedData, '~');
	this->updateFromString(splittedData[0]);
	this->modelo->getVision()->initFromString(splittedData[1]);
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
	return out;
}

void Personaje::increaseSpeed(float factor)
{
	this->modelo->increaseSpeed(factor);
}

void Personaje::heal() {

	vidaActual = modelo->getVidaMaxima();
}

bool Personaje::useMagic(float usedMagic) {
	if(magiaActual >= usedMagic) {
		magiaActual -= usedMagic;
		return true;
	}
	return false;
}

void Personaje::rechargeMagic() {

	magiaActual = modelo->getMagiaMaxima();
}

bool Personaje::isItem()
{
	return false;
}

void Personaje::eatIfItem(std::pair<int, int> destino) {
	Entity * entity= GameView::instance().getWorldView()->getTileAt(destino)->getOtherEntity();
	if (entity != NULL) {
		if(entity->isItem()) {
			ItemView* item=(ItemView*)entity;
			if(item->isAlive()) {
				item->modifyCharacter(this);
				item->kill();
				GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_EAT_ITEM)+";"+stringUtilities::pairIntToString(this->getPosicionActualEnTiles()));
			}
		}
	}
}

bool Personaje::hasValidSprite() {
	return ((this->getCurrentSpritePosition() <= static_cast<int>(sprites.size()-1))&&(this->getCurrentSpritePosition() >= 0));
}

bool Personaje::isThisSpriteValid(int currentAnimationNumber) {
	return ((this->calculateSpritePosition(currentAnimationNumber) <= static_cast<int>(sprites.size()-1))&&(this->calculateSpritePosition(currentAnimationNumber) >= 0));
}

void Personaje::setShield(float resistance,float absortion) {
	this->shieldResistance=resistance;
	this->shieldAbsortion=absortion;
}

bool Personaje::hasShield() {
	return (this->shieldResistance > 0);
}

std::vector<Weapon*>& Personaje::getWeapons() {
	return this->weapons;
}

void Personaje::loadWeapons() {
	//Initializing weapons
	Sword* sword = new Sword();
	sword->setOwner(this->getPlayerName());
	
	sword->initialize(true,1,this->modelo->getDanoMaximo(),this->modelo->getPrecisionMinima());
	this->getWeapons().push_back(sword);

	Bow* bow = new Bow();
	bow->setOwner(this->getPlayerName());
	bow->initialize(true,2,this->modelo->getDanoMaximo(),this->modelo->getPrecisionMinima());
	this->getWeapons().push_back(bow);

	HandGrenade* handGrenade = new HandGrenade();
	handGrenade->setOwner(this->getPlayerName());
	handGrenade->initialize(true,2,this->modelo->getDanoMaximo(),this->modelo->getPrecisionMinima());
	this->getWeapons().push_back(handGrenade);

	BombDropper* bombDropper = new BombDropper();
	bombDropper->setOwner(this->getPlayerName());
	bombDropper->initialize(true,2,this->modelo->getDanoMaximo(),this->modelo->getPrecisionMinima());
	this->getWeapons().push_back(bombDropper);
	WeaponIceIncantator* weaponIceIncantator = new WeaponIceIncantator();
	weaponIceIncantator->setOwner(this->getPlayerName());
	weaponIceIncantator->initialize(true,2,this->modelo->getDanoMaximo(),this->modelo->getPrecisionMinima());
	this->getWeapons().push_back(weaponIceIncantator);

	this->setSelectedWeapon(WEAPON_SWORD); //selectedWeapon es la posicion en el vector de weapons, ver PersonajeConstantes.h
	//this->setSelectedWeapon(WEAPON_BOW);
	//this->setSelectedWeapon(WEAPON_ICE_INCANTATOR);
	//this->setSelectedWeapon(WEAPON_HAND_GRENADE);
}

void Personaje::setSelectedWeapon(unsigned value) {
	this->selectedWeapon = value;
}

unsigned Personaje::getSelectedWeapon() {
	return this->selectedWeapon;
}

bool Personaje::isWood() {
	return false;
}

bool Personaje::hasIceSpell() {
	return this->iceSpell;
}

void Personaje::setIceSpell(bool value) {
	this->iceSpell = value;
}