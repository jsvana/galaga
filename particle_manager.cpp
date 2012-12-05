#include "particle_manager.h"

ParticleManager::ParticleManager(int x, int y) {
  int particleCount = rand() % 6 + 5;

  for (int i = 0; i < particleCount; i++) {
    Particle particle(x, y);
    _particles.push_back(particle);
  }
}

ParticleManager::~ParticleManager() {
  _particles.clear();
}

bool ParticleManager::update(unsigned int ticks) {
  std::list<Particle>::iterator particleIter = _particles.begin();

  while (particleIter != _particles.end()) {
    (*particleIter).update(ticks);

    if (!(*particleIter).isAlive()) {
      _particles.erase(particleIter++);
    } else {
      ++particleIter;
    }
  }

  if (_particles.empty()) {
    kill();
  }

  return true;
}

void ParticleManager::render() {
  for (Particle particle : _particles) {
    particle.render();
  }
}
