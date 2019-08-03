#include <SDL.h>
#include <SDL_ttf.h>
#include "Classes.h"
#include "Vars.h"
#include <chrono>
#include <list>


////////////////////// TEXT_INPUT //////////////////////////


void Text_Input::Handle_Input(SDL_Event e) {		
	
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_DELETE) {
				input_text.erase();
			}
			// Handle backspace //
			else if (e.key.keysym.sym == SDLK_BACKSPACE && input_text.length() > 0) {
				input_text.pop_back();				
			}
			// Handle Ctrl+c //
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
				SDL_SetClipboardText(input_text.c_str());
			}
			// Handle Ctrl+v //
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
				string temp = SDL_GetClipboardText();
				temp.resize(2);

				while (temp.size() != 0 && (!(int(temp.back()) >= int('0') && int(temp.back()) <= int('9'))))
					if (temp.size() > 0) {
						cout << endl << temp.back();
						temp.pop_back();
					}
				
				if (temp.size() != 0)
					input_text = SDL_GetClipboardText();						
			}
		} else if (e.type == SDL_TEXTINPUT)
			// Not Ctrl+c || Ctrl+v //
			if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {						
				if (input_text.size() < 2) {
					input_text += e.text.text;
					
					if (!(int(input_text.back()) >= int('0') && int(input_text.back()) <= int('9')))
						input_text.pop_back();
				}					
			}
}

void Text_Input::Text_Render(SDL_Rect* dst, int padding) {	
	Letters_Padded_Renderer(input_text, dst, padding);
	
	SDL_Rect dst1 = *dst;
	dst1.y -= SCREEN_HEIGHT / 50;

	SDL_SetRenderDrawColor(renderer, 177, 44, 44, 255);
	for (auto i = 0; i < 4; i++) {
		SDL_RenderDrawLine(renderer, dst1.x, dst1.y + dst1.h, dst1.x + dst1.w, dst1.y + dst1.h);
		--dst1.y;
	}	
	SDL_SetRenderDrawColor(renderer, 180, 180, 183, 255);
	
}

Text_Input::Text_Input(int a) {
	SDL_StartTextInput();
		
	input_text = to_string(a);
}

Text_Input::~Text_Input() {

	SDL_StopTextInput();	
}


/////////////////////// Menu ///////////////////////////////


Menu::Menu() {
	over_item = nothing;
	item = Main_Menu;
	current_text_input_field = nothing;

	textures = Image_Load("assets\\textures.png");
	
	SDL_Color text_color = { 40, 40, 45 };
}

