#include "utilities.h"

Rectangle::Rectangle() {
  _x = 0;
  _y = 0;
  _w = 0;
  _h = 0;
}

Rectangle::Rectangle(float x, float y, float w, float h) {
  _x = x;
  _y = y;
  _w = w;
  _h = h;
}

void Rectangle::operator=(Rectangle r) {
  _x = r.getX();
  _y = r.getY();
  _w = r.getW();
  _h = r.getH();
}

bool Rectangle::containsPoint(Point p) {
  return containsPoint(p.getX(), p.getY());
}

bool Rectangle::containsPoint(float x, float y) {
  return _x <= x && _x + _w >= x && _y <= y && _y + _h >= y;
}

bool Rectangle::collidesWith(Rectangle target) {
  return containsPoint(target.getX(), target.getY()) ||
      containsPoint(target.getX() + target.getW(), target.getY()) ||
      containsPoint(target.getX(), target.getY() + target.getH()) ||
      containsPoint(target.getX() + target.getW(), target.getY() + target.getH()) ||
      target.containsPoint(_x, _y) ||
      target.containsPoint(_x + _h, _y) ||
      target.containsPoint(_x, _y + _h) ||
      target.containsPoint(_x + _h, _y + _h);
}
