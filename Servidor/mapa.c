#include <stdio.h>
#include <string.h>

#include "labirinto.h"
#include "tui.h"
#include "constantes.h"



int main() {

	int j = 0;

	int curx = 0;
	int cury = 0;

	//SetWindow(90, 90);

	Labirinto lab;

	curx = 15;
	cury = 15;

		lab = CriaLabirinto(lab, 80, 80);

	char ch = getch();

	while (ch != key_ESCAPE) {

		ch = getch();

		if (ch == key_DOWN)cury++;
		if (ch == key_UP)cury--;
		if (ch == key_LEFT)curx--;
		if (ch == key_RIGHT)curx++;
		
		imprimeLabirinto(3, 4, curx, cury, 20, lab);
	}




	return 0;
}