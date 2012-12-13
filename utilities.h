#ifndef GALAGA_UTILITIES
  #define GALAGA_UTILITIES

  #include <iostream>

  #define GALAGA_LEFT 0
  #define GALAGA_RIGHT 1
  #define GALAGA_UP 2
  #define GALAGA_DOWN 3

  #define GALAGA_NONE -1

  // Game states
  #define GALAGA_GAME_START 0
  #define GALAGA_GAME_PLAYING 1
  #define GALAGA_GAME_PAUSED 2
  #define GALAGA_GAME_ENDED 3

  #define GALAGA_PI 3.14159

  typedef struct {
    int lifetime;
    int duration;
    int type;
    bool complete;
  } ActivePowerup;

  class Point {
    float _x;
    float _y;

  public:
    Point() { _x = 0; _y = 0; }
    Point(float x, float y) { _x = x; _y = y; }

    float getX() { return _x; }
    void setX(float x) { _x = x; }
    float getY() { return _y; }
    void setY(float y) { _y = y; }
  };

  class Rectangle {
    float _x;
    float _y;
    float _w;
    float _h;

  public:
    Rectangle();
    Rectangle(float x, float y, float w, float h);

    void operator =(Rectangle r);

    float getX() { return _x; }
    void setX(float x) { _x = x; }
    float getY() { return _y; }
    void setY(float y) { _y = y; }
    float getW() { return _w; }
    void setW(float w) { _w = w; }
    float getH() { return _h; }
    void setH(float h) { _h = h; }

    bool containsPoint(Point p);
    bool containsPoint(float x, float y);
    bool collidesWith(Rectangle target);
  };
#endif
