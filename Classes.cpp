#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>


using json = nlohmann::json;

////////////////////// TEXT_INPUT //////////////////////////


void Text_Input::Handle_Input() {	
	
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) 
		if (e.type == SDL_KEYDOWN) {
			// Handle backspace //

			if (e.key.keysym.sym == SDLK_BACKSPACE && input_text.length() > 0) {
				input_text.pop_back();				
			}
			// Handle Ctrl+c //
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
				SDL_SetClipboardText(input_text.c_str());
			}
			// Handle Ctrl+v //
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
				input_text = SDL_GetClipboardText();
				
				while (input_text.length() > 2)
					input_text.pop_back();
			}
		} else if (e.type == SDL_TEXTINPUT)
			// Not Ctrl+c || Ctrl+v //
			if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {		
				if (input_text.length() <= 2 && ( int(e.text.text) >= int('0') && int(e.text.text) <= int('9') ))
					input_text += e.text.text;				
			}
}

void Text_Input::Text_Render() {
	
	Letter_Renderer(input_text, &dst);
}

Text_Input::Text_Input(int a, SDL_Rect destination) {
	SDL_StartTextInput();
	
	dst = { int(destination.x + destination.w / 10), int(destination.y + destination.h / 10), int(destination.h * 0.8), int(destination.w * 0.8) };
	input_text = to_string(a);
}

Text_Input::~Text_Input() {
	SDL_StopTextInput();
}


/////////////////////// Menu ///////////////////////////////


Menu::Menu() {
	
	textures = Image_Load("assets\\textures.png");
	
	SDL_Color text_color = { 40, 40, 45 };
	
	item = Main_Menu;
}

Menu::~Menu() {
	SDL_DestroyTexture(textures);
}

void Menu::Menu_renderer() {
	
	SDL_RenderClear(renderer);

	switch (item) {
		case Main_Menu: {
			Main_Menu_Renderer();				
			break;
		}
		case Play: {
			start = true;
			break;
		}
		case Settings: {
			Settings_Renderer();
			break;
		}
		case Exit: {
			quit = true;
			this->~Menu();
			break;
		}
	}

	SDL_Rect src;
	SDL_Rect dst;

	dst = { int(SCREEN_WIDTH * 0.028), int(SCREEN_HEIGHT * 0.945), int(SCREEN_HEIGHT * 0.03 * 5.35), int(SCREEN_HEIGHT * 0.03) };
	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };

	Letter_Renderer("By Lis", &dst);

	SDL_RenderPresent(renderer);
}

