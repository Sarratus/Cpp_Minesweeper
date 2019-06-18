#pragma once

#include <SDL.h>


using namespace std;

//////////// œ≈–≈Ã≈ÕÕ€≈ //////////////////////////////////

const unsigned short int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;

extern SDL_Renderer* renderer;


//////////// ‘”Õ ÷»» /////////////////////////////////////

void Init();
void Close();

SDL_Texture* Image_Load(const char*);

int Generate_Random_Int(int, int);