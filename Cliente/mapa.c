#include <stdio.h>
#include <string.h>

//#include "labirinto.h"
#include "tui.h"
#include "..\Controlador\uteis.h"



int main() {

	int j = 0;

	int curx = 0;
	int cury = 0;

	aleatorio(1, 2, 1);

	//SetWindow(90, 90);


	//Labirinto lab;

	//curx = 15;
	//cury = 15;
	//lab = CriaLabirinto(lab, 80, 80);

	//cursorVisible(0);

/*	char ch = getch();

	while (ch != key_ESCAPE) {

		ch = getch();

		if (ch == key_DOWN)cury++;
		if (ch == key_UP)cury--;
		if (ch == key_LEFT)curx--;
		if (ch == key_RIGHT)curx++;

		imprimeLabirinto(3, 4, curx, cury, 40, 10, lab);
	}

*/


	return 0;
}
