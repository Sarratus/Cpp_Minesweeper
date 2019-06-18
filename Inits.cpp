#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Vars.h"
#include "Classes.h"

SDL_Window* window = nullptr;

void Init() {
	cout << "Init start..." << endl;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		std::cout << std::endl << "SDL initialization failed. SDL Error: " << SDL_GetError();
	if (!IMG_Init(IMG_INIT_PNG))
		cout << "PNG load error : " << SDL_GetError();
	if (TTF_Init() < 0)
		cout << "SDL_ttf load error : " << SDL_GetError();

	window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == nullptr)
		std::cout << std::endl << "Window initialization failed. SDL Error: " << SDL_GetError();

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr)
		std::cout << std::endl << "Renderer initialization failed. SDL Error: " << SDL_GetError();
	
	/*for (auto i = 0; i < 8; i++) {		
		SDL_Color temp[8] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
		(numbers + i)->Init(to_string(i + 1), *(temp + i), 20);
	}*/

}

void Close() {

	for (auto i = 0; i < 8; i++)
		(numbers + i)->~Text_render();
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	cout << endl << endl << "Program was close" << endl;
}