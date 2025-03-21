#ifndef GFX_H
#define GFX_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define BACKGROUND_TINT 255

#define WIDTH 1280
#define HEIGHT 720

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

void draw_rect(App* s, float x, float y, float h, float w, uint8_t r = 0,
               uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
void draw_vector(App* s, int x, int y, int dx, int dy);
void draw_circle(App* s, float x, float y, float radius, uint8_t r = 255,
               uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

#endif // GFX_H
