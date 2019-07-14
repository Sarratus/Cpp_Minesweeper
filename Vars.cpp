#include "Vars.h"
#include "Classes.h"

unsigned short int SCREEN_WIDTH = 1280;
unsigned short int SCREEN_HEIGHT = 720;

int sound_volume = 100;

bool lose = false;
bool quit = false;
bool restart = false;
bool start = false;

bool keyboard = true;

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;

mutex main_render;
mutex shaded_render;

Text_render* numbers = new Text_render[5];
Text_render* black_numbers = new Text_render[10];
Text_render* alphabet = new Text_render[79];