Menu::~Menu() {
	if (text_input != NULL)
		text_input->~Text_Input();

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

	SDL_Rect src, dst, dst1;
	
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

	dst = { SCREEN_HEIGHT < SCREEN_WIDTH ? 0 : int(SCREEN_HEIGHT / 6), 0, int((SCREEN_HEIGHT < SCREEN_WIDTH ? SCREEN_WIDTH : SCREEN_WIDTH - SCREEN_HEIGHT / 6)), int(SCREEN_HEIGHT / 6) };
	Letters_Padded_Renderer("Settings", &dst, (SCREEN_HEIGHT > SCREEN_WIDTH ? 23 : 45) );

	dst = { int(SCREEN_WIDTH / 2) - int(SCREEN_HEIGHT / 6 / 2 * 9 / 2 / 1.5), int(SCREEN_HEIGHT / 5), int(SCREEN_HEIGHT / 6 / 2 * 9 / 1.5), int(SCREEN_HEIGHT / 6 / 2 / 1.5) };
	Letter_Renderer("Mine field", &dst);	

	src = { int(ICONS_SIZE * 2.5), int(ICONS_SIZE * 0.3), 1, 1 };

	dst = { 0, int(SCREEN_HEIGHT / 4.35), int(SCREEN_WIDTH / 2.8), int(SCREEN_HEIGHT / 6) };
	Letters_Padded_Renderer("width:", &dst, 60);
	
	dst.y += int(dst.h / 3); dst.x -= int(SCREEN_WIDTH / 80);
	if (current_text_input_field == Width_text_input) {
		text_input->Text_Render(&dst, 65);
	} else {
		Letters_Padded_Renderer(to_string(playing_field_width), &dst, 65);
	}
	dst.y -= int(dst.h / 3); dst.x += int(SCREEN_WIDTH / 80);

	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y) {
		over_item = Width_text_input;
		if (current_text_input_field == nothing) {
			dst1 = { 0, int(SCREEN_HEIGHT / 4) + int(SCREEN_HEIGHT / 6), int(SCREEN_WIDTH / 3), 3 };
			SDL_RenderCopy(renderer, textures, &src, &dst1);
		}		
	}


	dst.x += SCREEN_WIDTH / 3;
	Letters_Padded_Renderer("height:", &dst, 60);
	
	dst.y += int(dst.h / 3); dst.x -= int(SCREEN_WIDTH / 80);
	if (current_text_input_field == Height_text_input) {
		text_input->Text_Render(&dst, 65);
	} else {
		Letters_Padded_Renderer(to_string(playing_field_height), &dst, 65);
	}
	dst.y -= int(dst.h / 3); dst.x += int(SCREEN_WIDTH / 80);

	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y) {
		over_item = Height_text_input;
		if (current_text_input_field == nothing) {
			dst1 = { int(SCREEN_WIDTH / 3), int(SCREEN_HEIGHT / 4) + int(SCREEN_HEIGHT / 6), int(SCREEN_WIDTH / 3), 3 };
			SDL_RenderCopy(renderer, textures, &src, &dst1);
		}
	}	

	dst.x += SCREEN_WIDTH / 3;
	Letters_Padded_Renderer("mines:\0\0  ", &dst, 60);
	
	dst.y += int(dst.h / 3); dst.x -= int(SCREEN_WIDTH / 80);
	if (current_text_input_field == Mines_text_input) {
		text_input->Text_Render(&dst, 65);
	} else {
		Letters_Padded_Renderer(to_string(number_of_mines_), &dst, 65);
	}
	dst.y -= int(dst.h / 3); dst.x += int(SCREEN_WIDTH / 80);

	if (x <= dst.x + dst.w && x >= dst.x && y <= dst.y + dst.h && y >= dst.y) {
		over_item = Mines_text_input;
		if (current_text_input_field == nothing) {
			dst1 = { int(SCREEN_WIDTH / 3 * 2), int(SCREEN_HEIGHT / 4) + int(SCREEN_HEIGHT / 6), int(SCREEN_WIDTH / 3), 3 };
			SDL_RenderCopy(renderer, textures, &src, &dst1);
		}
	}

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
	Letters_Padded_Renderer("window", &dst, 46);
			
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
	Letters_Padded_Renderer("fullscreen", &dst, 15);	
		
	

	if (SCREEN_WIDTH >= SCREEN_HEIGHT) {
		/*dst = { int(SCREEN_HEIGHT / 6 + SCREEN_HEIGHT / 6 / 20 * 3 * 2), int(SCREEN_HEIGHT / 6 / 3.5), int(SCREEN_HEIGHT / 6 / 2 * 7), int(SCREEN_HEIGHT / 6 / 2) };
		Letter_Renderer("Settings", &dst);*/


	} else {
		/*dst = { int(SCREEN_HEIGHT / 6 + SCREEN_HEIGHT / 6 / 20 * 3 * 2), int(SCREEN_HEIGHT / 6 / 3.5), int(SCREEN_WIDTH / 6 / 2 * 7), int(SCREEN_HEIGHT / 6 / 2) };
		Letter_Renderer("Settings", &dst);*/


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

	// Установка "нормальных" размеров окна при "нестандартном масштабировании" //



	if (e.window.data1 > e.window.data2 * 2.1 || e.window.data2 > e.window.data1 * 2.1) {
		if (e.window.data1 == SCREEN_WIDTH) {
			if (SCREEN_HEIGHT > e.window.data2) {
				SCREEN_HEIGHT = SCREEN_WIDTH / 2;
			}
			else {
				SCREEN_HEIGHT = SCREEN_WIDTH * 2;
			}
		}
		else if (e.window.data2 == SCREEN_HEIGHT) {
			if (SCREEN_WIDTH > e.window.data1) {
				SCREEN_WIDTH = SCREEN_HEIGHT / 2;
			}
			else {
				SCREEN_WIDTH = SCREEN_HEIGHT * 2;
			}
		}
		if (e.window.data1 < 450)
			SCREEN_WIDTH = 450;

		if (e.window.data2 < 450)
			SCREEN_HEIGHT = 450;

		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		//cout << endl << "Are you yebok? O_o";
	}
	else {
		SCREEN_WIDTH = e.window.data1;
		SCREEN_HEIGHT = e.window.data2;

		if (e.window.data1 < 450) {
			SCREEN_WIDTH = 450;
			SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		}


		if (e.window.data2 < 450) {
			SCREEN_HEIGHT = 450;
			SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		}

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
	switch (button)
	{
	case SDLK_RETURN:
		if (text_input != NULL) {
			switch (current_text_input_field) {
			case Width_text_input:
				playing_field_width = text_input->Value();
				break;
			case Height_text_input:
				playing_field_height = text_input->Value();
				break;
			case Mines_text_input:
				number_of_mines_ = text_input->Value();
				break;
			}
			text_input->~Text_Input();
			text_input = NULL;
			current_text_input_field = nothing;
		}
		break;
	case SDLK_ESCAPE:
		if (text_input != NULL) {
			switch (current_text_input_field) {
			case Width_text_input:
				playing_field_width = text_input->Value();
				break;
			case Height_text_input:
				playing_field_height = text_input->Value();
				break;
			case Mines_text_input:
				number_of_mines_ = text_input->Value();
				break;
			}
			text_input->~Text_Input();
			text_input = NULL;
			current_text_input_field = nothing;
		} else {
			switch (item)
			{
			case Main_Menu:
				item = Exit;
				break;
			case Settings:
				item = Main_Menu;
				break;
			}
		}
		break;
	case SDLK_PLUS:
		//if (KMOD_CTRL)
		
		break;
	}
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
		case Width_text_input: {
			if (text_input == NULL) {
				text_input = new Text_Input(playing_field_width);
				current_text_input_field = Width_text_input;
			}
			break;
		}
		case Height_text_input: {
			if (text_input == NULL) {
				text_input = new Text_Input(playing_field_height);
				current_text_input_field = Height_text_input;
			}
			break;
		}
		case Mines_text_input: {
			if (text_input == NULL) {
				text_input = new Text_Input(number_of_mines_);
				current_text_input_field = Mines_text_input;
			}
			break;
		}
		case nothing: {
			if (text_input != NULL) {				
				switch (current_text_input_field) {
				case Width_text_input:					
					playing_field_width = text_input->Value();
					break;
				case Height_text_input:
					playing_field_height = text_input->Value();
					break;
				case Mines_text_input:
					number_of_mines_ = text_input->Value();
					break;
				}
				text_input->~Text_Input();
				text_input = NULL;		
				current_text_input_field = nothing;
			}
			break;
		}
		}	
}

