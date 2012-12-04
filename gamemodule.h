#ifndef GALAGA_GAMEMODULE
  #define GALAGA_GAMEMODULE

  class GameModule {
  public:
    virtual ~GameModule();

    virtual bool update(unsigned int ticks);
    virtual void render();
  };
#endif
