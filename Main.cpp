#include "Vars.h"
#include "Classes.h"

int main(int argc, char* argv[]) {
	
	Init();
	
	Playing_field field;
	field.Field_Render(true);

	LTimer fpsTimer;
	LTimer capTimer;

	int countedFrames = 0;
	fpsTimer.start();

	SDL_Event e;	

	while (!quit && !lose) {
		capTimer.start();
		
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f); 
		if (avgFPS > 1000) { 
			avgFPS = 0; 
		}		

		field.Field_Render(true);

		while (SDL_PollEvent(&e) != 0) {
			if( e.type == SDL_QUIT ) { 
				quit = true; 
			}
			switch (e.window.event)	{
			
			case SDL_WINDOWEVENT_SIZE_CHANGED: {
				
					SCREEN_WIDTH = e.window.data1;
					SCREEN_HEIGHT = e.window.data2;

				field.Window_update();
				field.Field_Render(true);

				break;
			}				
			}
			switch (e.button.type) {
			case SDL_MOUSEBUTTONUP: {
				switch (e.button.button) {
				case SDL_BUTTON_LEFT: {
					field.Left_Click();
					break;
				}
				case SDL_BUTTON_RIGHT: {
					field.Right_Click();
					break;
				}
				}							
			}
			}
		}

		++countedFrames;

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
	if (lose) {
		field.~Playing_field();
	}

	Close();

	return 0;
}