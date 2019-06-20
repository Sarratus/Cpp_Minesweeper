#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>

using json = nlohmann::json;



Playing_field::Playing_field() {

	ifstream settings_file;

	// Загрузка настроек из файла //

	settings_file.open("settings.json");
	if (!settings_file.is_open())
		cout << endl << "Error to open file \"settings.json \"";

	auto settings = json::parse(settings_file);
	settings_file.close();

	// Загрузка в оперативную память изображений из файлов //

	width = settings.at("widthOfPlayingField").get<int>();
	height = settings.at("heightOfPlayingField").get<int>();	
	number_of_mines = settings.at("numberOfMines").get<int>();
	
	textures = Image_Load("assets\\textures.png");
	shaded_textures = Image_Load("assets\\textures.png");

	if (textures == nullptr || shaded_textures == nullptr)
		cout << endl << "Texture load error: " << SDL_GetError();

	// Подгон UI под размер окна

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

	// Инициализация массивов //

	mines_and_numbers = new short int* [width];
	open_cells = new bool* [width];
	player_interaction = new bool* [width];

	for (int i = 0; i < width; i++) {
		*(mines_and_numbers + i) = new short int[height];
		*(open_cells + i) = new bool[height];
		*(player_interaction + i) = new bool[height];
	}

	// Инициализация массивов "нулями" //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			open_cells[j][i] = false;
			player_interaction[j][i] = false;
			mines_and_numbers[j][i] = 0;
		}

	int temp = number_of_mines;
	int temp2 = 0;

	// Генератор минного поля //

	//cout << endl << "\tmines on:";
	while (temp != 0)
		for (auto i = 0; i < height; i++) {
			temp2 = 0;

			for (auto j = 0; j < width; j++) {
				if (Generate_Random_Int(0, 200) <= ((number_of_mines * 100 * 1.92) / (width * height)) && mines_and_numbers[j][i] != -1) {
					if (temp2 > number_of_mines / height * 1.6)
						break;
					mines_and_numbers[j][i] = -1;
					temp -= 1;
					temp2 += 1;
					//cout << endl << i << " and " << j;
				}
				if (temp == 0)
					goto EXIT;
			}
		}
	EXIT:

	// Расстановка количества окружающих клетку бомб //

	int arrayLOL[8][2] = { {-1,-1},	{0,-1},	{1,-1},		{-1,0}, {1,0},		 {-1,1},		{0,1},			{1,1} };
	int arrayKEK[8][2] = { {0,0},	{-2,0}, {width-1,0},	{0,-2}, {width-1,-2},	 {0,height-1},	{-2,height-1},	{width-1,height-1} };

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) 
			if (mines_and_numbers[j][i] == -1)
				for (int k = 0; k < 8; k++)
					if (j != arrayKEK[k][0] && i != arrayKEK[k][1]) {
						//cout << endl << k+1 << ":" << endl << j << " and " << i << endl << arrayKEK[k][0] << " and " << arrayKEK[k][1] << endl;
						if (mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] != -1)
							mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] += 1;
					}

	// Единоразовое открытие первой попавшейся клетки с нулевым количеством окружающих бомб //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mines_and_numbers[j][i] == 0) {
				Cell_Opening(j, i);
				goto EXIT2;
			}
	EXIT2:
	;
}

Playing_field::~Playing_field() {
	
	// Открытие всех клеток поля для показа местоположения бомб //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (open_cells[j][i] == false)
				open_cells[j][i] = true;

	Field_Render(false);	

	LTimer capTimer, fpsTimer;
	int countedFrames = 0;	

	fpsTimer.start();

	SDL_Event e;

	while (!quit && !restart) {
		capTimer.start();

		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}		

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}	
			if (e.key.keysym.sym == SDLK_r) {
				restart = true;
			}
		}

		++countedFrames;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	capTimer.stop();
	fpsTimer.stop();

	// Очистка памяти //
	
	delete mines_and_numbers;
	delete open_cells;
	delete player_interaction;

	SDL_DestroyTexture(textures);
	SDL_DestroyTexture(shaded_textures);
}

void Playing_field::Window_update() {
	if (SCREEN_HEIGHT * 0.75 / height <= SCREEN_WIDTH * 0.9 / width) {
		//cout << endl << "y is smaller";
		
		y = SCREEN_HEIGHT * 0.2;
		cell_size = SCREEN_HEIGHT * 0.75 / height;
		x = (-cell_size * width / 2) + (SCREEN_WIDTH / 2);
	}
	else {
		//cout << endl << "x is smaller";
		
		x = SCREEN_WIDTH * 0.05;
		cell_size = SCREEN_WIDTH * 0.9 / width;
		y = (SCREEN_HEIGHT * 0.2) - (cell_size * height / 2) + (SCREEN_HEIGHT * 0.75 / 2);
	}
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

void Playing_field::Field_Render(bool render_numbers) {
	main_render.lock();	

	SDL_RenderClear(renderer);
	
	// Рендер каждой ячейки поля //

	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
			Cell_Render(j, i, render_numbers);

	SDL_Rect src = { 3, 0, 14, 20 };
	SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 0.15 };
	
	SDL_RenderCopy(renderer, textures, &src, &dst);
	
	SDL_RenderPresent(renderer);

	main_render.unlock();
}

