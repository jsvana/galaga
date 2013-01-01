#include "level_manager.h"

LevelManager::LevelManager() {

}

LevelManager::~LevelManager() {
	if (_level != NULL) {
		delete _level;
	}
}

void LevelManager::initialize(int screenWidth, int screenHeight, Ship *ship) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_ship = ship;

	_level = new Level(_screenWidth, _screenHeight, 1, _ship, 0, 0);
}

bool LevelManager::update(unsigned int ticks) {
	_level->update(ticks);

	return true;
}

void LevelManager::render() {
	_level->render();
}

int LevelManager::getScore() {
	if (_level == NULL) {
		return 0;
	} else {
		return _level->getScore();
	}
}
