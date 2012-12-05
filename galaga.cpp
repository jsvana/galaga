#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;
  _ship.moveTo(_screenWidth / 2, _screenHeight - 20);

  int totalWidth = 6 * 20 + 5 * 10;

  for (int i = 0; i < 6; i++) {
    int x = _screenWidth / 2 - totalWidth / 2 + 30 * i;
    Enemy enemy(x, 0);
    _enemies.push_back(enemy);
  }
}

Galaga::~Galaga() {
  _bullets.erase(_bullets.begin(), _bullets.end());
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
    if (_bullets.size() < MAX_BULLETS) {
      Bullet newBullet(_ship.getContainer().getX() + _ship.getContainer().getW() / 4, _ship.getContainer().getY());

      _bullets.push_back(newBullet);
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

  // if (_enemy.hitTest(&_bullets)) {
  //   std::cout << "[LOG] hit" << std::endl;
  // }

  _ship.update(ticks);

  std::list<Enemy>::iterator enemyIter = _enemies.begin();

  while (enemyIter != _enemies.end()) {
    (*enemyIter).update(ticks);

    (*enemyIter).hitTest(&_bullets);

    if (!(*enemyIter).isAlive()) {
      _enemies.erase(enemyIter++);
    } else {
      ++enemyIter;
    }
  }

  std::list<Bullet>::iterator bulletIter = _bullets.begin();

  while (bulletIter != _bullets.end()) {
    (*bulletIter).update(ticks);

    if (!(*bulletIter).isAlive()) {
      _bullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  return true;
}

void Galaga::render() {
  int i;

  if (_needsDraw) {
    for (Bullet bullet : _bullets) {
      bullet.render();
    }

    _ship.render();

    for (Enemy enemy : _enemies) {
      enemy.render();
    }

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
  }
}
