#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SDL_ttf.h>
#include "Vars.h"
#include "Classes.h"

using json = nlohmann::json;

void Init() {
	cout << "Init start..." << endl;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		std::cout << std::endl << "SDL initialization failed. SDL Error: " << SDL_GetError();
	if (!IMG_Init(IMG_INIT_PNG))
		cout << "PNG load error : " << SDL_GetError();
	if (TTF_Init() < 0)
		cout << "SDL_ttf load error : " << SDL_GetError();

	window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(window == nullptr)
		std::cout << std::endl << "Window initialization failed. SDL Error: " << SDL_GetError();

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr)
		std::cout << std::endl << "Renderer initialization failed. SDL Error: " << SDL_GetError();
	
	SDL_SetRenderDrawColor(renderer, 180, 180, 183, 255);
	
	SDL_Color temp[5] = { {40, 40, 45}, {50, 80, 220}, {50, 160, 60}, {175, 40, 40}, {10, 20, 90} };

	for (auto i = 0; i < 10; i++) {					
		if (i < 5)
			(numbers + i)->Init(to_string(i), *(temp + i), 50);
		
		(black_numbers + i)->Init(to_string(i), *(temp + 0), 50);
	}

	char ch;
	string st;
	
	for (auto i = 0; i < 79; i++) {
		ch = '0' + char(i);
		st = ch;

		alphabet[i].Init(st, *(temp), 50);
	}

	ifstream settings_file;

	// Загрузка настроек из файла //

	settings_file.open("settings.json");
	if (!settings_file.is_open())
		cout << endl << "Error to open file \"settings.json \"";

	auto settings = json::parse(settings_file);
	settings_file.close();

	// Загрузка в оперативную память данных из файла //

	playing_field_width = settings.at("widthOfPlayingField").get<int>();
	playing_field_height = settings.at("heightOfPlayingField").get<int>();
	number_of_mines_ = settings.at("numberOfMines").get<int>();
}

void Close() {

	arrayLOL.~vector();
	arrayKEK.~vector();
	
	ifstream settings_file_i;

	// Загрузка настроек из файла //

	settings_file_i.open("settings.json");
	if (!settings_file_i.is_open())
		cout << endl << "Error to open file \"settings.json \"";

	auto settings = json::parse(settings_file_i);
	settings_file_i.close();

	// Загрузка в оперативную память данных из файла //
	settings.at("widthOfPlayingField") = playing_field_width;
	settings.at("heightOfPlayingField") = playing_field_height;
	settings.at("numberOfMines") = number_of_mines_;

	ofstream settings_file_o;
	settings_file_o.open("settings.json");
	
	settings_file_o << settings;

	settings_file_o.close();

	for (auto i = 0; i < 5; i++)
		(numbers + i)->~Text_render();

	for (auto i = 0; i < 10; i++)
		(black_numbers + i)->~Text_render();
		
	shaded_render.~shaded_render();
	main_render.~main_render();
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	cout << endl << endl << "Program was close" << endl;

}