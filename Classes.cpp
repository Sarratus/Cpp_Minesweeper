#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>


using json = nlohmann::json;

/////////////////////// Menu ///////////////////////////////


Menu::Menu() {
	textures = Image_Load("assets\\textures.png");
	
	SDL_Color text_color = { 40, 40, 45 };
	Text_render minesweeper("Minesweeper", text_color, 50);
	titles.push_back(minesweeper);
	//minesweeper.~Text_render();
	


	item = Main_Menu;
}

Menu::~Menu() {
	for (auto& text_texture : titles)
		text_texture.~Text_render();
}

void Menu::Menu_renderer() {
	
	SDL_RenderClear(renderer);
	
	SDL_Rect src;
	SDL_Rect dst;

	dst = { int(SCREEN_WIDTH * 0.035), int(SCREEN_HEIGHT * 0.945), int(SCREEN_HEIGHT * 0.03 * 6), int(SCREEN_HEIGHT * 0.03) };
	src = { 20, 0, 20, 20 };
	
	//SDL_RenderCopy(renderer, textures, &src, &dst);
	
	Letter_Renderer("by Lis", &dst);	

	switch (item)
	{
	case Main_Menu: {
		Main_Menu_Renderer();				

		break;
	}
	case Play: {
		start = true;

		break;
	}
	case Settings: {
		break;
	}
	case Exit: {
		quit = true;
		this->~Menu();
		break;
	}
	default:
		cout << endl << "Menu error (wrong item)";
		break;
	}

	SDL_RenderPresent(renderer);
}

void Menu::Main_Menu_Renderer() {
	SDL_Rect dst;
	SDL_Rect dst_text;
	SDL_Rect src;

	int x, y;
	SDL_GetMouseState(&x, &y);
	
	

	// ��������� ������� //
	src = { 23, 0, 14, 20 };
	dst = { 0, int(SCREEN_HEIGHT / 4 - SCREEN_WIDTH * 0.8 / 11 / 1.2), SCREEN_WIDTH, int(SCREEN_WIDTH * 0.8 / titles[Window_title].original_text.size() * 1.55) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_WIDTH * 0.1), int(SCREEN_HEIGHT / 4 - SCREEN_WIDTH * 0.8 / 11 / 2), int(SCREEN_WIDTH * 0.8), int(SCREEN_WIDTH * 0.8 / titles[Window_title].original_text.size()) };

	Letter_Renderer("Minesweeper", &dst);
	
	// ��������� ������ //
		// ������ //
			// ������ //
	dst = { int(SCREEN_WIDTH / 3.18), SCREEN_HEIGHT / 2, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 6 };
	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h) { src = { 20, 0, 20, 20 }; over_item = Play; }
	else { src = { 0, 0, 20, 20 }; over_item = nothing;  }

	SDL_RenderCopy(renderer, textures, &src, &dst);

			// ����� //
	dst_text.x = dst.x + int(dst.h * 1.05);
	dst_text.y = dst.y + int(dst.h / 3);
	dst_text.w = dst.w - dst.h * 2;
	dst_text.h = dst.h - int(dst.h / 2);

	Letter_Renderer("Play", &dst_text);

		// ��������� //
			// ������ //
	dst.y += SCREEN_HEIGHT / 6;
	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h) { src = { 20, 0, 20, 20 }; over_item = Settings; }
	else { src = { 0, 0, 20, 20 }; }

	SDL_RenderCopy(renderer, textures, &src, &dst);
		
			// ����� //
	dst_text.x = dst.x + int(dst.h / 1.75);
	dst_text.y = dst.y + int(dst.h / 2.5);
	dst_text.w = dst.w - int(dst.h / 1.9 * 2);
	dst_text.h = dst.h - int(dst.h / 3 * 2);

	Letter_Renderer("Settings", &dst_text);

		// ����� //
	dst.x += dst.w;
	dst.w = dst.h;
	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h) { src = { 20, 0, 20, 20 }; over_item = Exit; }
	else { src = { 0, 0, 20, 20 }; }

	SDL_RenderCopy(renderer, textures, &src, &dst);
	
		// ���� �� ������ ������ //
	src = { 60, 0, 20, 20 };

	SDL_RenderCopy(renderer, textures, &src, &dst);
}

