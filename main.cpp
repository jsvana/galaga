#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "asset_manager.h"
#include "config_manager.h"

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

  const float FPS = 60.0;

  if (!al_init()) {
    std::cerr << "[ERROR] Error initializing Allegro" << std::endl;
  }

  display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!display) {
    std::cerr << "[ERROR] Error creating Allegro display" << std::endl;
  }

  al_install_audio();
  al_install_keyboard();

  al_init_primitives_addon();
  al_init_acodec_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();

  al_reserve_samples(10);

  if (!AssetManager::initialize()) {
    std::cerr << "[ERROR] Error initializing assets" << std::endl;
  }

  if (!ConfigManager::initialize()) {
    std::cerr << "[ERROR] Error initializing config" << std::endl;
  }

  srand(time(NULL));

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
  al_register_event_source(eventQueue, al_get_keyboard_event_source());
  al_register_event_source(eventQueue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  GameModule *game = new Galaga(SCREEN_WIDTH, SCREEN_HEIGHT, eventQueue);

  while (game->update(ticks)) {
    game->render();

    ++ticks;
  }

  delete game;

  AssetManager::destroy();
  ConfigManager::destroy();

  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(eventQueue);

  return 0;
}
