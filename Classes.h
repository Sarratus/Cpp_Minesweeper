#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include "Vars.h"

class Text_render;
extern Text_render* numbers;
extern Text_render* black_numbers;

class Menu
{
public:
	Menu();
	~Menu();

	void Menu_renderer();
	void Window_update();

	void Gamepad_Control(Uint8 button);

	void Keyboard_Control(SDL_Keycode button);
	void Mouse_Control(Uint8 button);

	void Start_game();

private:

};

class Playing_field
{
public:
	Playing_field();
	~Playing_field();

	void Field_Render(bool render_numbers);
	void Window_update();
	
	void Show_mines_to_console();	
		
	void Gamepad_Control(Uint8 button);

	void Keyboard_Control(SDL_Keycode button);
	void Mouse_Control(Uint8 button);

private:
	//void Opening_Animation(int x_pos, int y_pos);
	void Cell_Render(int pos_x, int pos_y, bool render_numbers);
	void Cell_Opening(int x_pos, int y_pos);
	void Cell_Lighter(int x_pos, int y_pos);

	void Left_Click();
	void Right_Click();
	
	void Flag_setter(int x_pos, int y_pos);

	signed char**	mines_and_numbers	=	nullptr;
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
	
	void Init(string text, SDL_Color text_color, int size);
	
	SDL_Texture* texture = nullptr;
private:	
	SDL_Color color = { 0, 0, 0 };
	string original_text;
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