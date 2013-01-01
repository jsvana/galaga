#include "level.h"

Level::Level(int screenWidth, int screenHeight, int difficulty, Ship *ship, int score, int shotHits) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_difficulty = difficulty;
	_ship = ship;
	_score = score;
	_shotHits = shotHits;

	int totalWidth = 6 * 32 + 5 * 10;

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 6; x++) {
      int type;
      std::string deathSound;

      if (x < 2 || x > 3) {
        type = 0;
        deathSound = "enemy_death_1";
      } else {
        type = 1;
        deathSound = "enemy_death_2";
      }

      int enemyX = _screenWidth / 2 - totalWidth + 42 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 42 * y,
        AssetManager::getTexture("enemies"), type, deathSound);
      _enemies.push_back(enemy);
    }
  }
}

Level::~Level() {
	_enemies.clear();
	_enemyBullets.clear();
	_particleManagers.clear();
}

bool Level::update(unsigned int ticks) {
	std::list<Enemy>::iterator enemyIter = _enemies.begin();

	_ship->hitTest(&_enemyBullets);

	bool enemyDead = false;

	while (enemyIter != _enemies.end()) {
		enemyIter->update(ticks);
		enemyIter->decideShot(*_ship);

		if (enemyIter->hitTest(_ship->getBullets())) {
			if (!enemyDead) {
				enemyDead = true;

				AssetManager::playSample(enemyIter->getSample(), NULL);
			}

			++_shotHits;
			_score += enemyIter->getPointsWorth();

			Rectangle enemyContainer = enemyIter->getContainer();
			ParticleManager particleManager(enemyContainer.getX(), enemyContainer.getY());
			_particleManagers.push_back(particleManager);

			// TODO: make less constant-y
			if (rand() % 100 < 20) {
				Rectangle bounds(0, 0, _screenWidth, _screenHeight);
				// Powerup powerup(enemyContainer.getX(), enemyContainer.getY(),
				// 	AssetManager::getTexture("powerups"), rand() % 2, bounds);
				// _powerups.push_back(powerup);
			}
		}

		if (!enemyIter->isAlive()) {
			_enemies.erase(enemyIter++);
		} else {
			if (enemyIter->needsFire()) {
				int x = enemyIter->getContainer().getX() +
					enemyIter->getContainer().getW() / 4;
				Rectangle bounds(0, 0, _screenWidth, _screenHeight);
				Bullet newBullet(x, enemyIter->getContainer().getY(),
					AssetManager::getTexture("bullet"), false, bounds);

				_enemyBullets.push_back(newBullet);
				enemyIter->fire();
			}

			++enemyIter;
		}
	}

	std::list<Bullet>::iterator bulletIter = _enemyBullets.begin();

  while (bulletIter != _enemyBullets.end()) {
    bulletIter->update(ticks);

    if (!bulletIter->isAlive()) {
      _enemyBullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  std::list<ParticleManager>::iterator particleManagerIter = _particleManagers.begin();

  while (particleManagerIter != _particleManagers.end()) {
    particleManagerIter->update(ticks);

    if (!particleManagerIter->isAlive()) {
      _particleManagers.erase(particleManagerIter++);
    } else {
      ++particleManagerIter;
    }
  }

	return true;
}

void Level::render() {
	for (Bullet bullet : _enemyBullets) {
		bullet.render();
	}

	for (ParticleManager particleManager : _particleManagers) {
		particleManager.render();
	}

	for (Enemy enemy : _enemies) {
		enemy.render();
	}
}

bool Level::levelComplete() {
	return _enemies.size() == 0 && _enemyBullets.size() == 0
		&& _particleManagers.size() == 0;
}
