#include "background_manager.h"

BackgroundManager::BackgroundManager() {

}

BackgroundManager::BackgroundManager(int screenWidth, int screenHeight) {
  for (int i = 4; i >= 1; i--) {
    Background background(screenWidth, screenHeight, i);
    _backgrounds.push_front(background);
  }
}

void BackgroundManager::setBounds(int screenWidth, int screenHeight) {
  _backgrounds.clear();

  for (int i = 4; i >= 1; i--) {
    Background background(screenWidth, screenHeight, i);
    _backgrounds.push_front(background);
  }
}

bool BackgroundManager::update(unsigned int ticks) {
  for (Background& background : _backgrounds) {
    background.update(ticks);
  }

  return true;
}

void BackgroundManager::render() {
  for (Background background : _backgrounds) {
    background.render();
  }
}
