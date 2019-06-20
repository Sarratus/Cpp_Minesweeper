#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include "Vars.h"

class Text_render;
extern Text_render* numbers;

class Playing_field
{
public:
	Playing_field();
	~Playing_field();

	void Cell_Render(int pos_x, int pos_y, bool render_numbers);
	void Field_Render(bool render_numbers);
	void Show_mines_to_console();
	void Window_update();
	void Left_Click();
	void Right_Click();
	void Cell_Opening(int x_pos, int y_pos);
	void Opening_Animation(int x_pos, int y_pos);
	void Flag_setter(int x_pos, int y_pos);

private:
	short int**	mines_and_numbers	=	nullptr;
	bool**	open_cells		=	nullptr;
	bool**	player_interaction	=	nullptr;
	
	SDL_Texture* textures = nullptr;
	SDL_Texture* shaded_textures = nullptr;
	
	int cell_size = NULL;
	int x = NULL;
	int y = NULL;
	int width = NULL;
	int height = NULL;
	int number_of_mines = NULL;

};

class Text_render
{
public:	
	void Init(string text, SDL_Color text_color, int size);
	~Text_render();
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
	void pause(); 
	void unpause(); 
		
	Uint32 getTicks(); 
		
	bool isStarted(); 
	bool isPaused(); 
	private: 
			
	Uint32 mStartTicks; 
		
	Uint32 mPausedTicks; 
		
	bool mPaused; 
	bool mStarted; 
};