void Menu::Window_update() {
	
	if (SCREEN_HEIGHT / SCREEN_WIDTH <= 2.1 && SCREEN_WIDTH / SCREEN_HEIGHT <= 2.1)
	if (SCREEN_HEIGHT / 3  <= SCREEN_WIDTH / 3) {
		//cout << endl << "y is smaller";

		y = SCREEN_HEIGHT / 3;
		//cell_size = SCREEN_HEIGHT * 0.75 / height;
		//x = (-cell_size * width / 2) + (SCREEN_WIDTH / 2);
	}
	else {
		//cout << endl << "x is smaller";

		x = SCREEN_WIDTH / 3;
		//cell_size = SCREEN_WIDTH * 0.9 / width;
		//y = (SCREEN_HEIGHT * 0.2) - (cell_size * height / 2) + (SCREEN_HEIGHT * 0.75 / 2);
	}
}

void Menu::Gamepad_Control(Uint8 button) {

}

void Menu::Keyboard_Control(SDL_Keycode button) {

}

void Menu::Mouse_Control(Uint8 button) {
	switch (over_item)
	{
	case Play: {
		if (button == SDL_BUTTON_LEFT)
			item = Play;
		break;
	}
	case Settings: {
		if (button == SDL_BUTTON_LEFT)
			item = Settings;
		break;
	}
	case Exit: {
		if (button == SDL_BUTTON_LEFT) 
			quit = true;		
		break;
	}
	default:
		cout << endl << "Menu error (wrong item)";
		break;
	}	
}

void Menu::Menu_Navigation(int x_pos, int y_pos) {
	if (keyboard)
		item = over_item;	
}

//void Menu::Click(function<void> func(int, int)) {
//	// ��������� ��������� ���� //
//
//	int x_pos = 0, y_pos = 0;
//	SDL_GetMouseState(&x_pos, &y_pos);
//	func(x_pos, y_pos);
//}

void Menu::Start_game() {
	
	LTimer fpsTimer;
	LTimer capTimer;

	fpsTimer.start();

	SDL_GameController* game_controller = nullptr;
	game_controller = SDL_GameControllerOpen(0);
	if (game_controller == nullptr)
		cout << endl << "Controller load error: " << SDL_GetError();

	int countedFrames = 0;

	SDL_Event e;

	while (!quit && !start) {
		capTimer.start();

		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}		

		this->Menu_renderer();
		
		while (SDL_PollEvent(&e) != 0) {

			// ����� ������ //
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				SCREEN_WIDTH = e.window.data1;
				SCREEN_HEIGHT = e.window.data2;

				this->Window_update();
				this->Menu_renderer();

				break;
			}
			if (e.type == SDL_QUIT)
				quit = true;

			// �������� ������� ���������� //
			if (keyboard) {
				if (e.cdevice.type == SDL_CONTROLLERDEVICEADDED) {
					if (game_controller == nullptr)
						game_controller = SDL_GameControllerOpen(0);
					keyboard = false;
				}

				if (e.cbutton.type == SDL_CONTROLLERBUTTONUP)
					keyboard = false;

				if (e.key.type == SDL_KEYUP)
					this->Keyboard_Control(e.key.keysym.sym);

				if (e.button.type == SDL_MOUSEBUTTONUP) {
					this->Mouse_Control(e.button.button);
				}
				// �������� ������� �������� //
			}
			else {
				if (e.key.type == SDL_KEYUP)
					keyboard = true;

				if (e.cbutton.type == SDL_CONTROLLERBUTTONDOWN)
					this->Gamepad_Control(e.cbutton.button);
			}
		}
		++countedFrames;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	fpsTimer.stop();
	capTimer.stop();

RESTART:

	restart = false;
	lose = false;

	Playing_field* field = new Playing_field;

	if (!quit)
		field->Field_Render(true);	

	countedFrames = 0;
	fpsTimer.start();
		
	while (!quit && !lose) {
		capTimer.start();

		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}

		field->Field_Render(true);

		while (SDL_PollEvent(&e) != 0) {

			// ����� ������ //
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				SCREEN_WIDTH = e.window.data1;
				SCREEN_HEIGHT = e.window.data2;

				field->Window_update();
				field->Field_Render(true);

				break;
			}
			if (e.type == SDL_QUIT)
				quit = true;

			// �������� ������� ���������� //
			if (keyboard) {
				if (e.cdevice.type == SDL_CONTROLLERDEVICEADDED) {
					if (game_controller == nullptr)
						game_controller = SDL_GameControllerOpen(0);
					keyboard = false;
				}

				if (e.cbutton.type == SDL_CONTROLLERBUTTONUP)
					keyboard = false;

				if (e.key.type == SDL_KEYUP)
					field->Keyboard_Control(e.key.keysym.sym);

				if (e.button.type == SDL_MOUSEBUTTONUP) {
					field->Mouse_Control(e.button.button);
				}

				// �������� ������� �������� //
			}
			else {
				if (e.key.type == SDL_KEYUP)
					keyboard = true;

				if (e.cbutton.type == SDL_CONTROLLERBUTTONDOWN)
					field->Gamepad_Control(e.cbutton.button);
			}
		}
		++countedFrames;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	fpsTimer.stop();
	capTimer.stop();
	SDL_GameControllerClose(game_controller);

	if (lose) {
		field->~Playing_field();

		cout << endl << "RESTART!";

		if (restart && !quit)
			goto RESTART;
	}
}


