#include "Vars.h"
#include "Classes.h"

int main(int argc, char* argv[]) {
	
	Init();
	
	RESTART:

	restart = false;
	lose = false;
	
	Playing_field* field = new Playing_field;	
	field->Field_Render(true);

	LTimer fpsTimer;
	LTimer capTimer;
		
	int countedFrames = 0;
	fpsTimer.start();

	SDL_GameController* game_controller = nullptr;
	game_controller = SDL_GameControllerOpen(0);
	if (game_controller == nullptr)
		cout << endl << "Controller load error: " << SDL_GetError();

	//SDL_JoystickEventState(SDL_ENABLE);
	

	SDL_Event e;	
		
	while (!quit && !lose) {
		capTimer.start();
		
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 1000) {
			avgFPS = 0;
		}

		field->Field_Render(true);

		if (keyboard) {			
			while (SDL_PollEvent(&e) != 0) {
				if (e.cdevice.type == SDL_CONTROLLERDEVICEADDED)
					keyboard = false;				
				if (e.cbutton.type == SDL_CONTROLLERBUTTONUP)
					keyboard = false;
				
				if (e.type == SDL_QUIT) 
					quit = true;
				
				if (e.key.type == SDL_KEYUP)
					if (e.key.keysym.sym == SDLK_BACKSPACE) {
							Window_Fullscreen();
							break;
					}
			
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					SCREEN_WIDTH = e.window.data1;
					SCREEN_HEIGHT = e.window.data2;

					field->Window_update();
					field->Field_Render(true);

					break;
				}
			
				if (e.button.type == SDL_MOUSEBUTTONUP) {				
					switch (e.button.button) {
					case SDL_BUTTON_LEFT: {
						field->Left_Click();
						break;
					}
					case SDL_BUTTON_RIGHT: {
						field->Right_Click();
						break;
					}
					}				
				}
			}
		} else {
			while (SDL_PollEvent(&e) != 0) {
				if (e.key.type == SDL_KEYUP)
					keyboard = true;
				
				if (e.type == SDL_QUIT) 
					quit = true;				

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

	Close();

	return 0;
}