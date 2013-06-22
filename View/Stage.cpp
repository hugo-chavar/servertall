#include "Stage.h"
//#include "Surface.h"
#include "EntityObject.h"
#include "DataTypes.h"
#include "TileModel.h"
#include "ItemFactoryView.h"
#include <algorithm>
#include "GameView.h"
#include "Ammunition.h"
#include "Arrow.h"
#include "Bomb.h"
#include "Grenade.h"
//#include "IceBomb.h"
//#include "../DedicatedServer/Serializable.h"

//#define START_LEVEL 0
//#define EXTRA_TILES_TO_RENDER 9

Stage::Stage() {
	//_personaje = NULL;
}

Stage::~Stage() {
	for (unsigned int i = 0; i < spriteArray.size(); i++) {
		delete spriteArray[i];
	}
	spriteArray.clear();
	
	this->ammunitions.clear();

	//if (_personaje){
	//	delete _personaje;
	//	_personaje = NULL;
	//}
	deleteStage();
}

void Stage::loadSprites() {
	//carga de sprites estaticos
	Sprite* auxSprite;
	unsigned staticEntitiesModelCount = Game::instance().allEntities.vEntitiesObject.size();
	for (unsigned a = 0; a < staticEntitiesModelCount; a++){
		EntityObject *entity = Game::instance().allEntities.vEntitiesObject[a];
		mapEntityToSprite[entity->name()] = int(a);
		auxSprite = new Sprite(entity);
		spriteArray.push_back(auxSprite);
		auxSprite = NULL;
	}
	//carga de sprites animados
	SpriteAnimado* auxAnimatedSprite;
	unsigned animatedEntitiesModelCount = Game::instance().allEntities.vAnimatedEntities.size();
	for (unsigned a = 0; a < animatedEntitiesModelCount; a++){
		AnimatedEntity *entity = Game::instance().allEntities.vAnimatedEntities[a];
		mapEntityToSprite[entity->name()] = int(a + staticEntitiesModelCount);
		auxAnimatedSprite = new SpriteAnimado(entity);
		spriteArray.push_back(auxAnimatedSprite);
		auxAnimatedSprite = NULL;
	}
}

TileView* Stage::createTile(TileModel* tileModel) {
	TileView* tile = new TileView(tileModel);
	int posSpriteEntity = mapEntityToSprite.at(tile->getGroundEntityName());
	tile->createGround(spriteArray[posSpriteEntity]);
	if (tile->hasOtherEntity()) {
		posSpriteEntity = mapEntityToSprite.at(tile->getOtherEntityName());
		tile->createOtherEntity(spriteArray[posSpriteEntity]);
	}
	else{
		ItemFactoryView factory;
		ItemView* item=factory.generateRandomItem(Game::instance().world()->itemsPercentage(),HIDDEN_ITEM,tile->getPosition(),true);//Harcodeo porcentaje de items
		if(item)
			{
				itemsArray.push_back(item);
				tile->setOtherEntity(item);
			}	
	}
	tilesMap.insert(make_pair(tile->getPosition(), tile));
	return tile;
}

void Stage::generateStage() {
	TileModel* tileModel = worldModel->getFirstTile();
	this->firstTile = this->createTile(tileModel);
	tileLevels.push_back(this->firstTile);
	TileView* currentTile;
	TileView* prevTile = this->firstTile;
	KeyPair tilePos;
	tileModel = tileModel->getNextTile();
	while (tileModel) {
		currentTile = this->createTile(tileModel);
		if (prevTile->EOL())
			tileLevels.push_back(currentTile);
		prevTile->setNextTile(currentTile);
		prevTile = currentTile;
		tileModel = tileModel->getNextTile();
	}
	currentTile = this->firstTile;
	tileModel = worldModel->getFirstTile();
	while ((currentTile) && (tileModel)) {
		if (tileModel->getRelatedTile()) {
			tilePos = tileModel->getRelatedTile()->getPosition();
			prevTile = tilesMap.at(tilePos);
			currentTile->setRelatedTile(prevTile);
		}
		currentTile = currentTile->getNextTile();
		tileModel = tileModel->getNextTile();
	}
	//Generar Items Random Para los personajes al morir
	for(unsigned i=0;i<NUMBERITEMS;i++)
	{
		ItemFactoryView factory;
		ItemView* item=factory.generateItem(i,DEATH_ITEM,pair<int,int>(-1,-1),false);//Harcodeo porcentaje de items
		itemsArray.push_back(item);
	}
}
//
//void Stage::setTilesInCamera(int w, int h) {
//	unsigned horizontalTilesInCamera = static_cast<unsigned>(ceil(static_cast<float>(w) / DEFAULT_TILE_WIDTH));
//	unsigned verticalTilesInCamera = static_cast<unsigned>(ceil(static_cast<float>(h) / DEFAULT_TILE_HEIGHT));
//	minLevelsInCamera = horizontalTilesInCamera + verticalTilesInCamera;
//}

