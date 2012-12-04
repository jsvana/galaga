#ifndef GALAGA_GALAGA
  #define GALAGA_GALAGA

  #include <allegro5/allegro.h>
  #include <iostream>

  #include "bullet.h"
  #include "gamemodule.h"
  #include "ship.h"
  #include "utilities.h"

  class Galaga : public GameModule {
    Ship _ship;
    const int MOVE_SPEED = 5;
    ALLEGRO_KEYBOARD_STATE _keyState;
    int _screenWidth;
    int _screenHeight;
    ALLEGRO_EVENT_QUEUE *_eventQueue;

    bool _needsDraw;

  public:
    Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue);
    ~Galaga() { }

    bool update(unsigned int ticks);
    void render();
  };
#endif
