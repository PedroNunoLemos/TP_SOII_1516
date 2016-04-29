#include <Windows.h>
#include "consola.h"

void GoToXY(int column, int line)
{
	// Create a COORD structure and fill in its members.
	// This specifies the new position of the cursor that we will set.
	COORD coord;
	coord.X = column;
	coord.Y = line;

	// Obtain a handle to the console screen buffer.
	// (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
	// in conjunction with the GetStdHandle() to retrieve the handle.)
	// Note that because it is a standard handle, we don't need to close it.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Finally, call the SetConsoleCursorPosition function.
	if (!SetConsoleCursorPosition(hConsole, coord))
	{
		// Uh-oh! The function call failed, so you need to handle the error.
		// You can call GetLastError() to get a more specific error code.
		// ...
	}
}


void setcolor(WORD color)
 {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
            return;
 }


void SetWindow(int Width, int Height)
{
	COORD coord;
	coord.X = Width;
	coord.Y = Height;

	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}

void setForeGroundAndBackGroundColor(int ForeGroundColor,int BackGroundColor)
{
           int color=16*BackGroundColor+ForeGroundColor;
           setcolor(color);
}

void clrbox(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char bkcol)
        {
            int x,y;
            setcolor(bkcol);                       //Set to color bkcol

            for (y=y1;y<y2;y++)                    //Fill Y Region Loop
            {
                for (x=x1;x<x2;x++)               //Fill X region Loop
                {
                  gotoxy(x,y);cprintf(" ");       //Draw Solid space
                }
            }
        }