bool Stage::initialize() {
	worldModel = Game::instance().world();
	
	this->loadSprites();
	this->generateStage();

	//if (!Game::instance().personaje()){
	//	return false;
	//}
	//_personaje = new Personaje(Game::instance().personaje());
	//_personaje->loadSprites();

	return true;
}

Sprite* Stage::getSpriteWithName(string value) {
    bool found = false;
    map<string, int>::iterator it = mapEntityToSprite.begin();
    while(it != mapEntityToSprite.end())
    {
        found = (it->first == value);
        if(found)
            break;
        ++it;
    }
	if (found) {
		int posSprite = mapEntityToSprite.at(value);
		Sprite* auxSprite = spriteArray[posSprite];
		return auxSprite;
	} 
	return NULL;
}

void Stage::update() {
//	this->updateSprites();
//	this->updateTiles();
	this->updateItems();
	this->updateAmmunitions();
//	_personaje->update();
}

//Personaje* Stage::personaje() {
//	return _personaje;
//}

TileView* Stage::getTileAt(KeyPair k) {
	return tilesMap.at(k);
}

bool Stage::addOtherEntity(pair <int,int> position, string entityName) {
	if (mapEntityToSprite.find(entityName) == mapEntityToSprite.end())
		return false;
	Sprite* entitySprite = spriteArray[mapEntityToSprite.at(entityName)];
	Entity* entity = new Entity(position.first, position.second, entitySprite);
	this->getTileAt(position)->setOtherEntity(entity);
	return true;
}

void Stage::removeOtherEntity(pair <int,int> tile) {
	delete(this->getTileAt(tile)->getOtherEntity());
	this->getTileAt(tile)->setOtherEntity(NULL);
}

//TileView* Stage::getFirstMatch(std::pair<int,int> k) {
//	TileView* aux = tileLevels.at(this->fixLevel(k));
//	KeyPair position = aux->getPosition();
//	while ((static_cast<int>(position.second) > k.second) && (aux)) {
//		aux = aux->getNextTile();
//		if (aux)
//			position = aux->getPosition();
//	}
//	return aux;
//}
//
//TileView* Stage::getLastMatch(TileView* firstMatch, std::pair<int,int> k) {
//	TileView* aux = firstMatch;
//	KeyPair position = aux->getPosition();
//	while ((static_cast<int>(position.first) <= k.first) && (!aux->EOL())) {
//		aux = aux->getNextTile();
//		position = aux->getPosition();
//	}
//	return aux;
//}
//
//void Stage::fixKeyLeftBottom(int level, std::pair<int,int> &k) {
//	while (level < (k.first + k.second)) {
//			k.first--;
//	}
//}
//
//void Stage::fixKeyRightBottom(int level, std::pair<int,int> &k) {
//	while (level < (k.first + k.second)) {
//			k.second--;
//	}
//}
//
//int Stage::fixLevel(std::pair<int,int> k) {
//	int level = k.first + k.second;
//	if (level > this->worldModel->maxLevels() ) {
//		level = this->worldModel->maxLevels();
//	}
//	else if (level < START_LEVEL )
//		level = START_LEVEL;
//	return level;
//}
//
//int Stage::fixStartLevel(int endLevel, std::pair<int,int> &ref) {
//	int level = this->fixLevel(ref);
//	if (((endLevel - level) < minLevelsInCamera) && (level > START_LEVEL)) {
//		if ((endLevel - minLevelsInCamera) > START_LEVEL) {
//			ref.first -= minLevelsInCamera - (endLevel - level);
//			level = this->fixLevel(ref);
//		}else {
//			level = START_LEVEL;
//		}
//	}
//	return level;
//}
//
//void Stage::alignLevel(std::pair<int,int> &k1, std::pair<int,int> &k2) {
//	int level1 = this->fixLevel(k1);
//	int level2 = this->fixLevel(k2);
//	while (level1 > level2 )  {
//		k2.first++;
//		level2 = this->fixLevel(k2);
//	}
//	while (level1 < level2 ) {
//		k2.second--;
//		level2 = this->fixLevel(k2);
//	}
//}