void Playing_field::Cell_Render(int pos_x, int pos_y, bool render_numbers) {
	SDL_Rect dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
	SDL_Rect src;

		if (!open_cells[pos_x][pos_y]) {
			src = { 0, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);

			if (player_interaction[pos_x][pos_y]) {
				src = { 40, 0, 20, 20 };
				SDL_RenderCopy(renderer, textures, &src, &dst);
			}
		} else {
			src = { 20, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);		

			if (mines_and_numbers[pos_x][pos_y] > 0 && render_numbers) {
				dst = { x + pos_x * cell_size + int(cell_size * 0.22), y + pos_y * cell_size + int(cell_size * 0.23), int(cell_size * 0.625), int(cell_size * 0.625) };
				SDL_RenderCopy(renderer, (numbers + mines_and_numbers[pos_x][pos_y])->texture, NULL, &dst);				
			}	

			if (mines_and_numbers[pos_x][pos_y] == -1) {
				src = { 60, 0, 20, 20 };
				dst = { x + pos_x * cell_size + int(cell_size * 0.22), y + pos_y * cell_size + int(cell_size * 0.23), int(cell_size * 0.625), int(cell_size * 0.625) };
				SDL_RenderCopy(renderer, textures, &src, &dst);
			}
		} 		
		/*if (mines_and_numbers[pos_x][pos_y] == -1) {
			src = { 60, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}*/
}

void Playing_field::Opening_Animation(int x_pos, int y_pos) {
	LTimer controller;
	controller.start();

	float opening_speed = 255 / 500;

	SDL_Rect dst = { x+x_pos*cell_size, y+y_pos*cell_size, cell_size, cell_size };
	SDL_Rect src = { 0, 0, 20, 20 };

	while (int(opening_speed * controller.getTicks()) <= 255) {
		shaded_render.lock();
		
		SDL_SetTextureAlphaMod(shaded_textures, int(opening_speed * controller.getTicks()));
		SDL_RenderCopy(renderer, shaded_textures, &src, &dst);

		shaded_render.unlock();
	}

	controller.stop();
	SDL_SetTextureAlphaMod(shaded_textures, 255);
}

void Playing_field::Cell_Opening(int x_pos, int y_pos) {
	
	// Клик на мину => проигрыш //
	
	if (mines_and_numbers[x_pos][y_pos] == -1) {
		lose = true;
		return;
	}
	
	// Открытие ячейки и проигрывание анимации //

	if (open_cells[x_pos][y_pos] == false) {
		open_cells[x_pos][y_pos] =  true;
		//Opening_Animation(x_pos, y_pos);
	}	

	// Открытие прилегающий клеток, если рядом с текущей клеткой нет бомб //

	if (mines_and_numbers[x_pos][y_pos] == 0) {
		
		int arrayLOL[8][2] = { {-1,-1},	{0,-1},	{1,-1},		{-1,0}, {1,0},		 {-1,1},		{0,1},			{1,1} };
		int arrayKEK[8][2] = { {0,0},	{-2,0}, {width - 1,0},	{0,-2}, {width - 1,-2},	 {0,height - 1},	{-2,height - 1},	{width - 1,height - 1} };

		for (int k = 0; k < 8; k++)
			if (x_pos != arrayKEK[k][0] && y_pos != arrayKEK[k][1])
				if (open_cells[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]] == false)
					if (mines_and_numbers[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]]  != -1)
						Cell_Opening(x_pos + arrayLOL[k][0], y_pos + arrayLOL[k][1]);
	}				
}

void Playing_field::Flag_setter(int x_pos, int y_pos) {
	if (player_interaction[x_pos][y_pos] == false) {
		player_interaction[x_pos][y_pos] = true;
	} else {
		player_interaction[x_pos][y_pos] = false;
	}	
}

void Playing_field::Left_Click() {
	// Получение координат мыши //
	
	int x_pos = 0, y_pos = 0;
	SDL_GetMouseState(&x_pos, &y_pos);
	
	// Проверка координат курсора на наождение над минным полем //

	if (x_pos - x >= 0 && x_pos - x <= cell_size * width)
		if (y_pos - y >= 0 && y_pos - y <= cell_size * height) {
			x_pos = (x_pos - x) / cell_size;
			y_pos = (y_pos - y) / cell_size;
			
			if (player_interaction[x_pos][y_pos] == false)
				Cell_Opening(x_pos, y_pos);
		}
}

void Playing_field::Right_Click() {
	// Получение координат мыши //

	int x_pos = 0, y_pos = 0;
	SDL_GetMouseState(&x_pos, &y_pos);

	// Проверка координат курсора на наождение над минным полем //

	if (x_pos - x >= 0 && x_pos - x <= cell_size * width)
		if (y_pos - y >= 0 && y_pos - y <= cell_size * height) {
			x_pos = (x_pos - x) / cell_size;
			y_pos = (y_pos - y) / cell_size;

			Flag_setter(x_pos, y_pos);
		}
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


///////////////////////////////// LTIMER //////////////////////////////////////////////////////


LTimer::LTimer() { 		
	mStartTicks = 0; 
	mPausedTicks = 0; 
	mPaused = false; 
	mStarted = false; 
}

void LTimer::start() { 
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks(); 
	mPausedTicks = 0; 
}

void LTimer::stop() { 
	mStarted = false; 
	mPaused = false; 
	mStartTicks = 0; 
	mPausedTicks = 0; 
}

void LTimer::pause() { 
	if( mStarted && !mPaused ) {
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks; mStartTicks = 0; 
	} 
}

void LTimer::unpause() { 
	if( mStarted && mPaused ) { 
		mPaused = false; 
		mStartTicks = SDL_GetTicks() - mPausedTicks; 
		mPausedTicks = 0; 
	} 
}

Uint32 LTimer::getTicks() { 
	Uint32 time = 0; 
	if( mStarted ) { 
		if( mPaused ) { 
			time = mPausedTicks; 
		} else { 
			time = SDL_GetTicks() - mStartTicks; 
		} 
	} 
	return time; 
}

bool LTimer::isStarted() { 
	return mStarted; 
} 

bool LTimer::isPaused() { 
	return mPaused && mStarted; 
}