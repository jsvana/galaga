#ifndef GALAGA_ENEMY
  #define GALAGA_ENEMY

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_image.h>
  #include <allegro5/allegro_primitives.h>
  #include <cmath>
  #include <iostream>
  #include <list>

  #include "bullet.h"
  #include "utilities.h"

  #define GALAGA_ENEMY_STATE_IDLE 0
  #define GALAGA_ENEMY_STATE_MOVE 1
  #define GALAGA_ENEMY_STATE_GROW 2
  #define GALAGA_ENEMY_STATE_STRIKE 3

  class Enemy {
    Rectangle _container;

    ALLEGRO_BITMAP *_texture = NULL;
    ALLEGRO_SAMPLE *_sample;

    float _growX = 0.0;
    float _growY = 0.0;

    float _growRateX = 0.0;
    float _growRateY = 0.0;

    Point _initialPosition;

    int _xSpeed = -3;

    bool _alive;
    int _enemyType;
    int _frame = 0;

    int _previousState = NULL;
    int _currentState = GALAGA_ENEMY_STATE_IDLE;

    unsigned int _stateTicks;

  public:
    Enemy();
    Enemy(int x, int y, ALLEGRO_BITMAP *texture, int enemyType, ALLEGRO_SAMPLE *sample);
    ~Enemy();

    bool update(unsigned int ticks);
    void render();

    Rectangle getContainer() { return _container; }

    bool isAlive() { return _alive; }

    void kill() { _alive = false; }
    void vivify() { _alive = false; }

    bool hitTest(std::list<Bullet> *bullets);

    void moveTo(int x, int y);
  };
#endif
