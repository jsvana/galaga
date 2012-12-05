#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;
  _font = al_load_font("assets/fonts/arcade.ttf", 20, NULL);
  _pewPew = al_load_sample("assets/sounds/pewpew.wav");

  _ship.moveTo(_screenWidth / 2, _screenHeight - 32);

  _enemyTextures.push_back(al_load_bitmap("assets/images/moth.png"));
  _enemyTextures.push_back(al_load_bitmap("assets/images/wasp.png"));

  _bulletTexture = al_load_bitmap("assets/images/bullet.png");

  int totalWidth = 6 * 20 + 5 * 10;
  int innerXMax = 4 * 30;

  for (int y = 0; y < 2; y++) {
    int innerXMin = _screenWidth / 2 - 4 * 30;

    for (int x = 0; x < 6; x++) {
      Rectangle bounds(innerXMin, 0, innerXMax, _screenHeight);
      int enemyX = _screenWidth / 2 - totalWidth / 2 + 30 * x;
      Enemy enemy(enemyX, _screenHeight / 4 + 30 * y, bounds, _enemyTextures[x % 2]);
      _enemies.push_back(enemy);

      innerXMin += 30;
    }
  }
}

Galaga::~Galaga() {
  _shipBullets.erase(_shipBullets.begin(), _shipBullets.end());

  al_destroy_font(_font);
  al_destroy_sample(_pewPew);

  al_destroy_bitmap(_bulletTexture);

  for (ALLEGRO_BITMAP *enemyTexture : _enemyTextures) {
    al_destroy_bitmap(enemyTexture);
  }
}

bool Galaga::update(unsigned int ticks) {
  ALLEGRO_EVENT events;
  al_wait_for_event(_eventQueue, &events);
  Rectangle shipContainer;
  int i;

  _needsDraw = false;

  if ((events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    || events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
    return false;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode == ALLEGRO_KEY_SPACE) {
    if (_shipBullets.size() < MAX_BULLETS) {
      Bullet newBullet(_ship.getContainer().getX() + _ship.getContainer().getW() / 4, _ship.getContainer().getY(), _bulletTexture);

      _shipBullets.push_back(newBullet);

      ++_shotsFired;

      float pan = (((float)_ship.getContainer().getX() + (float)_ship.getContainer().getW() / 2) - (float)_screenWidth / 2) / ((float)_screenWidth / 2);

      al_play_sample(_pewPew, .8, pan, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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

    _needsDraw = true;
  }

  _ship.update(ticks);

  std::list<Enemy>::iterator enemyIter = _enemies.begin();

  while (enemyIter != _enemies.end()) {
    (*enemyIter).update(ticks);

    if ((*enemyIter).hitTest(&_shipBullets)) {
      ++_shotHits;

      Rectangle enemyContainer = (*enemyIter).getContainer();
      ParticleManager particleManager(enemyContainer.getX(), enemyContainer.getY());
      _particleManagers.push_back(particleManager);
    }

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

  std::list<ParticleManager>::iterator particleManagerIter = _particleManagers.begin();

  while (particleManagerIter != _particleManagers.end()) {
    (*particleManagerIter).update(ticks);

    if (!(*particleManagerIter).isAlive()) {
      _particleManagers.erase(particleManagerIter++);
    } else {
      ++particleManagerIter;
    }
  }

  return true;
}

void Galaga::render() {
  if (_needsDraw) {
    int lineHeight = al_get_font_line_height(_font);

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

    int percentAccuracy = 0;

    if (_shotsFired != 0) {
      percentAccuracy = (int)((float)_shotHits / _shotsFired * 100);
    }

    al_draw_textf(_font, al_map_rgb(255, 255, 255), 10, 0,
      ALLEGRO_ALIGN_LEFT, "Enemies left: %d", (int)_enemies.size());
    al_draw_textf(_font, al_map_rgb(255, 255, 255), 10, lineHeight,
      ALLEGRO_ALIGN_LEFT, "Accuracy: %d%%", percentAccuracy);

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
  }
}