///////////////////// Playing_field ////////////////////////////////////////////////////////////////////////////////////////////////////////


Playing_field::Playing_field() {

	ifstream settings_file;

	// �������� �������� �� ����� //

	settings_file.open("settings.json");
	if (!settings_file.is_open())
		cout << endl << "Error to open file \"settings.json \"";

	auto settings = json::parse(settings_file);
	settings_file.close();

	// �������� � ����������� ������ ����������� �� ������ //

	width = settings.at("widthOfPlayingField").get<int>();
	height = settings.at("heightOfPlayingField").get<int>();	
	number_of_mines = settings.at("numberOfMines").get<int>();
	
	textures = Image_Load("assets\\textures.png");
	shaded_textures = Image_Load("assets\\textures.png");

	SDL_SetTextureBlendMode(shaded_textures, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(shaded_textures, 100);

	if (textures == nullptr || shaded_textures == nullptr)
		cout << endl << "Texture load error: " << SDL_GetError();

	// ������ UI ��� ������ ����

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

	// ������������� �������� //

	mines_and_numbers = new signed char* [width];
	open_cells = new bool* [width];
	player_interaction = new bool* [width];

	for (int i = 0; i < width; i++) {
		*(mines_and_numbers + i) = new signed char[height];
		*(open_cells + i) = new bool[height];
		*(player_interaction + i) = new bool[height];
	}

	// ������������� �������� "������" //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			open_cells[j][i] = false;
			player_interaction[j][i] = false;
			mines_and_numbers[j][i] = 0;
		}

	int temp = number_of_mines;
	int temp2 = 0;

	// ��������� ������� ���� //

	//cout << endl << "\tmines on:";
	while (temp != 0)
		for (auto i = 0; i < height; i++) {
			temp2 = 0;

			for (auto j = 0; j < width; j++) {
				if (Generate_Random_Int(0, 200) <= ((number_of_mines * 100 * 1.92) / (width * height)) && mines_and_numbers[j][i] != -1) {
					if (temp2 > number_of_mines / height * 1.5)
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

	// ����������� ���������� ���������� ������ ���� //

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

	// ������������ �������� ������ ���������� ������ � ������� ����������� ���������� ���� //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mines_and_numbers[j][i] == 0) {
				Cell_Opening(j, i);
				goto EXIT2;
			}
	EXIT2:
	
	time_from_start = SDL_GetTicks();
}

Playing_field::~Playing_field() {
		
	// �������� ���� ������ ���� ��� ������ �������������� ���� //

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

			if (e.key.type == SDL_KEYUP)
				switch (e.key.keysym.sym)
				{
				case SDLK_BACKSPACE: {
					Window_Fullscreen();
					break;
				}
				}
			
			switch (e.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED: {

				SCREEN_WIDTH = e.window.data1;
				SCREEN_HEIGHT = e.window.data2;

				Window_update();
				Field_Render(false);

				break;
			}
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

	// ������� ������ //
	
	delete mines_and_numbers;
	delete open_cells;
	delete player_interaction;

	SDL_DestroyTexture(textures);
	SDL_DestroyTexture(shaded_textures);
}

void Playing_field::Window_update() {
	
	// ��������� �������� ����������������� ���������� ��� �������� ������� ���� //
	
	if (SCREEN_HEIGHT / SCREEN_WIDTH <= 2.1 && SCREEN_WIDTH / SCREEN_HEIGHT <= 2.1)
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
	// ����� �������� ���� ����� ���� � ������� //
	
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
	
	// UI �������� //
		// ������ ������ ������ ���� //

	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
			Cell_Render(j, i, render_numbers);

		// ������ ������� ������ //

	SDL_Rect src = { 3, 0, 14, 20 };
	SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 0.15 };
	
	SDL_RenderCopy(renderer, textures, &src, &dst);

		// ������ �������� ���������� ���� //

	src = { 20, 0, 20, 20 };
	dst = { x+ int(SCREEN_HEIGHT * 0.16 / 2), int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (number_of_mines - number_of_flags > 99 ? 3 : 2)), int(SCREEN_HEIGHT * 0.15 / 2) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Numbers_Renderer(number_of_mines - number_of_flags, &dst);

	src.x = 60;
	dst.x = x; dst.w = dst.h;

	SDL_RenderCopy(renderer, textures, &src, &dst);

		// ������ ���������� ������� � ������ ������ //

	src = { 20, 0, 20, 20 };
	dst = { x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3) ), int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3)), int(SCREEN_HEIGHT * 0.15 / 2) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Numbers_Renderer((int((SDL_GetTicks() - time_from_start) / 1000) < 9999 ? int((SDL_GetTicks() - time_from_start) / 1000) : 9999 ), &dst);

	// ������ ��������� ������ ��� �������� //

	if (!keyboard && render_numbers)
		Cell_Lighter(x_cell_controller, y_cell_controller);
	
	SDL_RenderPresent(renderer);

	main_render.unlock();
}


