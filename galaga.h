#ifndef GALAGA_GALAGA
  #define GALAGA_GALAGA

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include <iostream>
  #include <list>

  #include "bullet.h"
  #include "enemy.h"
  #include "gamemodule.h"
  #include "ship.h"
  #include "utilities.h"

  class Galaga : public GameModule {
    Ship _ship;
    std::list<Bullet> _shipBullets;
    std::list<Enemy> _enemies;

    ALLEGRO_EVENT_QUEUE *_eventQueue;
    ALLEGRO_KEYBOARD_STATE _keyState;
    ALLEGRO_FONT *_font;
    int _screenWidth;
    int _screenHeight;

    bool _needsDraw;

    const int MOVE_SPEED = 5;
    const int MAX_BULLETS = 2;

  public:
    Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue);
    ~Galaga();

    bool update(unsigned int ticks);
    void render();
  };
#endif
