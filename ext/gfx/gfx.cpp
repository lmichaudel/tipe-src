#include "gfx.hpp"
#include <SDL_timer.h>

App* create_application() {
  App* s = new App;
  SDL_Init(SDL_INIT_VIDEO);

  s->window = SDL_CreateWindow("TIPE", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(s->renderer, SDL_BLENDMODE_BLEND);
  s->running = false;
  s->tick = 0;

  return s;
}

void run(App* s, void draw(App*)) {
  s->running = true;

  SDL_SetRenderDrawColor(s->renderer, BACKGROUND_TINT, BACKGROUND_TINT,
                         BACKGROUND_TINT, 0);
  SDL_RenderClear(s->renderer);

  draw(s);

  SDL_RenderPresent(s->renderer);

  while (s->running) {
    while (SDL_PollEvent(&s->event)) {
      switch (s->event.type) {
      case SDL_QUIT:
        close(s);
        break;
      default:
        break;
      }
    }

    SDL_Delay(100);
  }

  SDL_DestroyWindow(s->window);
  SDL_DestroyRenderer(s->renderer);
  SDL_Quit();
  delete s;
}

void close(App* s) { s->running = false; }

void draw_rect(App* s, float x, float y, float h, float w, uint8_t r, uint8_t g,
               uint8_t b, uint8_t a) {

  s->rect.x = x;
  s->rect.y = y;
  s->rect.h = h;
  s->rect.w = w;

  SDL_SetRenderDrawColor(s->renderer, r, g, b, a);
  SDL_RenderDrawRectF(s->renderer, &s->rect);
}

void draw_vector(App* s, int x, int y, int dx, int dy) {
  SDL_SetRenderDrawColor(s->renderer, 255, 0, 0, 255);
  // Calcul de la position de la tête de la flèche
  int endX = x + dx;
  int endY = y + dy;

  // Dessiner la ligne principale de la flèche
  SDL_RenderDrawLine(s->renderer, x, y, endX, endY);

  // Taille de la pointe de la flèche
  constexpr float arrowHeadLength = 5.0f;
  constexpr float arrowHeadAngle = 3.14f / 6.0f; // 30 degrés

  // Calculer les points de la pointe de la flèche
  float angle = atan2(dy, dx);
  int arrowPoint1X =
      endX - static_cast<int>(arrowHeadLength * cos(angle + arrowHeadAngle));
  int arrowPoint1Y =
      endY - static_cast<int>(arrowHeadLength * sin(angle + arrowHeadAngle));
  int arrowPoint2X =
      endX - static_cast<int>(arrowHeadLength * cos(angle - arrowHeadAngle));
  int arrowPoint2Y =
      endY - static_cast<int>(arrowHeadLength * sin(angle - arrowHeadAngle));

  // Dessiner les deux lignes de la pointe de la flèche
  SDL_RenderDrawLine(s->renderer, endX, endY, arrowPoint1X, arrowPoint1Y);
  SDL_RenderDrawLine(s->renderer, endX, endY, arrowPoint2X, arrowPoint2Y);
}

void draw_circle(App* s, float x, float y, float radius, uint8_t r, uint8_t g,
                 uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(s->renderer, r, g, b, a);

  int status = 0;
  int offsetx = 0;
  int offsety = radius;
  int d = radius - 1;

  while (offsety >= offsetx) {

    status += SDL_RenderDrawLine(s->renderer, x - offsety, y + offsetx,
                                 x + offsety, y + offsetx);
    status += SDL_RenderDrawLine(s->renderer, x - offsetx, y + offsety,
                                 x + offsetx, y + offsety);
    status += SDL_RenderDrawLine(s->renderer, x - offsetx, y - offsety,
                                 x + offsetx, y - offsety);
    status += SDL_RenderDrawLine(s->renderer, x - offsety, y - offsetx,
                                 x + offsety, y - offsetx);

    if (status < 0) {
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }
}