void Playing_field::Cell_Render(int pos_x, int pos_y, bool render_numbers) {
	SDL_Rect dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
	SDL_Rect src;
		
	if (!open_cells[pos_x][pos_y]) {
		// ������ ������ //	
		src = { 0, 0, 20, 20 };
		SDL_RenderCopy(renderer, textures, &src, &dst);

		// ������ "�����" //
		if (player_interaction[pos_x][pos_y]) {
			src = { 40, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}
	} else {
		// ������ �������� ������ //
		src = { 20, 0, 20, 20 };
		SDL_RenderCopy(renderer, textures, &src, &dst);		

		// ������ ��� � ������ (��� ���������) //
		if (mines_and_numbers[pos_x][pos_y] == -1) {
			src = { 60, 0, 20, 20 };
			dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}			
		if (player_interaction[pos_x][pos_y] && !render_numbers) {
			src = { 40, 0, 20, 20 };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}

		// ������ ����� �������� ������ //
		if (mines_and_numbers[pos_x][pos_y] > 0 && render_numbers) {
			dst = { x + pos_x * cell_size + int(cell_size * 0.22), y + pos_y * cell_size + int(cell_size * 0.23), int(cell_size * 0.625), int(cell_size * 0.625) };
			SDL_RenderCopy(renderer, ((mines_and_numbers[pos_x][pos_y] > 4 ? black_numbers : numbers) + mines_and_numbers[pos_x][pos_y])->texture, NULL, &dst);
		}			
	} 	
}

void Playing_field::Cell_Opening(int x_pos, int y_pos) {
	
	// �������� ������ �� ������� "�����" //

	if (player_interaction[x_pos][y_pos] == false) {
		
		// ���� �� ���� => �������� //
		
		if (mines_and_numbers[x_pos][y_pos] == -1) {
			lose = true;
			return;
		}

		// �������� ������ � ������������ �������� //

		if (open_cells[x_pos][y_pos] == false) {
			open_cells[x_pos][y_pos] = true;
			//Opening_Animation(x_pos, y_pos);
		}

		// �������� ����������� ������, ���� ����� � ������� ������� ��� ���� //

		if (mines_and_numbers[x_pos][y_pos] == 0) {

			int arrayLOL[8][2] = { {-1,-1},	{0,-1},	{1,-1},		{-1,0}, {1,0},		 {-1,1},		{0,1},			{1,1} };
			int arrayKEK[8][2] = { {0,0},	{-2,0}, {width - 1,0},	{0,-2}, {width - 1,-2},	 {0,height - 1},	{-2,height - 1},	{width - 1,height - 1} };

			for (int k = 0; k < 8; k++)
				if (x_pos != arrayKEK[k][0] && y_pos != arrayKEK[k][1])
					if (open_cells[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]] == false)
						if (mines_and_numbers[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]] != -1)
							Cell_Opening(x_pos + arrayLOL[k][0], y_pos + arrayLOL[k][1]);
		}
	}
}

