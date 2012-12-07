#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "bullet.h"
#include "galaga.h"
#include "gamemodule.h"
#include "ship.h"
#include "utilities.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 700

int main(int argc, char *argv[]) {
  ALLEGRO_DISPLAY *display;
  unsigned int ticks = 0;

  srand(time(NULL));

  const float FPS = 60.0;

  if (!al_init()) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not Initialize Allegro", NULL, NULL);
  }

  display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!display) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not create Allegro Display", NULL, NULL);
  }

  al_install_audio();

  al_init_primitives_addon();
  al_init_acodec_addon();
  if (!al_reserve_samples(10)) {
    std::cout << "[ERROR] Unable to reserve sound samples, quitting." << std::endl;
  }
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();
  al_install_keyboard();

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
  al_register_event_source(eventQueue, al_get_keyboard_event_source());
  al_register_event_source(eventQueue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  GameModule *game = new Galaga(SCREEN_WIDTH, SCREEN_HEIGHT, eventQueue);
  bool gameRunning = true;

  while (gameRunning) {
    gameRunning = game->update(ticks);

    game->render();

    ++ticks;
  }

  delete game;

  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(eventQueue);

  return 0;
}