// void Stage::calculateTilesToRender(Camera& camera) { 
//	renderHelper.clear();
//	std::pair<int,int> position = std::make_pair(static_cast<int>(camera.getOffsetX()),static_cast<int>(camera.getOffsetY()));
//	std::pair<int,int> cameraReferenceTile = this->worldModel->pixelToTileCoordinates(position);
//	cameraReferenceTile.second -= EXTRA_TILES_TO_RENDER;
//	cameraReferenceTile.first -= EXTRA_TILES_TO_RENDER;
//	std::pair<int,int> leftBottom = this->worldModel->pixelToTileCoordinatesInStage(make_pair(0,camera.getHeight()), camera.getOffsetX(), camera.getOffsetY());
//	std::pair<int,int> rightBottom = this->worldModel->pixelToTileCoordinatesInStage(make_pair(camera.getWidth(),camera.getHeight()), camera.getOffsetX(), camera.getOffsetY());
//	leftBottom.second += EXTRA_TILES_TO_RENDER;
//	rightBottom.first += EXTRA_TILES_TO_RENDER;
//	int endLevel = this->fixLevel(leftBottom);
//	this->fixKeyLeftBottom(endLevel, leftBottom);
//	this->alignLevel(leftBottom, rightBottom );
//	int startLevel = this->fixStartLevel(endLevel, cameraReferenceTile);
//	renderHelper.setStartLevel(startLevel);
//	renderHelper.setEndLevel(endLevel);
//	while (renderHelper.incomplete()) {
//		TileView* firstMatch = this->getFirstMatch(leftBottom);
//		TileView* lastMatch;
//		if (firstMatch) {
//			lastMatch = this->getLastMatch(firstMatch,rightBottom);
//			renderHelper.addLevel(firstMatch,lastMatch);
//		} else {
//			renderHelper.setEmptyLevel();
//		}
//
//		if (renderHelper.flip()) {
//			leftBottom.first--;
//			rightBottom.first--;
//		} else {
//			rightBottom.second--;
//			leftBottom.second--;
//		}
//	}
//
//}

//void Stage::render(Camera& camera) {
//	this->calculateTilesToRender(camera);
//	renderHelper.renderGround(camera);
//	renderHelper.startRenderingEntities();
//	int a = 0;
//	while (renderHelper.hasLevelsToRender()) {
//		a++;
//		if (a == 53)
//			a--;
//		renderHelper.renderNextLevel(camera);
//		if (renderHelper.shouldRenderThis(_personaje->getPosicionEnTiles(),_personaje->getPosicionActualEnTiles()))
//			_personaje->render(camera);
//	}
//}

void Stage::deleteStage() {
	TileView* aux = this->firstTile;
	TileView* nextAux;
	while (aux) {
		nextAux = aux;
		aux = nextAux->getNextTile();
		delete nextAux;
	}
	tilesMap.clear();

	itemsArray.clear();
}


void Stage::updateTiles() {
	TileView* aux = this->firstTile;
	while (aux) {
		aux->update();
		aux = aux->getNextTile();
	}
}

void Stage::updateItems()
{
	for(unsigned i=0;i<this->itemsArray.size();i++)
	{
		itemsArray[i]->update();
	}
}

void Stage::updateSprites() {
	for (unsigned i = 0 ; i < spriteArray.size(); i++) {
		spriteArray[i]->updateFrame();
	}
}

Sprite* Stage::getSprite(string name)
{
	return spriteArray[mapEntityToSprite.at(name)];
}

string Stage::manageItemsInitialSynch()
{
	string itemsInfo="";
	for(unsigned i=0;i<this->itemsArray.size();i++)
	{
		itemsInfo+=itemsArray[i]->getName()+";";
		//if(!itemsArray[i]->isAlive())
		//{
		//	itemsInfo+="D;";
		//}
		//else if(itemsArray[i]->isHidden())
		//	itemsInfo+="H;";
		//else
		//	itemsInfo+="A;";
		itemsInfo+=itemsArray[i]->statusToString()+";";
		itemsInfo+=itemsArray[i]->positionToString()+";";
	}
	if(itemsInfo.size()>0)
		itemsInfo.erase(itemsInfo.size()-1);
	return itemsInfo; 
}

