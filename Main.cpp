#include "Vars.h"
#include "Classes.h"

int main(int argc, char* argv[]) {
	
	Init();
	
	Playing_field field;
	field.Field_Render();

	system("pause");

	Close();

	return 0;
}