void Menu::Menu_Navigation(int x_pos, int y_pos) {
	if (keyboard)
		item = over_item;	
}

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

			//  //
			if (this->text_input != NULL)
				this->text_input->Handle_Input(e);

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

	if (!quit) {
		if (playing_field_height * playing_field_width < number_of_mines_) {
			cout << endl << "Too many mines";

			while (playing_field_height* playing_field_width <= number_of_mines_)
				number_of_mines_ /= 5;
		}			

		restart = false;
		lose = false;

		Playing_field* field = new Playing_field;

		field->Field_Render(true);

		countedFrames = 0;
		fpsTimer.start();

		LTimer TEMP;
		TEMP.start();

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

					

					
					
					if (TEMP.getTicks() >= 100) {
					if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_P]) {						
							field->Scale_Changer(false, 5);

					} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_O]) {						
							field->Scale_Changer(true, 5);							

					} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP]) {
						field->Field_Moving(0, SCREEN_HEIGHT / 20);

					} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN]) {
						field->Field_Moving(0, -SCREEN_HEIGHT / 20);

					} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT]) {
						field->Field_Moving(SCREEN_HEIGHT / 20, 0);

					} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT]) {
						field->Field_Moving(-SCREEN_HEIGHT / 20, 0);
					}
					TEMP.start();
					} 
					
					if (e.key.type == SDL_KEYUP) {
						field->Keyboard_Control(e.key.keysym.sym);

						if (e.key.keysym.sym == SDLK_o) {
							//field->Scale_Changer(true, 5);
						} else if (e.key.keysym.sym == SDLK_p) {
							//field->Scale_Changer(false, 5);
						} else if (e.key.keysym.sym == SDLK_UP) {
							field->Field_Moving(0, SCREEN_HEIGHT / 20);
						} else if (e.key.keysym.sym == SDLK_DOWN) {
							field->Field_Moving(0, -SCREEN_HEIGHT / 20);
						} else if (e.key.keysym.sym == SDLK_LEFT) {
							field->Field_Moving(SCREEN_HEIGHT / 20, 0);
						} else if (e.key.keysym.sym == SDLK_RIGHT) {
							field->Field_Moving(-SCREEN_HEIGHT / 20, 0);
						}
					}	
					
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
	}

	SDL_GameControllerClose(game_controller);
}


