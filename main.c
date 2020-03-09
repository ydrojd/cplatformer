#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define W_HEIGHT 600
#define W_WIDTH 800
#define W_TITLE "Game"
#define BUFF_STARTSIZE 10
#define BUFF_GR 2
#define STRING_SIZE 100
#define IMAGE_PATH "img/"
#define MAX_IMAGES 200

enum mob_name {
               PLAYER,
               TURTLE
};

struct mob_entity {
  float x, y, w, h;
  enum mob_name name;
  bool circle_box;
  float speed;
  int vertical_dir;
  bool alive;
  SDL_Texture *tex;
  SDL_Rect srect;
};

struct game_state {
  struct mob_entity *mob;
  int mob_n, mob_blocks;
};

int loop(SDL_Renderer *renderer);
int default_scene(struct game_state *state, SDL_Texture **tex);
int load_img(SDL_Renderer *renderer, SDL_Texture **tex);

int main(void)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(W_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            W_WIDTH, W_HEIGHT, 0);
  if(!window) {
    printf("Window failed");
  }

  renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
  if(!renderer) {
    printf("Renderer failed");
  }

  loop(renderer);

  return 0;
}

int loop(SDL_Renderer *renderer)
{
  const unsigned char *key_state = SDL_GetKeyboardState(NULL);
  struct game_state state;
  float dt;
  Uint64 new_time, old_time;
  SDL_Texture *text_space[MAX_IMAGES];

  new_time = SDL_GetPerformanceCounter();
  dt = 0.016;

  load_img(renderer, text_space);
  default_scene(&state, text_space);

  while(1) {
    //input
    {
      SDL_Event event;

      while(SDL_PollEvent(&event));

      if(key_state[SDL_GetScancodeFromName("Q")]){
        break;
      }

      //player
      if(key_state[SDL_GetScancodeFromName("A")]){
        state.mob[0].vertical_dir = -1;
      } else if(key_state[SDL_GetScancodeFromName("D")]){
        state.mob[0].vertical_dir = 1;
      } else {

        state.mob[0].vertical_dir = 0;
      }
    }

    //mob loop
    {
      for(int i = 0; i < state.mob_n; i++){
        state.mob[i].x += state.mob[i].vertical_dir * state.mob[i].speed * dt;
      }
    }

    //render
    {
      SDL_Rect drect;

      SDL_RenderClear(renderer);

      for(int i = 0; i < state.mob_n; i++){
        drect.x = state.mob[i].x;
        drect.y = state.mob[i].y;
        drect.h = state.mob[i].h;
        drect.w = state.mob[i].w;

        SDL_RenderCopy(renderer, state.mob[i].tex, &state.mob[i].srect, &drect);
      }

      SDL_RenderPresent(renderer);
    }

    //calculate deltatime
    {
      old_time = new_time;
      new_time = SDL_GetPerformanceCounter();
      dt = (float) (new_time - old_time) * 1000;
      dt = dt / (float) SDL_GetPerformanceFrequency();
    }

  }

  printf("%f", state.mob[0].x);
  return 0;
}

int default_scene(struct game_state *state, SDL_Texture **tex)
{
  static struct mob_entity player;

  player.x = 400;
  player.y = 300;
  player.w = 22;
  player.h = 25;
  player.alive = true;
  player.circle_box = false;
  player.name = PLAYER;
  player.speed = 1;
  player.vertical_dir = 0;
  player.tex = tex[0];
  player.srect.x = 0;
  player.srect.y = 0;
  player.srect.w = 22;
  player.srect.h = 25;

  state->mob_n = 1;
  state->mob_blocks = BUFF_STARTSIZE;
  state->mob = (struct mob_entity *) malloc(sizeof(struct mob_entity) * BUFF_STARTSIZE);
  state->mob[0] = player;
  state->mob[0].w = 22;

  return 0;
}


int load_img(SDL_Renderer *renderer, SDL_Texture **tex){
  SDL_Surface *surf;

  surf = IMG_Load("img/player.png");
  tex[0] = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
}
