#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Vars.h"

class Text_render;
extern Text_render* numbers;
extern Text_render* black_numbers;
extern Text_render* alphabet;

enum Game_Menu {

	Window_title = 0,

	Menu_item_1,
	Menu_item_2,
	Menu_item_3,

	Main_Menu = 10,
	Play,
	Settings,
	Exit,

	Back = 20,
	Height_text_input,
	Width_text_input,
	Mines_text_input,
	Sound_Changer,
	Window,
	Fullscreen,

	nothing = 100
};

class Text_Input
{
public:
	Text_Input(int a);
	~Text_Input();

	//void Init(int a, SDL_Rect destination);

	int Value() { 
		if (input_text.size() > 0 && stoi(input_text) > 0) {
			return stoi(input_text);
		} else {
			return 1;
		}			
	}

	void Handle_Input(SDL_Event e);
	void Text_Render(SDL_Rect* dst, int padding);

private:
	string input_text;

	SDL_Rect dst = { 0, 0, 0, 0 };
};

class Menu
{
public:
	Menu();
	~Menu();

	void Menu_renderer();
	void Window_update(SDL_Event &e);

	void Gamepad_Control(Uint8 button);

	void Keyboard_Control(SDL_Keycode button);
	void Mouse_Control(Uint8 button);

	void Start_game();

	void Menu_Navigation(int x_pos, int y_pos);

	Text_Input* text_input = NULL;

private:
	void Main_Menu_Renderer();
	void Settings_Renderer();

	int x = NULL, y = NULL;
	int width = NULL, height = NULL;
	int item_height = NULL, item_width = NULL;

	Game_Menu over_item;
	Game_Menu item;
	Game_Menu current_text_input_field;

	SDL_Texture* textures = nullptr;
};

class Playing_field
{
public:
	Playing_field();
	~Playing_field();

	void Field_Render(bool render_numbers);
	void Window_update(SDL_Event &e);
	
	void Show_mines_to_console();	
		
	void Gamepad_Control(Uint8 button);

	void Keyboard_Control(SDL_Keycode button);
	void Mouse_Control(Uint8 button);
		
private:	
	void Cell_Render(int pos_x, int pos_y, bool render_numbers);
	void Cell_Lighter(int x_pos, int y_pos);
	
	void Cell_Opening(signed char x_pos, signed char y_pos);
	void NULL_Opening();
	void Flag_setter(int x_pos, int y_pos);

	void Left_Click();
	void Right_Click();

	void Win();

	short int**	mines_and_numbers	=	nullptr;
	bool**	open_cells		=	nullptr;
	bool**	player_interaction	=	nullptr;
	
	SDL_Texture* textures = nullptr;
	SDL_Texture* shaded_textures = nullptr;
	
	unsigned long int time_from_start;

	unsigned short int cell_size = NULL;
	
	unsigned short int x = NULL;
	unsigned short int y = NULL;
	
	unsigned int x_cell_controller = NULL;
	unsigned int y_cell_controller = NULL;

	unsigned short int width = NULL;
	unsigned short int height = NULL;
	
	unsigned short int number_of_mines = NULL;
	unsigned short int number_of_flags = NULL;	
};

class Text_render
{
public:	
	~Text_render();
	Text_render(string text, SDL_Color text_color, int size);
	Text_render() {};

	void Init(string text, SDL_Color text_color, int size);
	
	SDL_Texture* texture = nullptr;

	string original_text;

private:	
	SDL_Color color = { 0, 0, 0 };
	
	int width = NULL;
	int height = NULL;

};

class LTimer {
public: 	
	LTimer(); 
		
	void start(); 
	void stop(); 
		
	Uint32 getTicks();

private:			
	Uint32 mStartTicks; 	
};