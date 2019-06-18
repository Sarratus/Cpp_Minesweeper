#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>

using json = nlohmann::json;

Playing_field::Playing_field() {

	ifstream settings_file;

	settings_file.open("settings.json");
	if (!settings_file.is_open())
		cout << endl << "Error to open file \"settings.json \"";

	auto settings = json::parse(settings_file);
	settings_file.close();

	width = settings.at("Width_of_playing_field").get<int>();
	height = settings.at("Height_of_playing_field").get<int>();	
	number_of_mines = settings.at("Number_of_mines").get<int>();
	
	textures = Image_Load("assets\\textures.png");
	shaded_textures = Image_Load("assets\\textures.png");

	if (textures == nullptr || shaded_textures == nullptr)
		cout << endl << "Texture load error: " << SDL_GetError();

	if (SCREEN_HEIGHT * 0.2 / height <= SCREEN_WIDTH / width) {
		y = SCREEN_HEIGHT * 0.2;
		cell_size = SCREEN_HEIGHT * 0.75 / height;
		x = (-cell_size * width / 2) + (SCREEN_WIDTH / 2);
	}
	else {
		x = SCREEN_WIDTH * 0.05;
		cell_size = SCREEN_WIDTH * 0.9 / width;
		y = SCREEN_HEIGHT * 0.2 - cell_size * height + SCREEN_HEIGHT - cell_size * height / 2 + SCREEN_HEIGHT / 2;
	}

	mines_and_numbers = new int* [width];
	open_cells = new bool* [width];
	player_interaction = new bool* [width];

	for (int i = 0; i < width; i++) {
		*(mines_and_numbers + i) = new int[height];
		*(open_cells + i) = new bool[height];
		*(player_interaction + i) = new bool[height];
	}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			open_cells[j][i] = false;
			player_interaction[j][i] = false;
			mines_and_numbers[j][i] = 0;
		}

	int temp = number_of_mines;
	//cout << endl << "\tmines on:";
	while (temp != 0)
		for (auto i = 0; i < height; i++)
			for (auto j = 0; j < width; j++) {
				if (Generate_Random_Int(0, 200) <= ((number_of_mines * 100 * 1.92) / (width * height)) && mines_and_numbers[j][i] != -1) {
					mines_and_numbers[j][i] = -1;
					temp -= 1;
					//cout << endl << i << " and " << j;
				}
				if (temp == 0)
					goto EXIT;
			}	
	EXIT:
	
	int arrayLOL[8][2] = { {-1,-1},	{0,-1},	{1,-1},		{-1,0}, {1,0},		 {-1,1},		{0,1},			{1,1} };
	int arrayKEK[8][2] = { {0,0},	{-2,0}, {width-1,0},	{0,-2}, {width-1,-2},	 {0,height-1},	{-2,height-1},	{width-1,height-1} };

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) 
			if (mines_and_numbers[j][i] == -1)
				for (int k = 0; k < 8; k++)
					if (j != arrayKEK[k][0] && i != arrayKEK[k][1]) {
						//cout << endl << k+1 << ":" << endl << j << " and " << i << endl << arrayKEK[k][0] << " and " << arrayKEK[k][1] << endl;;
						if (mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] != -1)
							mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] += 1;
					}
}

Playing_field::~Playing_field() {
	delete mines_and_numbers;
	delete open_cells;
	delete player_interaction;

	SDL_DestroyTexture(textures);
	SDL_DestroyTexture(shaded_textures);
}

void Playing_field::Show_mines_to_console() {
	cout << endl << "Playing field: " << endl;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (mines_and_numbers[j][i] > -1)
				cout << " ";
			cout << mines_and_numbers[j][i] << "\t";

			if (j == width - 1)
				cout << endl;
		}
	cout << endl << endl;
}

void Playing_field::Field_Render() {
	SDL_RenderClear(renderer);
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
			Cell_Render(j, i);
	SDL_RenderPresent(renderer);
}

void Playing_field::Cell_Render(int pos_x, int pos_y) {
	SDL_Rect dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
	SDL_Rect src;

		if (!open_cells[pos_x][pos_y]) {
			src = { 0, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
			if (player_interaction[pos_x][pos_y]) {
				src = { 40, 0, 20, 20 };
				SDL_RenderCopy(renderer, textures, &src, &dst);
			}
		}
		else {
			src = { 20, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
			//if (mines_and_numbers[pos_x][pos_y] > 0)
				//SDL_RenderCopy(renderer, (numbers + mines_and_numbers[pos_x][pos_y])->texture, &src, &dst);
		}
		/*if (mines_and_numbers[pos_x][pos_y] == -1) {
			src = { 60, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}*/
}


////////////////////////////////////////// TEXT_RENDER //////////////////////////////////////////////////


void Text_render::Init(string str, SDL_Color text_color, int size) {
	TTF_Font* Sans = nullptr;
	original_text = str;
	color = text_color;
	height = size;
	width = size * str.length();

	Sans = TTF_OpenFont("assets\\Game_Sans.ttf", 26);

	if (Sans == nullptr)
		cout << endl << "Sans load error: " << SDL_GetError();

	SDL_Surface* message_surface = nullptr;
	message_surface = TTF_RenderText_Blended(Sans, str.c_str(), text_color);

	if (message_surface == nullptr)
		cout << endl << "Text surface creating error: " << SDL_GetError();

	texture = SDL_CreateTextureFromSurface(renderer, message_surface);
	if (texture == nullptr)
		cout << endl << "Text texture creating error: " << SDL_GetError();

	SDL_FreeSurface(message_surface);
	TTF_CloseFont(Sans);
}

Text_render::~Text_render() {
	SDL_DestroyTexture(texture);
}