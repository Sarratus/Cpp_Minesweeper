#include "Vars.h"
#include "Classes.h"

int main(int argc, char* argv[]) {
	
	Init();
	
	Menu game;
	
	game.Start_game();

	Close();

	return 0;
}