#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;

  _font = al_load_font("assets/fonts/arcade.ttf", 20, NULL);
  _bigFont = al_load_font("assets/fonts/arcade.ttf", 40, NULL);
  _hugeFont = al_load_font("assets/fonts/arcade.ttf", 60, NULL);

  _beginningMusic = al_load_sample("assets/sounds/beginning.wav");
  _endMusic = al_load_sample("assets/sounds/end.wav");
  _shotSample = al_load_sample("assets/sounds/fighter.wav");
  _explosionSample = al_load_sample("assets/sounds/explosion.wav");

  _enemyDeathSamples.push_back(al_load_sample("assets/sounds/enemy1death.wav"));
  _enemyDeathSamples.push_back(al_load_sample("assets/sounds/enemy2death.wav"));

  _shipTexture = al_load_bitmap("assets/images/galaga.png");
  _explosionTexture = al_load_bitmap("assets/images/explosion.png");
  _enemiesTexture = al_load_bitmap("assets/images/enemies.png");
  _bulletTexture = al_load_bitmap("assets/images/galaga_bullet.png");
  _powerupsTexture = al_load_bitmap("assets/images/powerups.png");

  Rectangle shipSize = _ship.getContainer();

  _ship.moveTo(_screenWidth / 2 - shipSize.getW() / 2, _screenHeight - shipSize.getH());
  _ship.setTexture(_shipTexture);
  _ship.setExplodingTexture(_explosionTexture);

  _backgroundManager.setBounds(_screenWidth, _screenHeight);

  int totalWidth = 6 * 32 + 5 * 10;

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 6; x++) {
      int type;
      if (x < 2 || x > 3) {
        type = 0;
      } else {
        type = 1;
      }
      int enemyX = _screenWidth / 2 - totalWidth + 42 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 42 * y, _enemiesTexture, type,
        _enemyDeathSamples[x % 2]);
      _enemies.push_back(enemy);
    }
  }
}

Galaga::~Galaga() {
  _shipBullets.clear();
  _enemyBullets.clear();

  al_destroy_font(_font);
  al_destroy_font(_bigFont);

  al_destroy_sample(_beginningMusic);
  al_destroy_sample(_endMusic);
  al_destroy_sample(_shotSample);
  al_destroy_sample(_explosionSample);

  al_destroy_bitmap(_shipTexture);
  al_destroy_bitmap(_explosionTexture);
  al_destroy_bitmap(_bulletTexture);
  al_destroy_bitmap(_enemiesTexture);
  al_destroy_bitmap(_powerupsTexture);

  _enemyDeathSamples.clear();
}

void Galaga::initialize() {
  _shotsFired = 0;
  _shotHits = 0;
  _score = 0;

  _maxBullets = 2;
  _bulletCount = 1;

  _ship.moveTo(_screenWidth / 2, _screenHeight - 32);

  _gameState = GALAGA_GAME_START;

  _backgroundManager.setBounds(_screenWidth, _screenHeight);

  int totalWidth = 6 * 32 + 5 * 10;

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 6; x++) {
      int type;
      if (x < 2 || x > 3) {
        type = 0;
      } else {
        type = 1;
      }
      int enemyX = _screenWidth / 2 - totalWidth + 42 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 42 * y, _enemiesTexture, type,
        _enemyDeathSamples[x % 2]);
      _enemies.push_back(enemy);
    }
  }
}

void Galaga::cleanup() {
  _shipBullets.clear();
  _enemyBullets.clear();
  _enemies.clear();
  _particleManagers.clear();
  _powerups.clear();
  _ship.reset();

  al_stop_sample(&_endSampleID);
}

bool Galaga::startGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  if (events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode == ALLEGRO_KEY_ENTER) {
    _prevGameState = _gameState;
    _gameState = GALAGA_GAME_PLAYING;
    _stateTicks = 0;
  }

  _backgroundManager.update(ticks);

  ++_stateTicks;

  return true;
}