void Menu::Settings_Renderer() {

	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_Rect src, dst;
	
	SDL_RenderClear(renderer);
	
	dst = { int(SCREEN_HEIGHT / 6), 0, SCREEN_WIDTH  - int(SCREEN_HEIGHT / 6), int(SCREEN_HEIGHT / 6) };
	src = { int(ICONS_SIZE / 6), 0, int(ICONS_SIZE * 0.6), ICONS_SIZE };	
	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_HEIGHT / 6), 0, int(SCREEN_HEIGHT / 6 / 20 * 3), int(SCREEN_HEIGHT / 6) };
	src = { 0, 0, int(ICONS_SIZE / 20 * 3), ICONS_SIZE };
	SDL_RenderCopy(renderer, textures, &src, &dst);	

	dst = { 0, 0, int(SCREEN_HEIGHT / 6), int(SCREEN_HEIGHT / 6) };
		
	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y) {
		over_item = Back;
	}
	else { over_item = nothing; }		
	
	if (over_item == Back) {
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	} else {
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };
	}	

	SDL_RenderCopy(renderer, textures, &src, &dst);
	
	src.y += 3 * ICONS_SIZE;
	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 9 / 2 / 1.5), int(SCREEN_HEIGHT / 5), int(SCREEN_HEIGHT / 6 / 2 * 9 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5) };
	Letter_Renderer("Mine field", &dst);

	/*dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 9 / 2 / 1.5), int(SCREEN_HEIGHT / 3.3), int(SCREEN_HEIGHT / 6 / 2 * 4 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5 / 2) };
	Letter_Renderer("width", &dst);

	dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 9 / 2 / 1.5), int(SCREEN_HEIGHT / 3.3), int(SCREEN_HEIGHT / 6 / 2 * 5 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5 / 2) };
	Letter_Renderer("height", &dst);

	dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 9 / 2 / 1.5), int(SCREEN_HEIGHT / 3.3), int(SCREEN_HEIGHT / 6 / 2 * 4 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5 / 2) };
	Letter_Renderer("mines", &dst);*/

	dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 5 / 2 / 1.5), int(SCREEN_HEIGHT / 2), int(SCREEN_HEIGHT / 6 / 2 * 5 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5) };
	Letter_Renderer("Sound", &dst);

	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE / 20};
	dst = { int(SCREEN_WIDTH / 4), int(SCREEN_HEIGHT / 1.65), int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 100) };
	SDL_RenderCopy(renderer, textures, &src, &dst);


	dst = { int(SCREEN_WIDTH / 4), int(SCREEN_HEIGHT / 1.7), int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 25) };
	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y)
		over_item = Sound_Changer;

	src = { 3*ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	dst = { int(SCREEN_WIDTH / 4 + sound_volume * SCREEN_WIDTH / 2 / 100 - SCREEN_HEIGHT / 6 / 2 / 2), int(SCREEN_HEIGHT / 1.65 + SCREEN_HEIGHT / 100 / 2 - SCREEN_HEIGHT / 6 / 2 / 2), int(SCREEN_HEIGHT / 6 / 2), int(SCREEN_HEIGHT / 6 / 2) };	

	SDL_RenderCopy(renderer, textures, &src, &dst);
		
	dst = { int(SCREEN_WIDTH / 6), int(SCREEN_HEIGHT / 1.33), int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 9) };
	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y)
		over_item = Window;

	if (over_item == Window) {
		src = { int(ICONS_SIZE + 0.2*ICONS_SIZE), 0, int(0.6*ICONS_SIZE), ICONS_SIZE };
	}
	else {
		src = { int(0.2*ICONS_SIZE), 0, int(0.6*ICONS_SIZE), ICONS_SIZE };
	}
	SDL_RenderCopy(renderer, textures, &src, &dst);

	src.x -= 0.2*ICONS_SIZE;	src.w = 0.2 * ICONS_SIZE;
	dst.w = int(SCREEN_HEIGHT / 8.5 * 0.2);
	SDL_RenderCopy(renderer, textures, &src, &dst);

	src.x += 0.8 * ICONS_SIZE;	
	dst.x += int(SCREEN_WIDTH / 3 - SCREEN_HEIGHT / 8.5 * 0.2);
	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_WIDTH / 6), int(SCREEN_HEIGHT / 1.33), int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 9) };
	Letter_Renderer("Window", &dst);
			
	dst = { int(SCREEN_WIDTH / 6 + SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 1.33), int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 9) };
	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y)
		over_item = Fullscreen;

	if (over_item == Fullscreen) {
		src = { int(ICONS_SIZE + 0.2 * ICONS_SIZE), 0, int(0.6 * ICONS_SIZE), ICONS_SIZE };
	}
	else {
		src = { int(0.2 * ICONS_SIZE), 0, int(0.6 * ICONS_SIZE), ICONS_SIZE };
	}
	SDL_RenderCopy(renderer, textures, &src, &dst);

	src.x -= 0.2 * ICONS_SIZE;	src.w = 0.2 * ICONS_SIZE;
	dst.w = int(SCREEN_HEIGHT / 8.5 * 0.2);
	SDL_RenderCopy(renderer, textures, &src, &dst);

	src.x += 0.8 * ICONS_SIZE;
	dst.x += int(SCREEN_WIDTH / 3 - SCREEN_HEIGHT / 8.5 * 0.2);
	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_WIDTH / 6 + SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 1.33), int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 9) };
	Letter_Renderer("Fullscreen", &dst);
	

	if (SCREEN_WIDTH >= SCREEN_HEIGHT) {
		dst = { int(SCREEN_HEIGHT / 6 + SCREEN_HEIGHT / 6 / 20 * 3 * 2), int(SCREEN_HEIGHT / 6 / 3.5), int(SCREEN_HEIGHT / 6 / 2 * 7), int(SCREEN_HEIGHT / 6 / 2) };
		Letter_Renderer("Settings", &dst);


	} else {
		dst = { int(SCREEN_HEIGHT / 6 + SCREEN_HEIGHT / 6 / 20 * 3 * 2), int(SCREEN_HEIGHT / 6 / 3.5), int(SCREEN_WIDTH / 6 / 2 * 7), int(SCREEN_HEIGHT / 6 / 2) };
		Letter_Renderer("Settings", &dst);


	}

	SDL_RenderPresent(renderer);
}

