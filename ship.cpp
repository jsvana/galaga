#include "bullet.h"
#include "ship.h"

Ship::Ship() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(32);
  _container.setH(32);
}

Ship::Ship(int x, int y, ALLEGRO_BITMAP *texture, ALLEGRO_BITMAP *explodingTexture) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(32);
  _container.setH(32);

  _texture = texture;
  _explodingTexture = explodingTexture;
}

Ship::~Ship() {
  _activePowerups.clear();
  _bullets.clear();
}

void Ship::setScreenBounds(int screenWidth, int screenHeight) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
}

void Ship::reset() {
  _previousState = NULL;
  _currentState = GALAGA_SHIP_STATE_MAIN;
  _stateTicks = 0;
  _frame = 0;

  _lives = 3;

  _bullets.clear();

  clearActivePowerups();
}

bool Ship::move(int direction) {
  if (isExploding()) {
    return false;
  }

  if (direction == GALAGA_LEFT && _container.getX() > 0) {
    _container.setX(_container.getX() - MOVE_SPEED);
    _direction = GALAGA_LEFT;

    return true;
  } else if (direction == GALAGA_RIGHT
    && _container.getX() < _screenWidth - _container.getW()) {
    _container.setX(_container.getX() + MOVE_SPEED);
    _direction = GALAGA_RIGHT;

    return true;
  } else {
    return false;
  }
}

bool Ship::fire() {
  if (getBulletCount() < _maxBullets && !isExploding()) {
    int width = _bulletCount * 10 + (_bulletCount - 1) * 2;

    for (int i = 0; i < _bulletCount; i++) {
      int x = _container.getX() + _container.getW() / 4;
      Rectangle bounds(0, 0, _screenWidth, _screenHeight);
      Bullet newBullet(x - width / 2 + 12 * i, _container.getY(),
        AssetManager::getTexture("bullet"), true, bounds);

      addBullet(newBullet);
    }

    ++_shotsFired;

    AssetManager::playSample("shot", NULL);

    return true;
  } else {
    return false;
  }
}

bool Ship::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);

  return true;
}

bool Ship::hitTest(std::list<Powerup> *powerups) {
  if (!isExploding()) {
    for (Powerup& powerup : *powerups) {
      Rectangle powerupContainer = powerup.getContainer();
      if (powerup.isAlive() && _container.collidesWith(powerupContainer)) {
        powerup.kill();
        powerup.hit();
        return true;
      }
    }
  }

  return false;
}

bool Ship::hitTest(std::list<Bullet> *bullets) {
  if (!isExploding()) {
    for (Bullet& bullet : *bullets) {
      Rectangle bulletContainer = bullet.getContainer();
      if (bullet.isAlive() && _container.collidesWith(bulletContainer)) {
        bullet.kill();
        kill();
        return true;
      }
    }
  }

  return false;
}

void Ship::kill() {
  --_lives;

  _previousState = _currentState;
  _currentState = GALAGA_SHIP_STATE_EXPLODING;
  _stateTicks = 0;

  clearActivePowerups();

  AssetManager::playSample("explosion", NULL);

  if (_lives < 0) {
    _alive = false;
  }
}

void Ship::update(unsigned int ticks) {
  std::list<ActivePowerup>::iterator powerupIter = _activePowerups.begin();
  std::list<Bullet>::iterator bulletIter = _bullets.begin();

  while (bulletIter != _bullets.end()) {
    bulletIter->update(ticks);

    if (!bulletIter->isAlive()) {
      _bullets.erase(bulletIter++);
    } else {
      ++bulletIter;
    }
  }

  switch (_currentState) {
    case GALAGA_SHIP_STATE_MAIN:

      for (ActivePowerup& powerup : _activePowerups) {
        ++powerup.lifetime;

        if (powerup.lifetime >= powerup.duration) {
          powerup.complete = true;
        }
      }

      break;
    case GALAGA_SHIP_STATE_EXPLODING:
      if (_stateTicks != 0 && _stateTicks % 15 == 0) {
        ++_frame;
      }

      if (_frame == 4) {
        _frame = 0;

        _previousState = _currentState;
        _currentState = GALAGA_SHIP_STATE_MAIN;
        _stateTicks = 0;
      }

      break;
  }

  ++_stateTicks;
}

void Ship::render() {
  int x = _container.getX();
  int y = _container.getY();

  for (Bullet bullet : _bullets) {
    bullet.render();
  }

  if (_currentState == GALAGA_SHIP_STATE_MAIN) {
    al_draw_bitmap(_texture, x, y, NULL);
  } else {
    al_draw_bitmap_region(_explodingTexture, 32 * _frame, 0, 32, 32, x, y, NULL);
  }
}

std::list<ActivePowerup> Ship::getActivePowerups() {
  std::list<ActivePowerup> oldPowerups(_activePowerups);

  std::list<ActivePowerup>::iterator powerupIter = _activePowerups.begin();

  while (powerupIter != _activePowerups.end()) {
    ++powerupIter->lifetime;

    if (powerupIter->complete) {
      std::cout << "[LOG] delete type " << powerupIter->type << std::endl;
      switch (powerupIter->type) {
        case GALAGA_POWERUP_DOUBLE:
          std::cout << "[LOG] maxBullets: " << _maxBullets << ", bulletCount: " << _bulletCount << std::endl;
          _maxBullets /= _bulletCount;
          --_bulletCount;
          _maxBullets *= _bulletCount;
          std::cout << "[LOG] maxBullets: " << _maxBullets << ", bulletCount: " << _bulletCount << std::endl;
          break;

        case GALAGA_POWERUP_MORE_BULLETS:
          _maxBullets -= 2 * _bulletCount;
          break;
      }

      _activePowerups.erase(powerupIter++);
    } else {
      ++powerupIter;
    }
  }

  return oldPowerups;
}

void Ship::addPowerup(int type) {
  ActivePowerup newPowerup;
  newPowerup.type = type;
  newPowerup.lifetime = 0;
  newPowerup.complete = false;

  switch (type) {
    case GALAGA_POWERUP_DOUBLE:
      newPowerup.duration = 500;

      std::cout << "[LOG] maxBullets: " << _maxBullets << ", bulletCount: " << _bulletCount << std::endl;
      _maxBullets /= _bulletCount;
      ++_bulletCount;
      _maxBullets *= _bulletCount;
      std::cout << "[LOG] maxBullets: " << _maxBullets << ", bulletCount: " << _bulletCount << std::endl;
      break;

    case GALAGA_POWERUP_MORE_BULLETS:
      newPowerup.duration = 500;

      _maxBullets += 2 * _bulletCount;
      break;
  }

  _activePowerups.push_back(newPowerup);
}

void Ship::clearActivePowerups() {
  _activePowerups.clear();

  _maxBullets = 2;
  _bulletCount = 1;
}

void Ship::addBullet(Bullet bullet) {
  _bullets.push_back(bullet);
}
