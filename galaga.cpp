#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;
  _font = al_load_font("arcade.ttf", 20, NULL);

  _ship.moveTo(_screenWidth / 2, _screenHeight - 20);

  int totalWidth = 6 * 20 + 5 * 10;
  int innerXMax = 4 * 30;

  for (int y = 0; y < 2; y++) {
    int innerXMin = _screenWidth / 2 - 4 * 30;

    for (int x = 0; x < 6; x++) {
      Rectangle bounds(innerXMin, 0, innerXMax, _screenHeight);
      int enemyX = _screenWidth / 2 - totalWidth / 2 + 30 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 30 * y, bounds);
      _enemies.push_back(enemy);

      innerXMin += 30;
    }
  }
}

Galaga::~Galaga() {
  _shipBullets.erase(_shipBullets.begin(), _shipBullets.end());

  al_destroy_font(_font);
}

bool Galaga::update(unsigned int ticks) {
  ALLEGRO_EVENT events;
  al_wait_for_event(_eventQueue, &events);
  Rectangle shipContainer;
  int i;

  _needsDraw = false;

  if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
    return false;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode == ALLEGRO_KEY_SPACE) {
    if (_shipBullets.size() < MAX_BULLETS) {
      Bullet newBullet(_ship.getContainer().getX() + _ship.getContainer().getW() / 4, _ship.getContainer().getY());

      _shipBullets.push_back(newBullet);
    }
  }

  if (events.type == ALLEGRO_EVENT_TIMER) {
    al_get_keyboard_state(&_keyState);
    shipContainer = _ship.getContainer();

    if(al_key_down(&_keyState, ALLEGRO_KEY_LEFT) && shipContainer.getX() > 0) {
      _ship.move(GALAGA_LEFT, MOVE_SPEED);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_RIGHT) && shipContainer.getX() < _screenWidth - shipContainer.getW()) {
      _ship.move(GALAGA_RIGHT, MOVE_SPEED);
    }

    if(al_key_down(&_keyState, ALLEGRO_KEY_UP) && shipContainer.getY() > 0) {
      _ship.move(GALAGA_UP, MOVE_SPEED);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_DOWN) && shipContainer.getY() < _screenHeight - shipContainer.getH()) {
      _ship.move(GALAGA_DOWN, MOVE_SPEED);
    }

    _needsDraw = true;
  }

  _ship.update(ticks);

  std::list<Enemy>::iterator enemyIter = _enemies.begin();

  while (enemyIter != _enemies.end()) {
    (*enemyIter).update(ticks);

    (*enemyIter).hitTest(&_shipBullets);

    if (!(*enemyIter).isAlive()) {
      _enemies.erase(enemyIter++);
    } else {
      ++enemyIter;
    }
  }

  std::list<Bullet>::iterator bulletIter = _shipBullets.begin();

  while (bulletIter != _shipBullets.end()) {
    (*bulletIter).update(ticks);

    if (!(*bulletIter).isAlive()) {
      _shipBullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  return true;
}

void Galaga::render() {
  if (_needsDraw) {
    int ascent = al_get_font_ascent(_font);

    for (Bullet bullet : _shipBullets) {
      bullet.render();
    }

    _ship.render();

    for (Enemy enemy : _enemies) {
      enemy.render();
    }

    al_draw_filled_rectangle(0, _screenHeight - 40, _screenWidth / 5, _screenHeight,
      al_map_rgb(31, 158, 14));

    al_draw_textf(_font, al_map_rgb(0, 0, 0), 10, _screenHeight - 20 - ascent / 2,
      ALLEGRO_ALIGN_LEFT, "Enemies left: %d", (int)_enemies.size());

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
  }
}