void Menu::Main_Menu_Renderer() {
	SDL_Rect dst;
	SDL_Rect dst_text;
	SDL_Rect src;

	int x, y;
	SDL_GetMouseState(&x, &y);	

	dst = { int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT * 2 / 3) - int(SCREEN_WIDTH / 9 / 2), int(SCREEN_WIDTH / 9), int(SCREEN_WIDTH / 9) };
	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	
	while (dst.x > 0)
		dst.x -= dst.w;
	while (dst.y > 0)
		dst.y -= dst.h;

	int primary_x = dst.x;
	
	while (1) {
		SDL_RenderCopy(renderer, textures, &src, &dst);
		dst.x += dst.w;

		if (dst.x >= SCREEN_WIDTH) {
			dst.x = primary_x;
			dst.y += dst.h;
		}
		if (dst.y >= SCREEN_HEIGHT)
			break;			
	}

	// Генерация надписи //
	src = { 23, 0, 14, ICONS_SIZE };
	dst = { 0, int(SCREEN_HEIGHT * 2 / 3) - int(SCREEN_WIDTH / 9 / 2) - (dst.h * 2), SCREEN_WIDTH, int(SCREEN_WIDTH * 0.8 / 11 * 1.55) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	dst = { int(SCREEN_WIDTH * 0.1 * 1.04), int(dst.y + dst.h / 4), int(SCREEN_WIDTH * 0.8), int(SCREEN_WIDTH * 0.8 / 11) };

	Letter_Renderer("Minesweeper", &dst);
	
	// Генерация кнопок //
		// Играть //
			// Кнопка //
	dst = { int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT * 2 / 3 ) - int(SCREEN_WIDTH / 9 / 2), int(SCREEN_WIDTH / 9), int(SCREEN_WIDTH / 9) };

	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h && keyboard) { 
		over_item = Play; 
	} else if (keyboard)
		over_item = nothing;  

	if (over_item == Play) {
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	} else 
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };

	SDL_RenderCopy(renderer, textures, &src, &dst);
	src.y += ICONS_SIZE;
	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Настройки //
			// Кнопка //
	dst.x += dst.w;

	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h && keyboard) 
		over_item = Settings; 	

	if (over_item == Settings) {
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	} else
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };

	SDL_RenderCopy(renderer, textures, &src, &dst);
	src.y += 2*ICONS_SIZE;
	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Выход //
	dst.x += dst.w;
	
	if (x >= dst.x && x <= dst.x + dst.w && y >= dst.y && y <= dst.y + dst.h && keyboard) 
		over_item = Exit; 	

	if (over_item == Exit) {
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	} else
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };

	SDL_RenderCopy(renderer, textures, &src, &dst);
	
		// Мина на кнопке выхода //
	src.y += 3 * ICONS_SIZE;

	SDL_RenderCopy(renderer, textures, &src, &dst);
}

