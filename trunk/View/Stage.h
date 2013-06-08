#ifndef _STAGE_H_
#define _STAGE_H_

#include <vector>
#include <utility>
#include <list>
//del modelo
#include "DataTypes.h"
#include "StageModel.h"
#include "Game.h"
//de la vista
#include "TileView.h"
//#include "Camera.h"
#include "Entity.h"
#include "Personaje.h"
#include "SpriteAnimado.h"
#include "stringQueue.h";
//#include "RenderHelper.h"


class Stage {
public:
	Stage();
	~Stage();
	void update(); 
	//void render(Camera& camera);
	bool initialize();
	void setTilesInCamera(int h, int v);
	Personaje* personaje();
	map <string,Personaje*> personajes;//agrego diccionario de personajes
	TileView* getTileAt(KeyPair k);
	bool addOtherEntity(std::pair <int,int> position, std::string entityName);
	void removeOtherEntity(pair <int,int> tile);
	Sprite* getSprite(string name);
	string manageItemsInitialSynch();
	StringQueue * getItemChanges();
	void addItemChange(string _string);
	ItemView* getItemInTile(int posX,int posY);
	string manageItemsUpdate();
private:
	StringQueue itemChanges;
	vector<ItemView*> itemsArray;
	vector<Sprite*> spriteArray;
	Personaje* _personaje;
	StageModel* worldModel;
	map<string,int> mapEntityToSprite;
	map<KeyPair, TileView*> tilesMap;
	TileView* firstTile;
	vector <TileView*> tileLevels;
	int minLevelsInCamera;
	//RenderHelper renderHelper;
	//void calculateTilesToRender(Camera& camera);
	void loadSprites();
	void generateStage();
	void deleteStage();
	TileView* createTile(TileModel* tile);
	TileView* getFirstMatch(std::pair<int,int> k);
	TileView* getLastMatch(TileView* firstMatch, std::pair<int,int> k);
	void fixKeyLeftBottom(int level, std::pair<int,int> &k);
	void fixKeyRightBottom(int level, std::pair<int,int> &k);
	int fixLevel(std::pair<int,int> k);
	int fixStartLevel(int endLevel, std::pair<int,int> &ref);
	void alignLevel(std::pair<int,int> &k1, std::pair<int,int> &k2);
	void updateTiles();
	void updateSprites();
};



#endif //_STAGE_H_