void Galaga::startGameRender() {
  _backgroundManager.render();

  int ascent = al_get_font_ascent(_bigFont);

  al_draw_text(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 - ascent / 2, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO START");

  al_flip_display();
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

bool Galaga::mainGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  Rectangle shipContainer;
  int i;

  _needsDraw = false;

  if (_stateTicks == 1 && _prevGameState == GALAGA_GAME_START) {
    al_play_sample(_beginningMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    ++_stateTicks;
    return true;
  } else if (_stateTicks < 380 && _prevGameState == GALAGA_GAME_START) {
    ++_stateTicks;
    return true;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
    if (events.keyboard.keycode == ALLEGRO_KEY_SPACE && !_ship.isExploding()) {
      if (_shipBullets.size() < _maxBullets) {
        int width = _bulletCount * 10 + (_bulletCount - 1) * 2;

        for (int i = 0; i < _bulletCount; i++) {
          int x = _ship.getContainer().getX() + _ship.getContainer().getW() / 4;
          Rectangle bounds(0, 0, _screenWidth, _screenHeight);
          Bullet newBullet(x - width / 2 + 12 * i, _ship.getContainer().getY(),
            _bulletTexture, true, bounds);

          _shipBullets.push_back(newBullet);
        }

        ++_shotsFired;

        float pan = (((float)_ship.getContainer().getX() + (float)_ship.getContainer().getW() / 2) - (float)_screenWidth / 2) / ((float)_screenWidth / 2);

        al_play_sample(_shotSample, 1.0, pan, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
    } else if (events.keyboard.keycode == ALLEGRO_KEY_P) {
      _prevGameState = _gameState;
      _gameState = GALAGA_GAME_PAUSED;
      _stateTicks = 0;
      return true;
    }
  }

  if (events.type == ALLEGRO_EVENT_TIMER) {
    al_get_keyboard_state(&_keyState);
    shipContainer = _ship.getContainer();

    if(al_key_down(&_keyState, ALLEGRO_KEY_LEFT) && shipContainer.getX() > 0 && !_ship.isExploding()) {
      _ship.move(GALAGA_LEFT, MOVE_SPEED);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_RIGHT)
      && shipContainer.getX() < _screenWidth - shipContainer.getW()
      && !_ship.isExploding()) {
      _ship.move(GALAGA_RIGHT, MOVE_SPEED);
    }

    _needsDraw = true;
  }

  // Update game objects

  _backgroundManager.update(ticks);

  _ship.update(ticks);

  _ship.hitTest(&_powerups);

  if (!_ship.isExploding()) {
    int prevLifeCount = _ship.lifeCount();

    _ship.hitTest(&_enemyBullets);

    if (_ship.lifeCount() < prevLifeCount) {
      al_play_sample(_explosionSample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
  }

  for (ActivePowerup powerup : _ship.getActivePowerups()) {
    if (powerup.complete) {
      removePowerup(powerup.type);
    }
  }

  std::list<Enemy>::iterator enemyIter = _enemies.begin();

  while (enemyIter != _enemies.end()) {
    (*enemyIter).update(ticks);

    if ((*enemyIter).hitTest(&_shipBullets)) {
      ++_shotHits;
      _score += 80;

      Rectangle enemyContainer = (*enemyIter).getContainer();
      ParticleManager particleManager(enemyContainer.getX(), enemyContainer.getY());
      _particleManagers.push_back(particleManager);

      if (rand() % 100 < 20) {
        Rectangle bounds(0, 0, _screenWidth, _screenHeight);
        Powerup powerup(enemyContainer.getX(), enemyContainer.getY(), _powerupsTexture, rand() % 2, bounds);
        _powerups.push_back(powerup);
      }
    }

    if (!(*enemyIter).isAlive()) {
      _enemies.erase(enemyIter++);
    } else {
      if ((*enemyIter).needsFire()) {
        int x = (*enemyIter).getContainer().getX() +
          (*enemyIter).getContainer().getW() / 4;
        Rectangle bounds(0, 0, _screenWidth, _screenHeight);
        Bullet newBullet(x, (*enemyIter).getContainer().getY(), _bulletTexture,
          false, bounds);

        _enemyBullets.push_back(newBullet);
        (*enemyIter).fire();
      }

      ++enemyIter;
    }
  }

  std::list<Bullet>::iterator bulletIter = _enemyBullets.begin();

  while (bulletIter != _enemyBullets.end()) {
    (*bulletIter).update(ticks);

    if (!(*bulletIter).isAlive()) {
      _enemyBullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  bulletIter = _shipBullets.begin();

  while (bulletIter != _shipBullets.end()) {
    (*bulletIter).update(ticks);

    if (!(*bulletIter).isAlive()) {
      _shipBullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  std::list<ParticleManager>::iterator particleManagerIter = _particleManagers.begin();

  while (particleManagerIter != _particleManagers.end()) {
    (*particleManagerIter).update(ticks);

    if (!(*particleManagerIter).isAlive()) {
      _particleManagers.erase(particleManagerIter++);
    } else {
      ++particleManagerIter;
    }
  }

  std::list<Powerup>::iterator powerupIter = _powerups.begin();

  while (powerupIter != _powerups.end()) {
    (*powerupIter).update(ticks);

    if ((*powerupIter).didHit()) {
      usePowerup((*powerupIter).getType());
    }

    if (!(*powerupIter).isAlive()) {
      _powerups.erase(powerupIter++);
    } else {
      ++powerupIter;
    }
  }

  if (_ship.lifeCount() == 0) {
    _prevGameState = _gameState;
    _gameState = GALAGA_GAME_ENDED;
    _stateTicks = 0;
  }

  ++_stateTicks;

  return true;
}

void Galaga::mainGameRender() {
  if (_stateTicks < 380 && _prevGameState == GALAGA_GAME_START) {
    _backgroundManager.render();

    _ship.render();

    int bigLineHeight = al_get_font_line_height(_bigFont);

    renderScore();

    al_draw_text(_bigFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
      _screenHeight / 2 - bigLineHeight / 2, ALLEGRO_ALIGN_CENTRE, "READY!");

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
  } else {
    if (_needsDraw) {
      int lineHeight = al_get_font_line_height(_font);

      _backgroundManager.render();

      for (Bullet bullet : _shipBullets) {
        bullet.render();
      }

      for (Bullet bullet : _enemyBullets) {
        bullet.render();
      }

      for (Powerup powerup : _powerups) {
        powerup.render();
      }

      _ship.render();

      for (Enemy enemy : _enemies) {
        enemy.render();
      }

      for (ParticleManager particleManager : _particleManagers) {
        particleManager.render();
      }

      int percentAccuracy = 0;

      if (_shotsFired != 0) {
        percentAccuracy = (int)((float)_shotHits / _shotsFired * 100);
      }

      renderLives();
      renderScore();
      renderPowerups();

      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
    }
  }
}

bool Galaga::pausedGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
    if (events.keyboard.keycode == ALLEGRO_KEY_P) {
      _prevGameState = _gameState;
      _gameState = GALAGA_GAME_PLAYING;
      _stateTicks = 0;
    }
  }

  ++_stateTicks;

  return true;
}

void Galaga::pausedGameRender() {
  int lineHeight = al_get_font_line_height(_font);

  _backgroundManager.render();

  for (Bullet bullet : _shipBullets) {
    bullet.render();
  }

  _ship.render();

  for (Enemy enemy : _enemies) {
    enemy.render();
  }

  for (ParticleManager particleManager : _particleManagers) {
    particleManager.render();
  }

  int bigAscent = al_get_font_ascent(_bigFont);
  int smallAscent = al_get_font_ascent(_font);

  int totalHeight = bigAscent + 20 + smallAscent;

  renderLives();
  renderScore();
  renderPowerups();

  al_draw_text(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2, _screenHeight / 2 - totalHeight / 2,
    ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2, _screenHeight / 2 + totalHeight / 2,
    ALLEGRO_ALIGN_CENTRE, "PRESS 'P' TO RESUME");

  al_flip_display();
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

bool Galaga::endGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  if (_stateTicks == 1 && _prevGameState == GALAGA_GAME_PLAYING) {
    al_play_sample(_endMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &_endSampleID);
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode == ALLEGRO_KEY_ENTER) {
    _prevGameState = _gameState;
    _gameState = GALAGA_GAME_START;
    _stateTicks = 0;
    cleanup();
    initialize();
  }

  _needsDraw = true;

  _backgroundManager.update(ticks);

  ++_stateTicks;

  return true;
}

void Galaga::endGameRender() {
  _backgroundManager.render();

  int hugeAscent = al_get_font_ascent(_hugeFont);
  int bigAscent = al_get_font_ascent(_bigFont);
  int smallAscent = al_get_font_ascent(_font);

  int percentAccuracy = 0;

  if (_shotsFired != 0) {
    percentAccuracy = (int)((float)_shotHits / _shotsFired * 100);
  }

  al_draw_text(_hugeFont, al_map_rgb(255, 0, 0), _screenWidth / 2, _screenHeight / 4 - hugeAscent / 2,
    ALLEGRO_ALIGN_CENTRE, "GAME OVER");
  al_draw_text(_bigFont, al_map_rgb(255, 0, 0), _screenWidth / 2, _screenHeight / 2 - bigAscent / 2,
    ALLEGRO_ALIGN_CENTRE, "RESULTS");
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2, _screenHeight / 2 + bigAscent - bigAscent / 2,
    ALLEGRO_ALIGN_CENTRE, "FINAL SCORE: %d", _score);
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2, _screenHeight / 2 + 2 * bigAscent - bigAscent / 2,
    ALLEGRO_ALIGN_CENTRE, "ACCURACY: %d%%", percentAccuracy);
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2, _screenHeight - bigAscent,
    ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO PLAY AGAIN");

  al_flip_display();
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

bool Galaga::update(unsigned int ticks) {
  ALLEGRO_EVENT events;
  al_wait_for_event(_eventQueue, &events);

  if ((events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    || events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
    return false;
  }

  switch (_gameState) {
    case GALAGA_GAME_START:
      return startGameUpdate(ticks, events);
    case GALAGA_GAME_PLAYING:
      return mainGameUpdate(ticks, events);
    case GALAGA_GAME_PAUSED:
      return pausedGameUpdate(ticks, events);
    case GALAGA_GAME_ENDED:
      return endGameUpdate(ticks, events);
  }

  return true;
}

void Galaga::render() {
  switch (_gameState) {
    case GALAGA_GAME_START:
      startGameRender();
      break;
    case GALAGA_GAME_PLAYING:
      mainGameRender();
      break;
    case GALAGA_GAME_PAUSED:
      pausedGameRender();
      break;
    case GALAGA_GAME_ENDED:
      endGameRender();
      break;
  }
}

void Galaga::renderScore() {
  int lineHeight = al_get_font_line_height(_bigFont);

  al_draw_text(_bigFont, al_map_rgb(255, 0, 0), 10, 0, ALLEGRO_ALIGN_LEFT, "Score");
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), 10, lineHeight,
    ALLEGRO_ALIGN_LEFT, "%d", _score);
}

void Galaga::renderPowerups() {
  std::list<ActivePowerup> powerups = _ship.getActivePowerups();
  int powerupsToRender[2] = {0, 0};

  for (ActivePowerup powerup : powerups) {
    if (!powerup.complete) {
      ++powerupsToRender[powerup.type];
    }
  }

  int y = _screenHeight - 32;
  int textOffset = 0;
  int ascent = al_get_font_ascent(_font);
  int toRender = 0;

  for (int i = 0; i < 2; i++) {
    if (powerupsToRender[i] > 0) {
      al_draw_bitmap_region(_powerupsTexture, 0, 32 * i,
        32, 32, 40 * toRender + textOffset, y, NULL);
      ++toRender;

      if (powerupsToRender[i] > 1) {
        al_draw_textf(_font, al_map_rgb(255, 255, 255),
          40 * (i + 1) + textOffset, y + 16 - ascent / 2, ALLEGRO_ALIGN_LEFT, "x%d",
          powerupsToRender[i]);
        textOffset += al_get_text_width(_font, "x1");
      }
    }
  }
}

void Galaga::renderLives() {
  for (int i = 0; i <= _ship.lifeCount(); i++) {
    al_draw_bitmap(_shipTexture, _screenWidth - 42 * i, _screenHeight - 42, NULL);
  }
}

void Galaga::usePowerup(int type) {
  switch (type) {
    case GALAGA_POWERUP_DOUBLE:
      _ship.addPowerup(type, 500);
      _maxBullets /= _bulletCount;
      ++_bulletCount;
      _maxBullets *= _bulletCount;
      break;
    case GALAGA_POWERUP_MORE_BULLETS:
      _ship.addPowerup(type, 500);
      _maxBullets += 2;
      break;
  }
}

void Galaga::removePowerup(int type) {
  switch (type) {
    case GALAGA_POWERUP_DOUBLE:
      _maxBullets /= _bulletCount;
      --_bulletCount;
      _maxBullets *= _bulletCount;
      break;
    case GALAGA_POWERUP_MORE_BULLETS:
      _maxBullets -= 2;
      break;
  }
}