void Menu::Window_update(SDL_Event &e) {	
	// Установка "нормальных" размеров окна при "нестандартном масштабировании" //

	if (e.window.data1 > e.window.data2 * 2.1 || e.window.data2 > e.window.data1 * 2.1) {
		if (e.window.data1 == SCREEN_WIDTH) {
			SCREEN_HEIGHT = SCREEN_WIDTH / 2.1;
		}
		else if (e.window.data2 == SCREEN_HEIGHT) {
			SCREEN_WIDTH = SCREEN_HEIGHT / 2;
		}
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		//cout << endl << "Are you yebok? O_o";
	} else {
		SCREEN_WIDTH = e.window.data1;
		SCREEN_HEIGHT = e.window.data2;
	}
	
	this->Menu_renderer();
}

void Menu::Gamepad_Control(Uint8 button) {
	switch (button) {
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
	case SDL_CONTROLLER_BUTTON_DPAD_UP: {
		if (over_item > Play) {
			over_item = static_cast<Game_Menu>(int(over_item) - 1);
		} else if (over_item == Play)
			over_item = Exit;

		break;
	}
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN: {
		if (over_item < Exit) {
			over_item = static_cast<Game_Menu>(int(over_item) + 1);
		} else if (over_item == Exit)
			over_item = Play;

		break;
	}
	case SDL_CONTROLLER_BUTTON_A: {
		item = over_item;
		break;
	}
	}
}

void Menu::Keyboard_Control(SDL_Keycode button) {

}

void Menu::Mouse_Control(Uint8 button) {
	if (button == SDL_BUTTON_LEFT) 
		switch (over_item)
		{
		case Play: {
			item = Play;
			break;
		}
		case Settings: {
			item = Settings;
			break;
		}
		case Exit: {
			quit = true;
			break;
		}
		case Back: {
			item = Main_Menu;
			break;
		}
		case Sound_Changer: {
			int x, y;
			SDL_GetMouseState(&x, &y);

			sound_volume = int((float(x - SCREEN_WIDTH / 4) / float(SCREEN_WIDTH / 2)) * 100);
			break;
		}
		case Window: {
			SDL_SetWindowFullscreen(window, 0);
			break;
		}
		case Fullscreen: {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;
		}
		}	
	
}

void Menu::Menu_Navigation(int x_pos, int y_pos) {
	if (keyboard)
		item = over_item;	
}

//void Menu::Click(function<void> func(int, int)) {
//	// Получение координат мыши //
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
	if (game_controller == nullptr) {
		cout << endl << "Controller load error: " << SDL_GetError();
	}

	int countedFrames = 0;

	SDL_Event e;

	bool is_pressed_on_volume = false;

	while (!quit && !start) {
		capTimer.start();

		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}		

		this->Menu_renderer();

		while (SDL_PollEvent(&e) != 0) {

			// Общие ивенты //
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				this->Window_update(e);
			
			if (e.type == SDL_QUIT)
				quit = true;			

			// Проверка нажатий клавиатуры //

			if (keyboard) {
				
				if (e.button.type == SDL_MOUSEBUTTONUP) {
					this->Mouse_Control(e.button.button);
					is_pressed_on_volume = false;
				}			

				if (e.button.type == SDL_MOUSEBUTTONDOWN && over_item == Sound_Changer) {
					is_pressed_on_volume = true;
				}

				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && e.type == SDL_MOUSEMOTION && is_pressed_on_volume) {
					int m_x;					
					SDL_GetMouseState(&m_x, NULL);
					
					sound_volume = int((float(m_x - SCREEN_WIDTH / 4) / float(SCREEN_WIDTH / 2)) * 100);

					if (sound_volume > 100)
						sound_volume = 100;
					if (sound_volume < 0)
						sound_volume = 0;
				}
								
				if (e.cdevice.type == SDL_CONTROLLERDEVICEADDED) {
					if (game_controller == nullptr)
						game_controller = SDL_GameControllerOpen(0);
					keyboard = false;
					over_item = Play;
				}

				if (e.cbutton.type == SDL_CONTROLLERBUTTONUP) {
					keyboard = false;
					over_item = Play;
				}
					
				if (e.key.type == SDL_KEYUP)
					this->Keyboard_Control(e.key.keysym.sym);

				// Проверка нажатий геймпада //
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
		
	while (!quit && !lose && !restart) {
		
		capTimer.start();

		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}

		field->Field_Render(true);

		while (SDL_PollEvent(&e) != 0) {

			// Общие ивенты //
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)				
					field->Window_update(e);		
			
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.button.type == SDL_MOUSEBUTTONUP) 
				field->Mouse_Control(e.button.button);
			
			// Проверка нажатий клавиатуры //
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

				// Проверка нажатий геймпада //
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
	
	if (lose)
		cout << endl << "You are loser!";

	if (lose || restart) 
		field->~Playing_field();					

	if (restart && !quit) {
		cout << endl << "RESTART!";
		//field->~Playing_field();
		goto RESTART;
	}
		

	SDL_GameControllerClose(game_controller);
}


