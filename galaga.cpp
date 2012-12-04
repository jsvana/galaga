#include "galaga.h"

Galaga::Galaga(int screenWidth, int screenHeight, ALLEGRO_EVENT_QUEUE *eventQueue) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _eventQueue = eventQueue;
  _ship.moveTo(_screenWidth / 2, _screenHeight - 20);
}

bool Galaga::update(unsigned int ticks) {
  ALLEGRO_EVENT events;
  al_wait_for_event(_eventQueue, &events);
  Point shipPosition;

  _needsDraw = false;

  if (events.type == ALLEGRO_EVENT_KEY_UP && events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
    return false;
  }

  if (events.type == ALLEGRO_EVENT_KEY_DOWN && events.keyboard.keycode == ALLEGRO_KEY_SPACE) {
    _ship.fire();
    std::cout << "[LOG] fire" << std::endl;
  }

  if (events.type == ALLEGRO_EVENT_TIMER) {
    al_get_keyboard_state(&_keyState);
    shipPosition = _ship.getPosition();

    if(al_key_down(&_keyState, ALLEGRO_KEY_LEFT) && shipPosition.getX() > 10) {
      _ship.move(GALAGA_LEFT, MOVE_SPEED);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_RIGHT) && shipPosition.getX() < _screenWidth - 10) {
      _ship.move(GALAGA_RIGHT, MOVE_SPEED);
    }

    if(al_key_down(&_keyState, ALLEGRO_KEY_UP) && shipPosition.getY() > 0) {
      _ship.move(GALAGA_UP, MOVE_SPEED);
    } else if(al_key_down(&_keyState, ALLEGRO_KEY_DOWN) && shipPosition.getY() < _screenHeight - 20) {
      _ship.move(GALAGA_DOWN, MOVE_SPEED);
    }

    _needsDraw = true;
  }

  _ship.update(ticks);

  return true;
}

void Galaga::render() {
  if (_needsDraw) {
    _ship.render();

    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
  }
}
