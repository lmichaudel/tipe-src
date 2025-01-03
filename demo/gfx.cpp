#include "gfx.hpp"

App* create_application() {
  App* s = (App*)malloc(sizeof(App));
  SDL_Init(SDL_INIT_VIDEO);

  s->window = SDL_CreateWindow("TIPE", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
  s->running = false;
  s->tick = 0;

  return s;
}

void run(App* s, void draw(App*)) {
  s->running = true;
  while (s->running) {
    while (!SDL_TICKS_PASSED(SDL_GetTicks64(), s->tick + 16)) {
    }

    // const float deltaTime = (SDL_GetTicks64() - s->tick) / 1000.0;
    s->tick = SDL_GetTicks64();

    while (SDL_PollEvent(&s->event)) {
      switch (s->event.type) {
      case SDL_QUIT:
        close(s);
        break;
      default:
        break;
      }
    }

    SDL_SetRenderDrawColor(s->renderer, 255, 255, 255, 255);
    SDL_RenderClear(s->renderer);

    draw(s);

    SDL_RenderPresent(s->renderer);
  }

  SDL_DestroyWindow(s->window);
  SDL_Quit();
  free(s);
}

void close(App* s) { s->running = false; }

void draw_rect(App* s, float x, float y, float h, float w, int r, int g, int b,
               int a) {

  s->rect.x = x;
  s->rect.y = y;
  s->rect.h = h;
  s->rect.w = w;

  SDL_SetRenderDrawColor(s->renderer, r, g, b, a);
  SDL_RenderDrawRectF(s->renderer, &s->rect);
}

void draw_vector(App* s, float x, float y, float dx, float dy) {
  SDL_SetRenderDrawColor(s->renderer, 255, 0, 0, 255);
  // Calcul de la position de la tête de la flèche
  int endX = x + dx;
  int endY = y + dy;

  // Dessiner la ligne principale de la flèche
  SDL_RenderDrawLine(s->renderer, x, y, endX, endY);

  // Taille de la pointe de la flèche
  const float arrowHeadLength = 5.0f;
  const float arrowHeadAngle = 3.14 / 6.0f; // 30 degrés

  // Calculer les points de la pointe de la flèche
  float angle = atan2(dy, dx);
  int arrowPoint1X =
      endX - (int)(arrowHeadLength * cos(angle + arrowHeadAngle));
  int arrowPoint1Y =
      endY - (int)(arrowHeadLength * sin(angle + arrowHeadAngle));
  int arrowPoint2X =
      endX - (int)(arrowHeadLength * cos(angle - arrowHeadAngle));
  int arrowPoint2Y =
      endY - (int)(arrowHeadLength * sin(angle - arrowHeadAngle));

  // Dessiner les deux lignes de la pointe de la flèche
  SDL_RenderDrawLine(s->renderer, endX, endY, arrowPoint1X, arrowPoint1Y);
  SDL_RenderDrawLine(s->renderer, endX, endY, arrowPoint2X, arrowPoint2Y);
}

void draw_circle(App* s, int x, int y, int radius) {
  SDL_SetRenderDrawColor(s->renderer, 255, 0, 0, 255);

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