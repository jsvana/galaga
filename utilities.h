#ifndef GALAGA_UTILITIES
  #define GALAGA_UTILITIES

  #define GALAGA_LEFT 0
  #define GALAGA_RIGHT 1
  #define GALAGA_UP 2
  #define GALAGA_DOWN 3

  class Point {
    int _x;
    int _y;

  public:
    Point();
    Point(int x, int y) { _x = x; _y = y; }

    int getX() { return _x; }
    void setX(int x) { _x = x; }
    int getY() { return _y; }
    void setY(int y) { _y = y; }
  };
#endif
