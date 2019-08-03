#include <vector>
#include <algorithm>
#include "Classes.h"
#include "Vars.h"


void Window_Fullscreen() {

	Uint32 fullscreen_flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool is_fullscreen = SDL_GetWindowFlags(window) & fullscreen_flag;

	if (!is_fullscreen) {
		SDL_SetWindowFullscreen(window, fullscreen_flag);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
	}
}

void Numbers_Renderer(int num, SDL_Rect* dst) {

	vector<int> number;
	
	if (num > 0) {
		while (num > 0) {
			number.push_back(num % 10);
			num /= 10;
		}

		int size = dst->h * 0.8;
		SDL_Rect dst_t = { dst->x + dst->w - size - size * 0.26, dst->y + dst->h / 2 - size / 2 + size * 0.05, size, size };

		for (auto i = number.begin(); i != number.end(); i++) {
			SDL_RenderCopy(renderer, (black_numbers + *i)->texture, NULL, &dst_t);
			dst_t.x -= size;
		}
	} else {
		int size = dst->h * 0.8;
		SDL_Rect dst_t = { dst->x + dst->w - size - size * 0.26, dst->y + dst->h / 2 - size / 2 + size * 0.05, size, size };

		SDL_RenderCopy(renderer, (black_numbers + 0)->texture, NULL, &dst_t);
	}		
}

void Letter_Renderer(string str, SDL_Rect* dst) {

	SDL_Rect dst_t = { dst->x, dst->y, dst->w/str.size(), dst->h};

	for (auto i : str) {		
		SDL_RenderCopy(renderer, i == ' ' ? NULL : (alphabet+int(i - '0'))->texture, NULL, &dst_t);
		dst_t.x += dst_t.w;
	}	
}

void Letters_Scaled_Renderer(string str, SDL_Rect* dst, float percentage) {

	SDL_Rect dst_t;
		
	dst_t.h = int(float(dst->h) * percentage / 100.0f);
	dst_t.y = dst->y + int(dst->h / 2 - dst_t.h / 2);
	dst_t.w = dst_t.h * str.size() * 0.95;
	dst_t.x = dst->x - int((dst_t.w - dst->w) / 2);

	dst_t.w /= str.size();
	
	for (auto i : str) {
		SDL_RenderCopy(renderer, i == ' ' ? NULL : (alphabet + int(i - '0'))->texture, NULL, &dst_t);
		dst_t.x += dst_t.w;
	}
}

void Letters_Padded_Renderer(string str, SDL_Rect* dst, float percentage_padding) {

	if (str.size() > 0) {
		SDL_Rect dst_t;
		float percentage;

		if ((float(dst->w) * (100 - percentage_padding) / 100.0f / (str.size() * 0.95)) <= (dst->h * (100 - percentage_padding) / 100.0f)) {
			dst_t.h = float(dst->w) * (100 - percentage_padding) / 100.0f / (str.size() * 0.95);
		} else {
			dst_t.h = dst->h * (100 - percentage_padding) / 100.0f;
		}			

		dst_t.y = dst->y + int(dst->h / 2 - dst_t.h / 2);
		dst_t.w = dst_t.h * str.size() * 0.95;
		dst_t.x = dst->x - int((dst_t.w - dst->w) / 2);

		dst_t.w /= str.size();

		for (auto i : str) {
			

			SDL_RenderCopy(renderer, i == ' ' ? NULL : (alphabet + int(i - '0'))->texture, NULL, &dst_t);
			dst_t.x += dst_t.w;
		}
	}	
}

