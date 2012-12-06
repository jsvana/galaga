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
  #define GALAGA_ENEMY_STATE_MOVE 0
  #define GALAGA_ENEMY_STATE_STRIKE 0

  class Enemy {
    Rectangle _container;
    Rectangle _bounds;
    Point _initialPosition;

    ALLEGRO_BITMAP *_texture = NULL;
    ALLEGRO_SAMPLE *_sample;

    bool _alive;
    int _enemyType;
    int _frame = 0;

    int _moveSpeed = 2;

    int _previousState = NULL;
    int _currentState = GALAGA_ENEMY_STATE_IDLE;

    int _idleRadius = 5;
    float _rotation = 3 * GALAGA_PI / 2;
    float _rotationSpeed = GALAGA_PI / 16;

  public:
    Enemy();
    Enemy(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int enemyType, ALLEGRO_SAMPLE *sample);
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
