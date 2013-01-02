#ifndef GALAGA_GALAGA
  #define GALAGA_GALAGA

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include <allegro5/allegro_image.h>
  #include <iostream>
  #include <list>
  #include <vector>

  #include "asset_manager.h"
  #include "background_manager.h"
  #include "bullet.h"
  #include "enemy.h"
  #include "gamemodule.h"
  #include "level.h"
  #include "level_manager.h"
  #include "particle_manager.h"
  #include "powerup.h"
  #include "ship.h"
  #include "utilities.h"

  class Galaga : public GameModule {
    Ship _ship;
    std::list<Powerup> _powerups;
    BackgroundManager _backgroundManager;
    LevelManager _levelManager;

    int _gameState = GALAGA_GAME_START;
    int _prevGameState = NULL;

    unsigned int _stateTicks = 0;
    ALLEGRO_BITMAP *_powerupsTexture;

    ALLEGRO_EVENT_QUEUE *_eventQueue;
    ALLEGRO_KEYBOARD_STATE _keyState;

    ALLEGRO_FONT *_font;
    ALLEGRO_FONT *_bigFont;
    ALLEGRO_FONT *_hugeFont;

    ALLEGRO_SAMPLE_ID _endSampleID;

    unsigned int _shotsFired = 0;
    unsigned int _shotHits = 0;

    unsigned int _score = 0;

    int _screenWidth;
    int _screenHeight;

    bool _needsDraw;

    int _bulletCount = 1;
    int _maxBullets = 2;

  public:
    Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue);
    ~Galaga();

    void initialize();
    void cleanup();

    bool startGameUpdate(unsigned int ticks, ALLEGRO_EVENT events);
    void startGameRender();

    bool mainGameUpdate(unsigned int ticks, ALLEGRO_EVENT events);
    void mainGameRender();

    bool pausedGameUpdate(unsigned int ticks, ALLEGRO_EVENT events);
    void pausedGameRender();

    bool endGameUpdate(unsigned int ticks, ALLEGRO_EVENT events);
    void endGameRender();

    bool update(unsigned int ticks);
    void render();

    void renderLives();
    void renderScore();
    void renderPowerups();

    void usePowerup(int type);
    void removePowerup(int type);
  };
#endif