///////////////////// Playing_field ////////////////////////////////////////////////////////////////////////////////////////////////////////


Playing_field::Playing_field() {

	width = playing_field_width;
	height = playing_field_height;
	number_of_mines = number_of_mines_;
	
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

	mines_and_numbers = new short int* [width];
	open_cells = new bool* [width];
	player_interaction = new bool* [width];
	
	for (int i = 0; i < width; i++) {
		*(mines_and_numbers + i) = new short int[height];
		*(open_cells + i) = new bool[height];
		*(player_interaction + i) = new bool[height];
	}

	arrayLOL = { {-1, -1}, { 0,-1 }, { 1,-1 }, { -1,0 }, { 1,0 }, { -1,1 }, { 0,1 }, { 1,1 } };
	arrayKEK = { {0, 0}, { -2,0 }, { signed char(width - 1),0 }, { 0,-2 }, { signed char(width - 1),-2 }, { 0, signed char(height - 1) }, { -2, signed char(height - 1) }, { signed char(width - 1), signed char(height - 1) } };

	// Инициализация массивов "нулями" //

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			open_cells[j][i] = false;
			player_interaction[j][i] = false;
			mines_and_numbers[j][i] = NULL;
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
	
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) 
			if (mines_and_numbers[j][i] == -1)
				for (int k = 0; k < 8; k++)
					if (j != arrayKEK[k][0] && i != arrayKEK[k][1]) {
						//cout << endl << k+1 << ":" << endl << j << " and " << i << endl << arrayKEK[k][0] << " and " << arrayKEK[k][1] << endl;
						if (mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] != -1)
							mines_and_numbers[j+arrayLOL[k][0]][i+arrayLOL[k][1]] += 1;
					}

	// Единоразовое открытие первой попавшейся клетки с нулевым (или 1, если 0 нет) количеством окружающих бомб //

	bool NULL_opened = false;
	int temp3 = -1;

	TRY:

	++temp3;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (mines_and_numbers[j][i] == temp3) {
				Cell_Opening(j, i);
				NULL_opened = true;
				goto EXIT2;
			}

	if (!NULL_opened)
		goto TRY;

	EXIT2:
		
	
	time_from_start = SDL_GetTicks();
}

