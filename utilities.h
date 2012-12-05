#ifndef GALAGA_UTILITIES
  #define GALAGA_UTILITIES

  #include <iostream>

  #define GALAGA_LEFT 0
  #define GALAGA_RIGHT 1
  #define GALAGA_UP 2
  #define GALAGA_DOWN 3

  // Game states
  #define GALAGA_GAME_START 0
  #define GALAGA_GAME_PLAYING 1
  #define GALAGA_GAME_PAUSED 2
  #define GALAGA_GAME_ENDED 3

  class Point {
    int _x;
    int _y;

  public:
    Point() { _x = 0; _y = 0; }
    Point(int x, int y) { _x = x; _y = y; }

    int getX() { return _x; }
    void setX(int x) { _x = x; }
    int getY() { return _y; }
    void setY(int y) { _y = y; }
  };

  class Rectangle {
    int _x;
    int _y;
    int _w;
    int _h;

  public:
    Rectangle();
    Rectangle(int x, int y, int w, int h);

    void operator =(Rectangle r);

    int getX() { return _x; }
    void setX(int x) { _x = x; }
    int getY() { return _y; }
    void setY(int y) { _y = y; }
    int getW() { return _w; }
    void setW(int w) { _w = w; }
    int getH() { return _h; }
    void setH(int h) { _h = h; }

    bool containsPoint(Point p);
    bool containsPoint(int x, int y);
    bool collidesWith(Rectangle target);
  };
#endif
