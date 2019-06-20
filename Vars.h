#pragma once

#include <SDL.h>
#include <mutex>

using namespace std;

//////////// œ≈–≈Ã≈ÕÕ€≈ //////////////////////////////////

extern unsigned short int SCREEN_WIDTH, SCREEN_HEIGHT;

const int SCREEN_FPS = 30; 
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

extern bool lose;
extern bool quit;
extern bool restart;

extern SDL_Renderer* renderer;

extern mutex main_render;
extern mutex shaded_render;

//////////// ‘”Õ ÷»» /////////////////////////////////////

void Init();
void Close();

SDL_Texture* Image_Load(const char*);

int Generate_Random_Int(int, int);