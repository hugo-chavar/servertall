#include "CharacterVision.h"
#include "../View/GameView.h"
#include "TileModel.h"
#include "StringUtilities.h"
#include "Logger.h"

CharacterVision::CharacterVision() {
	mapKnowledge.clear();
}

CharacterVision::~CharacterVision() {

}

void CharacterVision::setRangeVision(int value) {
	this->rangeVision = value;
}

void CharacterVision::initialize() {
	this->mapWidth = Game::instance().world()->width();
	this->mapHeight = Game::instance().world()->height();
	for (int i = 0; i < this->mapHeight; i++) {
		bitset <MAX_STAGE_SIZE_X> line;
		line.reset();
		mapKnowledge.push_back(line);
	}

	vision.initialize(this->position, this->rangeVision);
	vision.fill();
	pair<int, int > aux;
	while (vision.hasNext()) {
		aux = vision.next();
		this->setKnown(aux);
	}
}

void CharacterVision::setPosition(pair<int, int> pos) {
	this->position = pos;
}

void CharacterVision::updatePosition(pair<int, int> pos) {
	if (pos == this->position)
		return;
	vision.clear();
	vision.initialize(pos, this->rangeVision);
	vision.fill();
	pair<int, int > aux;
	while (vision.hasNext()) {
		aux = vision.next();
		this->setKnown(aux);
	}
	this->position = pos;
}

bool CharacterVision::testPosition(pair<int, int> pos) {
	return this->mapKnowledge[pos.second].test(pos.first);
}

void CharacterVision::setKnown(pair<int, int> pos){
	if (Game::instance().world()->isInsideWorld(pos)) {
		if (testPosition(pos))
			return;
		this->mapKnowledge[pos.second].set(pos.first);
		TileModel* relatedTile = Game::instance().world()->getTileAt(pos)->getRelatedTile();
		if (relatedTile){
			while (relatedTile != Game::instance().world()->getTileAt(pos) ) {
				pair<int, int> posRelated = relatedTile->getPosition();
				this->mapKnowledge[posRelated.second].set(posRelated.first);
				relatedTile = relatedTile->getRelatedTile();
			}
		}
	}
}

bool CharacterVision::isInsideVision(pair<int, int> pos) {
	return vision.inside(pos);
}

string CharacterVision::toString() {
	pair<int, int> pos;
	string out = "";
	for (pos.first = 0; pos.first < this->mapHeight; pos.first++) {
		for (pos.second = 0; pos.second < this->mapWidth; pos.second++) {
			if (this->testPosition(pos)){
				out.append(stringUtilities::pairIntToString(pos));
				out.append(";");
			}
		}
	}
	//common::Logger::instance().log(out);
	return out;
}

void CharacterVision::fromString(string data) {
	vector <string> auxVector;
	pair<int, int> pos;
	auxVector.clear();
	stringUtilities::splitString(data, auxVector, ':');
	for (vector <string>::iterator it = auxVector.begin(); it != auxVector.end(); ++it) {
		pos = stringUtilities::stringToPairInt(*it);
		this->setKnown(pos);
	}
}