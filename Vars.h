#pragma once

#include <SDL.h>
#include <mutex>

using namespace std;

//////////// œ≈–≈Ã≈ÕÕ€≈ //////////////////////////////////

extern unsigned short int SCREEN_WIDTH, SCREEN_HEIGHT;

const int SCREEN_FPS = 20; 
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int CONTROLLER_DEAD_ZONE = 8000;

extern bool lose;
extern bool quit;
extern bool restart;
extern bool start;

extern bool keyboard;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

extern mutex main_render;
extern mutex shaded_render;

//////////// ‘”Õ ÷»» /////////////////////////////////////

void Init();
void Close();

SDL_Texture* Image_Load(const char*);

int Generate_Random_Int(int, int);

void Window_Fullscreen();

void Numbers_Renderer(int, SDL_Rect*);
void Letter_Renderer(string str, SDL_Rect* dst);
