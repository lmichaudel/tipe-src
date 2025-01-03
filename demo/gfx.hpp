#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct App {
  SDL_Renderer* renderer;
  SDL_Window* window;
  SDL_Event event;

  bool running;
  Uint32 tick;

  SDL_FRect rect;
} App;

App* create_application();
void run(App* s, void draw(App*));
void close(App* s);

void draw_rect(App* s, float x, float y, float h, float w, int r = 0,
               int g = 255, int b = 0, int a = 255);
void draw_vector(App* s, float x, float y, float dx, float dy);
void draw_circle(App* s, int x, int y, int radius);

#endif // GFX_H
