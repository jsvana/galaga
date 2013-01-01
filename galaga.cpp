#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;

  _font = AssetManager::getFont("normal");
  _bigFont = AssetManager::getFont("big");
  _hugeFont = AssetManager::getFont("huge");

  Rectangle shipSize = _ship.getContainer();

  _ship.moveTo(_screenWidth / 2 - shipSize.getW() / 2, _screenHeight - shipSize.getH());
  _ship.setTexture(AssetManager::getTexture("ship"));
  _ship.setExplodingTexture(AssetManager::getTexture("explosion"));

  _backgroundManager.setBounds(_screenWidth, _screenHeight);

  _levelManager.initialize(_screenWidth, _screenHeight, &_ship);
}

Galaga::~Galaga() {

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

  _levelManager.initialize(_screenWidth, _screenHeight, &_ship);
}

void Galaga::cleanup() {
  _enemyBullets.clear();
  _enemies.clear();
  _particleManagers.clear();
  _powerups.clear();
  _ship.reset();

  AssetManager::stopSample(&_endSampleID);
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
    AssetManager::playSample("beginning", NULL);
    ++_stateTicks;
    return true;
  } else if (_stateTicks < 380 && _prevGameState == GALAGA_GAME_START) {
    ++_stateTicks;
    return true;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
    if (events.keyboard.keycode == ALLEGRO_KEY_SPACE && !_ship.isExploding()) {
      if (_ship.getBulletCount() < _maxBullets) {
        int width = _bulletCount * 10 + (_bulletCount - 1) * 2;

        for (int i = 0; i < _bulletCount; i++) {
          int x = _ship.getContainer().getX() + _ship.getContainer().getW() / 4;
          Rectangle bounds(0, 0, _screenWidth, _screenHeight);
          Bullet newBullet(x - width / 2 + 12 * i, _ship.getContainer().getY(),
            AssetManager::getTexture("bullet"), true, bounds);

          _ship.addBullet(newBullet);
        }

        ++_shotsFired;

        AssetManager::playSample("shot", NULL);
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

    if(al_key_down(&_keyState, ALLEGRO_KEY_LEFT) && shipContainer.getX() > 0
      && !_ship.isExploding()) {
      _ship.move(GALAGA_LEFT);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_RIGHT)
      && shipContainer.getX() < _screenWidth - shipContainer.getW()
      && !_ship.isExploding()) {
      _ship.move(GALAGA_RIGHT);
    } else if (!al_key_down(&_keyState, ALLEGRO_KEY_LEFT)
      && !al_key_down(&_keyState, ALLEGRO_KEY_RIGHT)) {
      _ship.stopMovement();
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
      AssetManager::playSample("explosion", NULL);
    }
  }

  for (ActivePowerup powerup : _ship.getActivePowerups()) {
    if (powerup.complete) {
      removePowerup(powerup.type);
    }
  }

  // std::list<Powerup>::iterator powerupIter = _powerups.begin();

  // while (powerupIter != _powerups.end()) {
  //   powerupIter->update(ticks);

  //   if (powerupIter->didHit()) {
  //     usePowerup(powerupIter->getType());
  //   }

  //   if (!powerupIter->isAlive()) {
  //     _powerups.erase(powerupIter++);
  //   } else {
  //     ++powerupIter;
  //   }
  // }

  _levelManager.update(ticks);

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

      _levelManager.render();

      _ship.render();

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

  _levelManager.render();

  _ship.render();

  int bigAscent = al_get_font_ascent(_bigFont);
  int smallAscent = al_get_font_ascent(_font);

  int totalHeight = bigAscent + 20 + smallAscent;

  renderLives();
  renderScore();
  renderPowerups();

  al_draw_text(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 - totalHeight / 2, ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + totalHeight / 2, ALLEGRO_ALIGN_CENTRE,
    "PRESS 'P' TO RESUME");

  al_flip_display();
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

bool Galaga::endGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  if (_stateTicks == 1 && _prevGameState == GALAGA_GAME_PLAYING) {
    AssetManager::playSample("end", &_endSampleID);
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

  al_draw_text(_hugeFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
    _screenHeight / 4 - hugeAscent / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
  al_draw_text(_bigFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
    _screenHeight / 2 - bigAscent / 2, ALLEGRO_ALIGN_CENTRE, "RESULTS");
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + bigAscent - bigAscent / 2, ALLEGRO_ALIGN_CENTRE,
    "FINAL SCORE: %d", _score);
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + 2 * bigAscent - bigAscent / 2, ALLEGRO_ALIGN_CENTRE,
    "ACCURACY: %d%%", percentAccuracy);
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight - bigAscent, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO PLAY AGAIN");

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
    ALLEGRO_ALIGN_LEFT, "%d", _levelManager.getScore());
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
      al_draw_bitmap_region(AssetManager::getTexture("powerups"), 0, 32 * i,
        32, 32, 40 * toRender + textOffset, y, NULL);
      ++toRender;

      if (powerupsToRender[i] > 1) {
        al_draw_textf(_font, al_map_rgb(255, 255, 255),
          40 * (i + 1) + textOffset, y + 16 - ascent / 2, ALLEGRO_ALIGN_LEFT,
          "x%d", powerupsToRender[i]);
        textOffset += al_get_text_width(_font, "x1");
      }
    }
  }
}

void Galaga::renderLives() {
  for (int i = 0; i <= _ship.lifeCount(); i++) {
    al_draw_bitmap(AssetManager::getTexture("ship"), _screenWidth - 42 * i,
      _screenHeight - 42, NULL);
  }
}

// TODO: move inside Ship
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

// TODO: move inside Ship
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
