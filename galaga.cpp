#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;

  _font = AssetManager::getFont("normal");
  _bigFont = AssetManager::getFont("big");
  _hugeFont = AssetManager::getFont("huge");

  Rectangle shipSize = _ship.getContainer();

  _ship.setScreenBounds(_screenWidth, _screenHeight);
  _ship.moveTo(_screenWidth / 2 - shipSize.getW() / 2, _screenHeight - 2 * shipSize.getH());
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
}

bool Galaga::mainGameUpdate(unsigned int ticks, ALLEGRO_EVENT events) {
  Rectangle shipContainer;
  int i;

  if (_stateTicks == 1 && _prevGameState == GALAGA_GAME_START) {
    AssetManager::playSample("beginning", NULL);
    ++_stateTicks;
    return true;
  } else if (_stateTicks < 380 && _prevGameState == GALAGA_GAME_START) {
    ++_stateTicks;
    return true;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
    if (events.keyboard.keycode == ALLEGRO_KEY_SPACE
      && !_levelManager.isTransitioning()) {
      _ship.fire();
    } else if (events.keyboard.keycode == ALLEGRO_KEY_P) {
      _prevGameState = _gameState;
      _gameState = GALAGA_GAME_PAUSED;
      _stateTicks = 0;
      return true;
    }
  }

  if (events.type == ALLEGRO_EVENT_TIMER && !_levelManager.isTransitioning()) {
    al_get_keyboard_state(&_keyState);
    shipContainer = _ship.getContainer();

    if(al_key_down(&_keyState, ALLEGRO_KEY_LEFT)) {
      _ship.move(GALAGA_LEFT);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_RIGHT)) {
      _ship.move(GALAGA_RIGHT);
    } else if (!al_key_down(&_keyState, ALLEGRO_KEY_LEFT)
      && !al_key_down(&_keyState, ALLEGRO_KEY_RIGHT)) {
      _ship.stopMovement();
    }
  }

  // Update game objects

  _backgroundManager.update(ticks);

  _ship.update(ticks);

  _levelManager.update(ticks);

  if (_ship.isDead()) {
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

    renderLives();
    renderScore();

    int bigLineHeight = al_get_font_line_height(_bigFont);

    al_draw_text(_bigFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
      _screenHeight / 2 - bigLineHeight / 2, ALLEGRO_ALIGN_CENTRE, "READY!");
  } else {
    _backgroundManager.render();

    _levelManager.render();

    _ship.render();

    renderLives();
    renderScore();
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

  al_draw_text(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 - totalHeight / 2, ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + totalHeight / 2, ALLEGRO_ALIGN_CENTRE,
    "PRESS 'P' TO RESUME");
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

  if (_ship.getShotsFired() != 0) {
    percentAccuracy = (int)((float)_levelManager.getShotsHit()
      / _ship.getShotsFired() * 100);
  } else {
    percentAccuracy = 0;
  }

  al_draw_text(_hugeFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
    _screenHeight / 4 - hugeAscent / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
  al_draw_text(_bigFont, al_map_rgb(255, 0, 0), _screenWidth / 2,
    _screenHeight / 2 - bigAscent / 2, ALLEGRO_ALIGN_CENTRE, "RESULTS");
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + bigAscent - bigAscent / 2, ALLEGRO_ALIGN_CENTRE,
    "FINAL SCORE: %d", _levelManager.getScore());
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight / 2 + 2 * bigAscent - bigAscent / 2, ALLEGRO_ALIGN_CENTRE,
    "ACCURACY: %d%%", percentAccuracy);
  al_draw_text(_font, al_map_rgb(255, 255, 255), _screenWidth / 2,
    _screenHeight - bigAscent, ALLEGRO_ALIGN_CENTRE, "PRESS ENTER TO PLAY AGAIN");
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

  al_flip_display();
  al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Galaga::renderScore() {
  int lineHeight = al_get_font_line_height(_bigFont);

  al_draw_text(_bigFont, al_map_rgb(255, 0, 0), 10, 0, ALLEGRO_ALIGN_LEFT, "Score");
  al_draw_textf(_bigFont, al_map_rgb(255, 255, 255), 10, lineHeight,
    ALLEGRO_ALIGN_LEFT, "%d", _levelManager.getScore());
}

void Galaga::renderLives() {
  for (int i = _ship.lifeCount(); i >= 0; i--) {
    al_draw_bitmap(AssetManager::getTexture("ship"), _screenWidth - 42 * i,
      _screenHeight - 32, NULL);
  }
}
