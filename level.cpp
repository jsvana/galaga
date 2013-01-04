#include "level.h"

Level::Level(int screenWidth, int screenHeight, int difficulty, Ship *ship, int score, int shotsHit) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_difficulty = difficulty;
	_ship = ship;
	_score = score;
	_shotsHit = shotsHit;

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

      Rectangle bounds(42 * x, 0, _screenWidth - 42 * (5 - x), _screenHeight);
      int enemyX = _screenWidth / 2 - totalWidth + 42 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 42 * y, bounds,
        AssetManager::getTexture("enemies"), type, deathSound, _difficulty);
      _enemies.push_back(enemy);
    }
  }
}

Level::~Level() {
	_enemies.clear();
	_enemyBullets.clear();
	_particleManagers.clear();
	_powerups.clear();
}

bool Level::update(unsigned int ticks) {
	std::list<Enemy>::iterator enemyIter = _enemies.begin();

	_ship->hitTest(&_enemyBullets);

	_ship->hitTest(&_powerups);

  std::list<Powerup>::iterator powerupIter = _powerups.begin();

  while (powerupIter != _powerups.end()) {
    powerupIter->update(ticks);

    if (powerupIter->didHit()) {
      _ship->addPowerup(powerupIter->getType());
    }

    if (!powerupIter->isAlive()) {
      _powerups.erase(powerupIter++);
    } else {
      ++powerupIter;
    }
  }

	bool enemyDead = false;

	while (enemyIter != _enemies.end()) {
		enemyIter->update(ticks);
		enemyIter->decideShot(*_ship);

		if (enemyIter->hitTest(_ship->getBullets())) {
			if (!enemyDead) {
				enemyDead = true;

				AssetManager::playSample(enemyIter->getSample(), NULL);
			}

			++_shotsHit;
			_score += enemyIter->getPointsWorth();

			Rectangle enemyContainer = enemyIter->getContainer();
			ParticleManager particleManager(enemyContainer.getX(), enemyContainer.getY());
			_particleManagers.push_back(particleManager);

			// TODO: make less constant-y
			if (rand() % 100 < 20) {
				Rectangle bounds(0, 0, _screenWidth, _screenHeight);
				Powerup powerup(enemyContainer.getX(), enemyContainer.getY(),
					AssetManager::getTexture("powerups"), rand() % 2, bounds);
				_powerups.push_back(powerup);
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

	for (Powerup powerup : _powerups) {
		powerup.render();
	}

	for (Enemy enemy : _enemies) {
		enemy.render();
	}

  renderPowerups();
}

bool Level::isComplete() {
	return _enemies.size() == 0 && _enemyBullets.size() == 0
		&& _particleManagers.size() == 0;
}

void Level::renderPowerups() {
	ALLEGRO_FONT *font = AssetManager::getFont("normal");

	int y = _screenHeight - 32;
  int textOffset = 0;
  int ascent = al_get_font_ascent(font);
  int toRender = 0;

  std::list<ActivePowerup> powerups = _ship->getActivePowerups();
  int powerupsToRender[2] = {0, 0};

  for (ActivePowerup powerup : powerups) {
    if (!powerup.complete) {
      ++powerupsToRender[powerup.type];
    }
  }

  for (int i = 0; i < 2; i++) {
    if (powerupsToRender[i] > 0) {
      al_draw_bitmap_region(AssetManager::getTexture("powerups"), 0, 32 * i,
        32, 32, 40 * toRender + textOffset, y, NULL);
      ++toRender;

      if (powerupsToRender[i] > 1) {
        al_draw_textf(font, al_map_rgb(255, 255, 255),
          40 * (i + 1) + textOffset, y + 16 - ascent / 2, ALLEGRO_ALIGN_LEFT,
          "x%d", powerupsToRender[i]);
        textOffset += al_get_text_width(font, "x1");
      }
    }
  }
}
