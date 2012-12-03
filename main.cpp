// C++ ALLEGRO 5 MADE EASY TUTORIAL 9 - KEYBOARD INPUT
// CODINGMADEEASY

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include "bullet.h"
#include "ship.h"
#include "utilities.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char *argv[]) {
  ALLEGRO_DISPLAY *display;

  const float FPS = 60.0;

  if (!al_init()) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not Initialize Allegro", NULL, NULL);
  }

  display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!display) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not create Allegro Display", NULL, NULL);
  }

  al_set_window_position(display, 200, 200);

  bool done = false, draw = true;
  int moveSpeed = 5;
  Point shipPosition;

  Ship *ship = new Ship(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);

  al_init_primitives_addon();
  al_install_keyboard();

  ALLEGRO_KEYBOARD_STATE keyState;

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  while (!done) {
    ALLEGRO_EVENT events;
    al_wait_for_event(event_queue, &events);

    if (events.type == ALLEGRO_EVENT_KEY_UP) {
      switch(events.keyboard.keycode)
      {
      case ALLEGRO_KEY_ESCAPE:
        done = true;
      }
    }

    if (events.type == ALLEGRO_EVENT_TIMER) {
      al_get_keyboard_state(&keyState);
      shipPosition = ship->getPosition();

      if(al_key_down(&keyState, ALLEGRO_KEY_LEFT) && shipPosition.getX() > 10) {
        ship->move(GALAGA_LEFT, moveSpeed);
      } else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && shipPosition.getX() < SCREEN_WIDTH - 10) {
        ship->move(GALAGA_RIGHT, moveSpeed);
      }

      if(al_key_down(&keyState, ALLEGRO_KEY_UP) && shipPosition.getY() > 0) {
        ship->move(GALAGA_UP, moveSpeed);
      } else if(al_key_down(&keyState, ALLEGRO_KEY_DOWN) && shipPosition.getY() < SCREEN_HEIGHT - 20) {
        ship->move(GALAGA_DOWN, moveSpeed);
      }

      draw = true;
    }

    if (draw) {
      draw = false;
      ship->render();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
    }
  }

  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(event_queue);

  delete ship;

  return 0;
}
