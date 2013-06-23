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



Stage::Stage() {
}

Stage::~Stage() {
	for (unsigned int i = 0; i < spriteArray.size(); i++) {
		delete spriteArray[i];
	}
	spriteArray.clear();
	
	this->ammunitions.clear();

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

bool Stage::initialize() {
	worldModel = Game::instance().world();
	this->loadSprites();
	this->generateStage();
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
		data = "";
		if ((*it)->isAlive()) {
			(*it)->update();
			if (((*it)->getName() == "Arrow")||((*it)->getName() == "IceIncantation")) {
				data = ((ImpactAmmo*)(*it))->serialize();
			}
			if ((*it)->getName() == "Bomb") {
				if(((Bomb*)(*it))->needsUpdates()) {
					data = ((Bomb*)(*it))->serialize();
				}
			}
			//if ((*it)->getName() == "Grenade") {
			//	data = ((Grenade*)(*it))->serialize();
			//}
			//if ((*it)->getName() == "IceBomb") {
			//	data = ((IceBomb*)(*it))->serialize();
			//}
			//common::Logger::instance().log("alive "+ data);
			if (data != "")
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