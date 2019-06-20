#include "Vars.h"
#include "Classes.h"

unsigned short int SCREEN_WIDTH = 1280;
unsigned short int SCREEN_HEIGHT = 720;

bool lose = false;
bool quit = false;
bool restart = false;

SDL_Renderer* renderer = nullptr;

mutex main_render;
mutex shaded_render;

Text_render* numbers = new Text_render[8];