Playing_field::~Playing_field() {
		
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
			if (SCREEN_HEIGHT > e.window.data2) {
				SCREEN_HEIGHT = SCREEN_WIDTH / 2;
			} else {
				SCREEN_HEIGHT = SCREEN_WIDTH * 2;
			}
		} else if (e.window.data2 == SCREEN_HEIGHT) {
			if (SCREEN_WIDTH > e.window.data1) {
				SCREEN_WIDTH = SCREEN_HEIGHT / 2;
			}
			else {
				SCREEN_WIDTH = SCREEN_HEIGHT * 2;
			}						
		}		
		if (e.window.data1 < 450)
			SCREEN_WIDTH = 450;

		if (e.window.data2 < 450)
			SCREEN_HEIGHT = 450;

		SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		//cout << endl << "Are you yebok? O_o";
	}
	else {
		SCREEN_WIDTH = e.window.data1;
		SCREEN_HEIGHT = e.window.data2;	

		if (e.window.data1 < 450) {
			SCREEN_WIDTH = 450;
			SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
			

		if (e.window.data2 < 450) {
			SCREEN_HEIGHT = 450;
			SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
			
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

			cell_size = SCREEN_WIDTH * 0.9 / width;
			x = (SCREEN_WIDTH - cell_size * width)/2;
			y = (SCREEN_HEIGHT * 0.2) - (cell_size * height / 2) + (SCREEN_HEIGHT * 0.75 / 2);
		}

	this->Field_Render(true);

}

void Playing_field::Show_mines_to_console() {
	// Вывод значений всех ячеек поля в консоль //
	
	cout << endl << "Playing field[" << width << "][" << height << "]: " << endl;
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
	
	/*if ((x + (cell_size * pos_x) > -cell_size) && (x + (cell_size * pos_x) < SCREEN_WIDTH) && (y + (cell_size * pos_y) > -cell_size) && (y + (cell_size * pos_y) < SCREEN_HEIGHT))*/

	int x_pos_left, x_pos_right;
	int y_pos_up, y_pos_down;

	x_pos_left = (x >= 0) ? 0 : ((-x) / cell_size);
	x_pos_right = (x + (cell_size * width) <= SCREEN_WIDTH) ? width : (width - (x + (cell_size * width) - SCREEN_WIDTH) / cell_size);

	y_pos_up = (y >= 0) ? 0 : ((-y) / cell_size);
	y_pos_down = (y + (cell_size * height) <= SCREEN_HEIGHT) ? height : (height - (y + (cell_size * height) - SCREEN_HEIGHT) / cell_size);
	
	for (auto i = y_pos_up; i < y_pos_down; i++)
		for (auto j = x_pos_left; j < x_pos_right; j++)
			Cell_Render(j, i, render_numbers);

		// Рендер верхней панели //

	SDL_Rect src = { 3, 0, 14, ICONS_SIZE };
	SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 0.15 };
	
	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер счётчика оставшихся бомб //

	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	dst = { x+ int(SCREEN_HEIGHT * 0.16 / 2), int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (number_of_mines - number_of_flags > 99 ? 3 : 2)), int(SCREEN_HEIGHT * 0.15 / 2) };

	bool nes = false;
	bool over_the_edge = false;

	if (dst.x + dst.w > x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3))) {
		dst.x -= int((dst.x + dst.w - (x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3)))) / 2);
		nes = true;
	} else if (dst.x - dst.h < 0) {
		dst.x = dst.h;
		over_the_edge = true;
	}

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Letters_Padded_Renderer(to_string(number_of_mines - number_of_flags), &dst, 25);

	src.x = 3 * ICONS_SIZE;
	if (nes) {
		dst.x -= dst.h;
	} else if (over_the_edge) {
		dst.x = 0;
	} else {
		dst.x = x;
	}

	dst.w = dst.h;

	SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер прошедшего времени с начала раунда //

	src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
	
	if (nes) {
		dst.x = x + int(SCREEN_HEIGHT * 0.16 / 2) + int(SCREEN_HEIGHT * 0.15 / 2 * (number_of_mines - number_of_flags > 99 ? 3 : 2)) - int((x + int(SCREEN_HEIGHT * 0.16 / 2) + int(SCREEN_HEIGHT * 0.15 / 2 * (number_of_mines - number_of_flags > 99 ? 3 : 2)) - (x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3)))) / 2);
	} else {
		dst.x = x + width * cell_size - int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3));
	}

	dst = { dst.x, int(SCREEN_HEIGHT * 0.15 / 4), int(SCREEN_HEIGHT * 0.15 / 2 * (((SDL_GetTicks() - time_from_start) / 1000) > 999 ? 4 : 3)), int(SCREEN_HEIGHT * 0.15 / 2) };

	if (dst.x + dst.w > SCREEN_WIDTH)
		dst.x = SCREEN_WIDTH - dst.w;

	SDL_RenderCopy(renderer, textures, &src, &dst);

	Letters_Padded_Renderer(to_string(int((SDL_GetTicks() - time_from_start) / 1000) < 9999 ? int((SDL_GetTicks() - time_from_start) / 1000) : 9999 ), &dst, 25);

	// Рендер выделения клеток для геймпада //

	if (!keyboard && render_numbers)
		Cell_Lighter(x_cell_controller, y_cell_controller);
	
 	SDL_RenderPresent(renderer);

	main_render.unlock();
}

