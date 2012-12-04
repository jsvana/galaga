#include "enemy.h"

Enemy::Enemy() {
}

Enemy::Enemy(int x, int y) {
  _position.setX(x);
  _position.setY(y);
}

Enemy::~Enemy() {
}

bool Enemy::hitTest(std::vector<Bullet> *bullets) {
  // int i;

  // for (i = 0; i < bullets->size(); i++) {
  //   Point pos = *bullets[i].getPosition();

  //   if (_position.getX() )
  // }
  // return false;

  return false;
}

bool Enemy::update(unsigned int ticks) {
  return true;
}

void Enemy::render() {
  al_draw_filled_rectangle(_position.getX() - 10, _position.getY() - 10, _position.getX() + 10, _position.getY() + 10, al_map_rgb(255, 0, 255));
}

void Enemy::moveTo(int x, int y) {
  _position.setX(x);
  _position.setY(y);
}
