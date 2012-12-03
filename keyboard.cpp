// C++ ALLEGRO 5 MADE EASY TUTORIAL 9 - KEYBOARD INPUT
// CODINGMADEEASY

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define ScreenWidth 800
#define ScreenHeight 600

int main(int argc, char *argv[]) {
  ALLEGRO_DISPLAY *display;

  const float FPS = 60.0;

  enum Direction { UP, DOWN, LEFT, RIGHT };

  if (!al_init()) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not Initialize Allegro", NULL, NULL);
  }

  display = al_create_display(ScreenWidth, ScreenHeight);

  if (!display) {
    al_show_native_message_box(NULL, "Error", NULL, "Could not create Allegro Display", NULL, NULL);
  }

  al_set_window_position(display, 200, 200);

  bool done = false, draw = true;
  int x = ScreenWidth / 2, y = ScreenHeight - 20, moveSpeed = 5;
  int dir = DOWN;

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
      if(al_key_down(&keyState, ALLEGRO_KEY_DOWN) && y < ScreenHeight - 20) {
        y += moveSpeed;
      } else if(al_key_down(&keyState, ALLEGRO_KEY_UP) && y > 0) {
        y -= moveSpeed;
      }

      if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && x < ScreenWidth - 10) {
        x += moveSpeed;
      } else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT) && x > 10) {
        x -= moveSpeed;
      }

        draw = true;
    }

    if (draw) {
      draw = false;
      al_draw_filled_triangle(x - 10, y + 20, x, y, x + 10, y + 20, al_map_rgb(255, 0, 0));
      al_flip_display();
      al_clear_to_color(al_map_rgb(0, 0, 0));
    }
  }
  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(event_queue);

  return 0;
}

class Point {
  int _x;
  int _y;

public:
  Point(int x, int y) { _x = x; _y = y; }

  int getX() { return _x; }
  void setX(int x) { _x = x; }
  int getY() { return _y; }
  void setY(int y) { _y = y; }
};

class Bullet {
  Point *position;

public:
  Bullet(int x, int y);
  ~Bullet();
  int move();
  void render(/* camera *camera */);
};

Bullet::Bullet(int x, int y) {
  position = new Point(x, y);
}

Bullet::~Bullet() {
  delete position;
}

int Bullet::move() {
  position->setY(position->getY() - 4);

  return position->getY();
}
