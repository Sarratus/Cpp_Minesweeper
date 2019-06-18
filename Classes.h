#pragma once

#include <iostream>
#include <string>
#include "Vars.h"

class Text_render;
extern Text_render* numbers;

class Playing_field
{
public:
	Playing_field();
	~Playing_field();

	void Cell_Render(int pos_x, int pos_y);
	void Field_Render();
	void Show_mines_to_console();
private:
	int**	mines_and_numbers	=	nullptr;
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