StringQueue * Stage::getItemChanges()
{
return &this->itemChanges;
}

void Stage::addItemChange(string _string)
{
	this->itemChanges.push(_string);
}

ItemView* Stage::getItemInTile(int posX,int posY)
{
	for(unsigned i=0;i<this->itemsArray.size();i++)
	{
		if(itemsArray[i]->getPosition() == std::make_pair(posX, posY))
			if(itemsArray[i]->isHidden()&&itemsArray[i]->isAlive())
				return this->itemsArray[i];
	}
	return NULL;
}

string Stage::manageItemsUpdate()
{
	return GameView::instance().manageUpdate(&this->itemChanges);
	/*this->itemChanges.lock();
	string changes="";
	while(itemChanges.size()>0)
	{
		changes+=itemChanges.pop()+":";
	}
	if (changes.size()!=0)
	{
		changes.pop_back();
	}
	this->itemChanges.unLock();
	return changes;*/
}


//void Stage::regenerateItem()
//{
//	unsigned actual=SDL_GetTicks();
//	if(actual-this->lastRegeneration>REGENERATIONTIME)
//	{
//			this->reviveitem('H');
//			this->lastRegeneration=actual;
//	}
//}
//
//void Stage::reviveitem(char hidden)
//{
//	bool revive=false;
//	unsigned i=0;
//	while(!revive && i<this->itemsArray.size())
//	{
//		if(!itemsArray[i]->isAlive())
//		{
//			pair<int,int> pos=itemsArray[i]->getPosicionActualEnTiles();
//			if(pos.first==-1 && pos.second==-1)
//				this->relocateItem(itemsArray[i]);
//			itemsArray[i]->revive(hidden); //todo:comunicar a la vista esto
//			revive=true;
//		}
//
//	}
//}
//
//void Stage::relocateItem(ItemView* item)
//{
//	int random= rand() % this->tilesMap.size();
//	//this->worldModel->
//}

//bool Stage::isThereAPlayerInTile(pair <int,int> pos)
//{
//	std::map<std::string,Personaje*>::iterator it;
//	for(it=this->personajes.begin();it!=personajes.end();it++)
//		{
//			if(it->second->getPosicionEnTiles()==pos)
//				return true;
//		}
//	return false;
//}

ItemView* Stage::findDeathItem()
{
	for(unsigned i=this->itemsArray.size();i>0;i--)
	{
		if(!itemsArray[i-1]->isAlive() && !(itemsArray[i-1]->getCanReviveForHimself()))
			{
				int random=rand()%itemsArray.size();
				std::swap(itemsArray[i-1],itemsArray[random]);
				return itemsArray[random];
			}
	}
	return NULL;
}

void Stage::relocateItem(pair<int,int>pos)
{
	ItemView* item=this->findDeathItem();
	if(item)
	{
		item->revive(REVIVE_UNCOVER_ITEM,pos);
		this->getTileAt(pos)->setOtherEntity(item);
	}
}

void Stage::updateAmmunitions() {
	vector<Entity*>::iterator it;
	it = ammunitions.begin();
	std::string data;
	std::string evento = stringUtilities::intToString(EVENT_AMMUNITION_CHANGE) + ";";
	while ( it != ammunitions.end()) {
		//if(((Ammunition*)(*it))->needsUpdates()) {

		if ((*it)->isAlive()) {
			(*it)->update();
			if (((*it)->getName() == "Arrow")||((*it)->getName() == "IceIncantation")) {
				data = ((ImpactAmmo*)(*it))->serialize();
			}
			if ((*it)->getName() == "Bomb") {
				data = ((Bomb*)(*it))->serialize();
			}
			//if ((*it)->getName() == "Grenade") {
			//	data = ((Grenade*)(*it))->serialize();
			//}
			//if ((*it)->getName() == "IceBomb") {
			//	data = ((IceBomb*)(*it))->serialize();
			//}
			common::Logger::instance().log("alive "+ data);
			GameView::instance().addEventUpdate(evento + data);
			it++;
		} else {
			ammunitions.erase(it); 
		}
	}
}

void Stage::addAmmunition(Entity * ammo) {
	this->ammunitions.push_back(ammo);
}