///////////////////// Playing_field ////////////////////////////////////////////////////////////////////////////////////////////////////////


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

	SDL_SetTextureBlendMode(shaded_textures, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(shaded_textures, 100);

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

	mines_and_numbers = new signed char* [width];
	open_cells = new bool* [width];
	player_interaction = new bool* [width];

	for (int i = 0; i < width; i++) {
		*(mines_and_numbers + i) = new signed char[height];
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
	
	time_from_start = SDL_GetTicks();
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

			if (e.key.keysym.sym == SDLK_r || e.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
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
			
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				Window_update(e);			
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

void Playing_field::Window_update(SDL_Event &e) {	
	
	// Установка "нормальных" размеров окна при "нестандартном масштабировании" //
	
	if (e.window.data1 > e.window.data2 * 2.1 || e.window.data2 > e.window.data1 * 2.1) {
		if (e.window.data1 == SCREEN_WIDTH) {
			SCREEN_HEIGHT = SCREEN_WIDTH / 2.1;					

		} else if (e.window.data2 == SCREEN_HEIGHT) {
			SCREEN_WIDTH = SCREEN_HEIGHT / 2;			
		}		
		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		//cout << endl << "Are you yebok? O_o";
	}
	else {
		SCREEN_WIDTH = e.window.data1;
		SCREEN_HEIGHT = e.window.data2;	
	}

	// Установка размеров пользовательского интерфейса для текущего размера окна //

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

	this->Field_Render(true);

}

void Playing_field::Show_mines_to_console() {
	// Вывод значений всех ячеек поля в консоль //
	
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
	
	// UI рендерер //
		// Рендер каждой ячейки поля //

	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
			Cell_Render(j, i, render_numbers);

		// Рендер верхней панели //

	SDL_Rect src = { 3, 0, 14, ICONS_SIZE };
	SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 0.15 };
	
	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер счётчика оставшихся бомб //

	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	dst = { x+ int(SCREEN_HEIGHT * 0.16 / 2), int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (number_of_mines - number_of_flags > 99 ? 3 : 2)), int(SCREEN_HEIGHT * 0.15 / 2) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Numbers_Renderer(number_of_mines - number_of_flags, &dst);

	src.x = 60;
	dst.x = x; dst.w = dst.h;

	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер прошедшего времени с начала раунда //

	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	dst = { x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3) ), int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3)), int(SCREEN_HEIGHT * 0.15 / 2) };

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Numbers_Renderer((int((SDL_GetTicks() - time_from_start) / 1000) < 9999 ? int((SDL_GetTicks() - time_from_start) / 1000) : 9999 ), &dst);

	// Рендер выделения клеток для геймпада //

	if (!keyboard && render_numbers)
		Cell_Lighter(x_cell_controller, y_cell_controller);
	
 	SDL_RenderPresent(renderer);

	main_render.unlock();
}


