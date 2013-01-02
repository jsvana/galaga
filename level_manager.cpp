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
	switch (_currentState) {
		case GALAGA_LEVEL_MANAGER_STATE_MAIN:
			_level->update(ticks);

			if (_level->isComplete()) {
				++_currentLevel;

				int oldDifficulty = _level->getDifficulty();
				int score = _level->getScore();
				int shotHits = _level->getShotHits();

				delete _level;

				_level = new Level(_screenWidth, _screenHeight, oldDifficulty + 1,
					_ship, score, shotHits);

				_currentState = GALAGA_LEVEL_MANAGER_STATE_TRANSITION;
				_stateTicks = 0;
			}
			break;

		case GALAGA_LEVEL_MANAGER_STATE_TRANSITION:
			if (_stateTicks > 100) {
				_currentState = GALAGA_LEVEL_MANAGER_STATE_MAIN;
				_stateTicks = 0;
			}
			break;
	}

	++_stateTicks;

	return true;
}

void LevelManager::render() {
	switch (_currentState) {
		case GALAGA_LEVEL_MANAGER_STATE_MAIN:
			_level->render();
			break;

		case GALAGA_LEVEL_MANAGER_STATE_TRANSITION:
			ALLEGRO_FONT *font = AssetManager::getFont("big");

			int lineHeight = al_get_font_line_height(font);

	    al_draw_textf(font, al_map_rgb(255, 0, 0), _screenWidth / 2,
	      _screenHeight / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTRE, "LEVEL %d",
	      _currentLevel);
	}
}

int LevelManager::getScore() {
	if (_level == NULL) {
		return 0;
	} else {
		return _level->getScore();
	}
}
