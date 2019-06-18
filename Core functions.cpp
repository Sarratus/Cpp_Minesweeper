#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
#include "Vars.h"

int Generate_Random_Int(int from, int to) {
	random_device rdevice;
	mt19937 rng(rdevice());
	uniform_int_distribution<int> range(from, to);
	return range(rdevice);
}

SDL_Texture* Image_Load(const char* image) {
	
		SDL_Surface* load_image = nullptr;
		SDL_Texture* texture = nullptr;

		load_image = IMG_Load(image);
		if (load_image == nullptr) 
			cout << endl << "Error to load image : " << SDL_GetError();

		texture = SDL_CreateTextureFromSurface(renderer, load_image);
		SDL_FreeSurface(load_image);
		
		return texture;	
}