void Playing_field::Cell_Render(int pos_x, int pos_y, bool render_numbers) {
	SDL_Rect dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
	SDL_Rect src;
		
	if (!open_cells[pos_x][pos_y]) {
		// Рендер клетки //	
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };
		SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер "флага" //
		if (player_interaction[pos_x][pos_y]) {
			src = { 2*ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}
	} else {
		// Рендер подложки клетки //
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
		SDL_RenderCopy(renderer, textures, &src, &dst);		

		// Рендер мин и флагов (для проигрыша) //
		if (mines_and_numbers[pos_x][pos_y] == -1) {
			src = { 3*ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
			dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}			
		if (player_interaction[pos_x][pos_y] && !render_numbers) {
			src = { 2*ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}

		// Рендер чисел открытых клеток //
		if (mines_and_numbers[pos_x][pos_y] > 0 && render_numbers) {
			dst = { x + pos_x * cell_size + int(cell_size * 0.22), y + pos_y * cell_size + int(cell_size * 0.23), int(cell_size * 0.625), int(cell_size * 0.625) };
			SDL_RenderCopy(renderer, ((mines_and_numbers[pos_x][pos_y] > 4 ? black_numbers : numbers) + mines_and_numbers[pos_x][pos_y])->texture, NULL, &dst);
		}			
	} 	
}

void Playing_field::Cell_Opening(int x_pos, int y_pos) {
	
	// Проверка клетки на наличие "флага" //

	if (player_interaction[x_pos][y_pos] == false) {
		
		// Клик на мину => проигрыш //
		
		if (mines_and_numbers[x_pos][y_pos] == -1) {
			lose = true;
			return;
		}

		// Открытие ячейки и проигрывание анимации //

		if (open_cells[x_pos][y_pos] == false) {
			open_cells[x_pos][y_pos] = true;
			//Opening_Animation(x_pos, y_pos);
		}

		// Открытие прилегающий клеток, если рядом с текущей клеткой нет бомб //

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
	// Установка флага //
		
	if (player_interaction[x_pos][y_pos] == false && open_cells[x_pos][y_pos] == false && number_of_mines - number_of_flags > 0) {
		player_interaction[x_pos][y_pos] = true;
		++number_of_flags;
		//cout << endl << "++ " << number_of_flags;
	} else if (*(*(player_interaction + x_pos) + y_pos) == true && *(*(open_cells + x_pos) + y_pos) == false) {
		player_interaction[x_pos][y_pos] = false;
		--number_of_flags;
		//cout << endl << "-- " << number_of_flags;
	}

	// Проверка на "выигрыш" //

	if (number_of_mines - number_of_flags == 0) {
		//cout << endl << "Max flags!";
		for (auto i = 0; i < height; i++)
			for (auto j = 0; j < width; j++) {
				if ((mines_and_numbers[j][i] == -1 && player_interaction[j][i] == true) || (mines_and_numbers[j][i] != -1)) {}
				else { 
					//cout << endl << "Not correct flags: " << j+1 << ", " << i+1; 
					goto EXIT; 
				}
			}
		cout << endl << "You are winner!";

		Win();

		EXIT:;
	}
}

void Playing_field::Win() {
	Field_Render(true);

	SDL_Color win_color = { 20, 20, 22 };
	Text_render win("You are win!", win_color, 50);
	
	SDL_Rect dst = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2 / 12 };
	SDL_RenderCopy(renderer, win.texture, NULL, &dst);

	SDL_RenderPresent(renderer);

	LTimer fpsTimer;
	LTimer capTimer;

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

				// Общие ивенты //
				
				if (e.type == SDL_QUIT)
					quit = true;

				if (e.window.type == SDL_WINDOWEVENT_SIZE_CHANGED) {
					this->Window_update(e);

					SDL_Rect dst = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_WIDTH / 2 / 12 };
					SDL_RenderCopy(renderer, win.texture, NULL, &dst);

					SDL_RenderPresent(renderer);
				}

				if (e.key.keysym.sym == SDLK_r || e.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
					restart = true;
			}

		++countedFrames;

		int frameTicks = capTimer.getTicks();  
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	fpsTimer.stop();
	capTimer.stop();		
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
	case SDL_CONTROLLER_BUTTON_B:
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