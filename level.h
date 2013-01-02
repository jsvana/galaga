#ifndef GALAGA_LEVEL
	#define GALAGA_LEVEL

	#include <allegro5/allegro.h>

	#include <list>

	#include "asset_manager.h"
	#include "bullet.h"
	#include "enemy.h"
	#include "particle_manager.h"
	#include "powerup.h"
	#include "ship.h"
	#include "utilities.h"

	class Level {
		int _screenWidth;
		int _screenHeight;
		Ship *_ship;
		int _difficulty;
		int _score;
		int _shotHits;

		std::list<Enemy> _enemies;
		std::list<Bullet> _enemyBullets;
		std::list<ParticleManager> _particleManagers;
		std::list<Powerup> _powerups;

	public:
		Level(int screenWidth, int screenHeight, int difficulty, Ship *ship, int score, int shotHits);
		~Level();

		bool update(unsigned int ticks);
		void render();

		int getScore() { return _score; }
		int getShotHits() { return _shotHits; }

		bool levelComplete();

		void usePowerup(int type);
		void removePowerup(int type);

		void renderPowerups();
	};
#endif
