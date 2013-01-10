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

	Rectangle bounds(0, 0, _screenWidth, _screenHeight);
	_boss = new Boss(_screenWidth / 2, _screenHeight / 4, bounds, AssetManager::getTexture("enemies"), 1);

	//_level = new Level(_screenWidth, _screenHeight, 1, _ship, 0, 0);
}

bool LevelManager::update(unsigned int ticks) {
	// switch (_currentState) {
	// 	case GALAGA_LEVEL_MANAGER_STATE_MAIN:
	// 		_level->update(ticks);

	// 		if (_level->isComplete()) {
	// 			++_currentLevel;

	// 			int oldDifficulty = _level->getDifficulty();
	// 			int score = _level->getScore();
	// 			int shotHits = _level->getShotsHit();

	// 			delete _level;

	// 			_level = new Level(_screenWidth, _screenHeight, oldDifficulty + 1,
	// 				_ship, score, shotHits);

	// 			_currentState = GALAGA_LEVEL_MANAGER_STATE_TRANSITION;
	// 			_stateTicks = 0;
	// 		}
	// 		break;

	// 	case GALAGA_LEVEL_MANAGER_STATE_TRANSITION:
	// 		if (_stateTicks > 100) {
	// 			_currentState = GALAGA_LEVEL_MANAGER_STATE_MAIN;
	// 			_stateTicks = 0;
	// 		}
	// 		break;
	// }

	if (_boss != NULL) {
		_boss->update(ticks);
	}

	++_stateTicks;

	return true;
}

void LevelManager::render() {
	// switch (_currentState) {
	// 	case GALAGA_LEVEL_MANAGER_STATE_MAIN:
	// 		_level->render();
	// 		break;

	// 	case GALAGA_LEVEL_MANAGER_STATE_TRANSITION:
	// 		if (_stateTicks < 90) {
	// 			ALLEGRO_FONT *font = AssetManager::getFont("big");

	// 			int lineHeight = al_get_font_line_height(font);

	// 	    al_draw_textf(font, al_map_rgb(255, 0, 0), _screenWidth / 2,
	// 	      _screenHeight / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTRE, "LEVEL %d",
	// 	      _currentLevel);
	// 	  }
	// 	  break;
	// }
	if (_boss != NULL) {
		_boss->render();
	}
}

int LevelManager::getScore() {
	return _level == NULL ? 0 : _level->getScore();
}

int LevelManager::getShotsHit() {
	return _level == NULL ? 0 : _level->getShotsHit();
}

bool LevelManager::isTransitioning() {
	return _currentState == GALAGA_LEVEL_MANAGER_STATE_TRANSITION
		&& _stateTicks < 90;
}
