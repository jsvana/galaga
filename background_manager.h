#ifndef GALAGA_BACKGROUND_MANAGER
  #define GALAGA_BACKGROUND_MANAGER

  #include <list>

  #include "background.h"
  #include "utilities.h"

  class BackgroundManager {
    std::list<Background> _backgrounds;

  public:
    BackgroundManager();
    BackgroundManager(int screenWidth, int screenHeight);

    void setBounds(int screenWidth, int screenHeight);

    bool update(unsigned int ticks);
    void render();
  };
#endif