void Playing_field::Scale_Changer(int new_scale) {

}

int Playing_field::Scale_Changer(bool add, int how_many) {
	
	this->cell_size += (this->cell_size * how_many / 100 >= 1) ? (int(this->cell_size * (add ? 1 : -1) * how_many / 100)) : (add ? 1 : -1);

	if (cell_size < 4) {
		cell_size = 4;
	} else if (cell_size > 80) {
		cell_size = 80;
	}	

	this->Field_Moving(0, 0);

	if (x != (SCREEN_WIDTH - (x + cell_size * width))  &&  (cell_size * width) <= SCREEN_WIDTH) 
		x = (SCREEN_WIDTH - cell_size * width) / 2;
	
	if (y != (SCREEN_HEIGHT - (y + cell_size * height)) && (cell_size * height) <= SCREEN_HEIGHT * 0.85)
		y = (SCREEN_HEIGHT * 0.15) + ((SCREEN_HEIGHT * 0.85) - cell_size * height) / 2;

	return cell_size;
}

int Playing_field::Field_Moving(int x_plus, int y_plus) {
	x += x_plus;

	if (x > SCREEN_WIDTH / 2) {
		x = SCREEN_WIDTH / 2;
	} else if (x < -(cell_size * width - SCREEN_WIDTH / 2)) {
		x = -(cell_size * width - SCREEN_WIDTH / 2);
	}		

	y += y_plus;

	if (y > SCREEN_HEIGHT / 2) {
		y = SCREEN_HEIGHT / 2;
	} else if (y < -(cell_size * height - SCREEN_HEIGHT / 2)) {
		y = -(cell_size * height - SCREEN_HEIGHT / 2);
	}

	if (x_plus != 0)
		return x;

	if (y_plus != 0)
		return y;

	return 0;
}

