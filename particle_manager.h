#ifndef GALAGA_PARTICLE_MANAGER
  #define GALAGA_PARTICLE_MANAGER

  #include <stdlib.h>
  #include <iostream>
  #include <list>

  #include "particle.h"
  #include "utilities.h"

  class ParticleManager {
    std::list<Particle> _particles;

    bool _alive = true;

  public:
    ParticleManager(int x, int y);
    ~ParticleManager();

    bool update(unsigned int ticks);
    void render();

    bool isAlive() { return _alive; }

    void kill() { _alive = false; }
    void vivify() { _alive = true; }
  };
#endif
