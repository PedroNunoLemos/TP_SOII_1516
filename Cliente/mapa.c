#include <stdio.h>
#include <string.h>


#include "tui.h"
#include "..\Controlador\uteis.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\jogo.h"


int main() {

	int j = 0;

	int curx = 0;
	int cury = 0;


	cursorVisible(0);

	char ch = getch();

	while (ch != key_ESCAPE) {

		ch = getch();

		if (ch == key_DOWN)cury++;
		if (ch == key_UP)cury--;
		if (ch == key_LEFT)curx--;
		if (ch == key_RIGHT)curx++;

		//imprimeLabirinto(3, 4, curx, cury, 40, 10, lab);
	}




	return 0;
}