void Playing_field::Cell_Render(int pos_x, int pos_y, bool render_numbers) {
	
	SDL_Rect dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
	SDL_Rect src;

	if (!open_cells[pos_x][pos_y] && !lose) {
		// Рендер клетки //	
		src = { 0, 0, ICONS_SIZE, ICONS_SIZE };
		SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер "флага" //
		if (player_interaction[pos_x][pos_y]) {
			src = { 2 * ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
			SDL_RenderCopy(renderer, textures, &src, &dst);
		}
	}
	else {
		// Рендер подложки клетки //
		src = { ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
		SDL_RenderCopy(renderer, textures, &src, &dst);

		// Рендер чисел открытых клеток //
		if (mines_and_numbers[pos_x][pos_y] > 0 && render_numbers) {
			dst = { x + pos_x * cell_size + int(cell_size * 0.22), y + pos_y * cell_size + int(cell_size * 0.23), int(cell_size * 0.625), int(cell_size * 0.625) };
			SDL_RenderCopy(renderer, ((mines_and_numbers[pos_x][pos_y] > 4 ? black_numbers : numbers) + mines_and_numbers[pos_x][pos_y])->texture, NULL, &dst);
		}
		// Рендер мин и флагов (для проигрыша) //
		if (lose) {
			if (mines_and_numbers[pos_x][pos_y] == -1) {
				src = { 3 * ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
				dst = { x + pos_x * cell_size, y + pos_y * cell_size, cell_size, cell_size };
				SDL_RenderCopy(renderer, textures, &src, &dst);
			}
			if (player_interaction[pos_x][pos_y] && !render_numbers) {
				src = { 2 * ICONS_SIZE, 0, ICONS_SIZE, ICONS_SIZE };
				SDL_RenderCopy(renderer, textures, &src, &dst);
			}
		}
	}	
}

	list<vector<signed char>> Queue;

	void Playing_field::Cell_Opening(signed char x_pos, signed char y_pos) {

		// Проверка клетки на наличие "флага" //
		

		if (player_interaction[x_pos][y_pos] == false && open_cells[x_pos][y_pos] == false) {

			// Открытие прилегающий клеток, если рядом с текущей клеткой нет бомб //

			if (mines_and_numbers[x_pos][y_pos] == 0) {

				Queue.push_back({ x_pos, y_pos });				

				while (Queue.size() != 0) {					

					NULL_Opening();					
					Queue.pop_front();			
				}
			}
			else if (open_cells[x_pos][y_pos] == false && mines_and_numbers[x_pos][y_pos] != -1) {
				// Открытие ячейки и проигрывание анимации //

				open_cells[x_pos][y_pos] = true;
				//Opening_Animation(x_pos, y_pos);
			}
			else {
				// Клик на мину => проигрыш //

				lose = true;
				return;
			}
		}
	}

	void Playing_field::NULL_Opening() {		
		
		signed char x_pos = Queue.front()[0];
		signed char y_pos = Queue.front()[1];		

		if (mines_and_numbers[x_pos][y_pos] == 0)
			if (open_cells[x_pos][y_pos] == false)
				for (int k = 0; k < 8; k++)
					if (x_pos != arrayKEK[k][0] && y_pos != arrayKEK[k][1])
						if (open_cells[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]] == false)
							if (mines_and_numbers[x_pos + arrayLOL[k][0]][y_pos + arrayLOL[k][1]] != -1) {
								Queue.push_back({ x_pos + arrayLOL[k][0], y_pos + arrayLOL[k][1] });
							}	

		open_cells[Queue.front()[0]][Queue.front()[1]] = true;	

		//this->Field_Render(true);
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

	SDL_Rect src = { ICONS_SIZE + 3, 0, ICONS_SIZE - 6, ICONS_SIZE };
	SDL_FRect dst1 = { 0, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 7 / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 7 };
	SDL_RenderCopyF(renderer, textures, &src, &dst1);

	SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Letters_Padded_Renderer("You are win", &dst, 50);
	
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

					Field_Render(true);

					SDL_RenderCopyF(renderer, textures, &src, &dst1);
					Letters_Padded_Renderer("You are winner", &dst, 50);

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