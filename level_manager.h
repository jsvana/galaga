#ifndef GALAGA_LEVEL_MANAGER
	#define GALAGA_LEVEL_MANAGER

	#include "level.h"
	#include "ship.h"

	class LevelManager {
		int _screenWidth;
		int _screenHeight;
		Ship *_ship;

		Level *_level = NULL;

	public:
		LevelManager();
		~LevelManager();

		void initialize(int screenWidth, int screenHeight, Ship *ship);

		bool update(unsigned int ticks);
		void render();

		int getScore();
	};
#endif