void Playing_field::Flag_setter(int x_pos, int y_pos) {
	// ��������� ����� //
		
	if (player_interaction[x_pos][y_pos] == false && open_cells[x_pos][y_pos] == false && number_of_mines - number_of_flags > 0) {
		player_interaction[x_pos][y_pos] = true;
		++number_of_flags;
	} else if (*(*(player_interaction + x_pos) + y_pos) == true && *(*(open_cells + x_pos) + y_pos) == false) {
		player_interaction[x_pos][y_pos] = false;
		--number_of_flags;
	}

	if (number_of_mines - number_of_flags >= 0) {
		for (auto i = 0; i < height; i++)
			for (auto j = 0; j < width; j++) {
				if (*(*(mines_and_numbers + j) + i) == -1 && *(*(player_interaction + j) + i) == true) {}
				else { goto EXIT; }
			}
		Win();

		EXIT:;
	}
}

void Playing_field::Win() {
	Field_Render(true);
	SDL_Color win_color = { 200, 200, 200 };
	Text_render win("You are win!", win_color, 50);
	//win.Init("You are win!", win_color, 50);
	SDL_Rect dst = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2 / 12 };
	SDL_RenderCopy(renderer, win.texture, NULL, &dst);

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {

		// ����� ������ //
		if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			SCREEN_WIDTH = e.window.data1;
			SCREEN_HEIGHT = e.window.data2;

			Window_update();
			Field_Render(true);

			break;
		}
		if (e.type == SDL_QUIT)
			quit = true;
	}

}

void Playing_field::Left_Click() {
	// ��������� ��������� ���� //
	
	int x_pos = 0, y_pos = 0;
	SDL_GetMouseState(&x_pos, &y_pos);
	
	// �������� ��������� ������� �� ��������� ��� ������ ����� //

	if (x_pos - x >= 0 && x_pos - x <= cell_size * width)
		if (y_pos - y >= 0 && y_pos - y <= cell_size * height) {
			x_pos = (x_pos - x) / cell_size;
			y_pos = (y_pos - y) / cell_size;
			
			Cell_Opening(x_pos, y_pos);
		}
}

void Playing_field::Right_Click() {
	// ��������� ��������� ���� //

	int x_pos = 0, y_pos = 0;
	SDL_GetMouseState(&x_pos, &y_pos);

	// �������� ��������� ������� �� ��������� ��� ������ ����� //

	if (x_pos - x >= 0 && x_pos - x <= cell_size * width)
		if (y_pos - y >= 0 && y_pos - y <= cell_size * height) {
			x_pos = (x_pos - x) / cell_size;
			y_pos = (y_pos - y) / cell_size;

			Flag_setter(x_pos, y_pos);
		}
}

void Playing_field::Cell_Lighter(int x_pos, int y_pos) {
	SDL_Rect src = { 47, 7, 6, 6 };
	SDL_Rect dst = { x+x_pos*cell_size, y+y_pos*cell_size, cell_size, cell_size };

	SDL_RenderCopy(renderer, shaded_textures, &src, &dst);
}

void Playing_field::Gamepad_Control(Uint8 button) {
	switch (button)
	{
	case SDL_CONTROLLER_BUTTON_DPAD_UP: {
		if (y_cell_controller > 0)
			--y_cell_controller;
		break;
	}
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN: {
		if (y_cell_controller < height - 1)
			++y_cell_controller;
		break;
	}
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT: {
		if (x_cell_controller > 0)
			--x_cell_controller;
		break;
	}
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: {
		if (x_cell_controller < width - 1)
			++x_cell_controller;
		break;
	}
	case SDL_CONTROLLER_BUTTON_A: {
		Cell_Opening(x_cell_controller, y_cell_controller);
		break;
	}
	case SDL_CONTROLLER_BUTTON_X: {
		Flag_setter(x_cell_controller, y_cell_controller);
		break;
	}
	}
}

void Playing_field::Keyboard_Control(SDL_Keycode button) {
	if (button == SDLK_BACKSPACE) {
		Window_Fullscreen();		
	}
}

void Playing_field::Mouse_Control(Uint8 button) {
	switch (button) {
	case SDL_BUTTON_LEFT: {
		Left_Click();
		break;
	}
	case SDL_BUTTON_RIGHT: {
		Right_Click();
		break;
	}
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

Text_render::Text_render(string str, SDL_Color text_color, int size) {
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
}

void LTimer::start() { 
	mStartTicks = SDL_GetTicks();  
}

void LTimer::stop() {
	mStartTicks = 0; 	 
}

Uint32 LTimer::getTicks() { 
	return SDL_GetTicks() - mStartTicks;
}