#ifndef GALAGA_LEVEL_MANAGER
	#define GALAGA_LEVEL_MANAGER

	#define GALAGA_LEVEL_MANAGER_STATE_MAIN 0
	#define GALAGA_LEVEL_MANAGER_STATE_TRANSITION 1

	#include <allegro5/allegro.h>
	#include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>

	#include "boss.h"
	#include "level.h"
	#include "ship.h"

	class LevelManager {
		int _screenWidth;
		int _screenHeight;
		Ship *_ship;

		Level *_level = NULL;

		Boss *_boss = NULL;

		int _currentLevel = 1;

		int _currentState = GALAGA_LEVEL_MANAGER_STATE_MAIN;
		unsigned int _stateTicks = 0;

	public:
		LevelManager();
		~LevelManager();

		void initialize(int screenWidth, int screenHeight, Ship *ship);

		bool update(unsigned int ticks);
		void render();

		int getScore();
		int getShotsHit();

		int getCurrentLevel() { return _currentLevel; }

		bool isTransitioning();
	};
#endif
