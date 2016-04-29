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


	

Your thinking is correct. At the end of the day, it would probably look like this:

void Keypressed()
{
    if (kbhit())
    {
      keypress =getch();
        switch (keypress )
        {

            case key_F1:
                displayHelp();
            break;

            case key_LEFT:


            case key_RIGHT:

            break;

            case key_UP:
                   PlayersPad.y-=3;if (PlayersPad.y<0) PlayersPad.y=0;
            break;

            case key_DOWN:
                   PlayersPad.y+=3;if (PlayersPad.y>18) PlayersPad.y=18;
            break;

            case key_ENTER:
            break;

            case key_SPACE:
            break;

            case key_TAB:
                autoPlay=-autoPlay;
                if (autoPlay>0)
                {
                    setcolor(79);gotoxy(62,23);printf("cheat on  " );setcolor(15);
                }
                else
                {
                    setcolor(79);gotoxy(62,23);printf("cheat off  " );setcolor(15);
                }

            break;


        }
    }

}

enter image description here

And as an added bonus, here is code to a Pong game i have written, its a player vs player or player vs computer.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <fstream.h>

#define Black 0
#define Blue 1
#define Green 2
#define Cyan 3
#define Red 4
#define Magenta 5
#define Yellow 6
#define White 7
#define Gray 8
#define LightBlue 9
#define LightGreen 10
#define LightCyan 11
#define LightRed 12
#define LightMagenta 13
#define LightYellow 14
#define BrightWhite 15

#define key_F1 59
#define key_UP 72
#define key_DOWN 80
#define key_LEFT 75
#define key_RIGHT 77
#define key_SPACE 32
#define key_ENTER 13
#define key_ESCAPE 27
#define key_TAB 9
#define key_INSERT 82
#define PONG_WIDTH 78
#define PONG_HEIGHT 22
#define PONG_SCREEN_RIGHT PONG_WIDTH-3
#define PONG_SCREEN_LEFT 5
#define PONG_SCREEN_TOP 2
#define PONG_SCREEN_BOTTOM 22

struct t_ball{ int x,y,headingX,headingY;};
struct t_pad{ int x,y,LEFT,RIGHT;};
int temp,kbChar;
int _key=0;
unsigned long   OldTicksPerSecond=GetTickCount(),NewTicksPerSecond=GetTickCount();
unsigned long   PreferredFramesPerSecond;
unsigned long   DeltaTicksPerSecond;
unsigned int frames=0;
float fps=60.0f;
float Refesh;
float InverseFramesPerSecond;
float OneFramePerSecond;
int keypress=0;

t_ball ball;
t_pad  PlayersPad,computersPad;
int  autoPlay,playersScore,computersScore;


void moveBall();
void runGame();
void initGame();
void Keypressed();
void gameLogic();
void removeBall();
void displayHelp();
void displayYouMissed();
void displayCheatEnabled();
void Render_Game_At_60_Frames_Per_Second();

void getKey();
void gotoxy(int x, int y);
void setcolor(WORD color);
void textColor(unsigned char fColor,unsigned char bColor);
void clrscr();
void delay(int milliseconds);

void txtPlot( unsigned char x, unsigned char y, unsigned char Color);
void txtLine( int xx1, int yy1, int xx2, int yy2 ,int color);
void clrbox(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char bkcol);
void box(unsigned x,unsigned y,unsigned sx,unsigned sy,unsigned char col,unsigned char col2,char text_[]);
void putbox(unsigned x,unsigned y,unsigned sx,unsigned sy,
                 unsigned char col, unsigned char col2,unsigned char bkcol,char text_[]);

int main()
{

    runGame();

  return 0;
}







/*********************************
 * runGame()
 ********************************/

void  runGame()
{
    initGame();
    while (keypress !=key_ESCAPE)
    {
        Keypressed();
        gameLogic();
        moveBall();
        Sleep( 50  );

        removeBall();
        setcolor(31);gotoxy(7,22);printf("ball  X=%d,Y=%d ",ball.x,ball.y );setcolor(15);
        setcolor(31);gotoxy(7,23);printf("pad   X=%d,Y=%d ",PlayersPad.x,PlayersPad.y );setcolor(15);
        setcolor(31);gotoxy(32,22);printf("Your Score:       %d ", playersScore );setcolor(15);
        setcolor(79);gotoxy(32,23);printf("Computers Score:  %d ", computersScore );setcolor(15);
    }

}
/*********************************
 * moveBall()
 ********************************/

void moveBall()
{

    txtPlot( ball.x,ball.y,15);
    txtLine(PlayersPad.x,PlayersPad.y,PlayersPad.x,PlayersPad.y+3,15);
    txtLine(computersPad.x,computersPad.y,computersPad.x,computersPad.y+3,15);


}
 /*********************************
 * removeBall()
 ********************************/

void removeBall()
{
    //Sleep( 9  );
    txtPlot( ball.x,ball.y,0);
    txtLine(PlayersPad.x,PlayersPad.y,PlayersPad.x,PlayersPad.y+3,0);
    txtLine(computersPad.x,computersPad.y,computersPad.x,computersPad.y+3,0);

}
 /*********************************
 * gameLogic()
 ********************************/

void gameLogic()
{
    /* update ball's x location */
    ball.x+=ball.headingX;
    /* update ball's y location */
    ball.y+=ball.headingY;

    /* if ball at most right of screen then reverse ball's x heading */
    if( (ball.x>PONG_SCREEN_RIGHT) )
    {
        ball.headingX=-ball.headingX;
        computersScore+=10;
    }
    /* check if ball's location at top or bottom of screen,if true reverse ball's y heading */
    if( (ball.y<PONG_SCREEN_TOP) || (ball.y>PONG_SCREEN_BOTTOM-2) ) ball.headingY=-ball.headingY;

    PlayersPad.LEFT=PlayersPad.y-3;
    PlayersPad.RIGHT=PlayersPad.y+5;

    /* check if ball lands on pad, if true bounce back */
    if ( (ball.y>= PlayersPad.LEFT) && (ball.y<= PlayersPad.RIGHT) && (ball.x==PlayersPad.x))
    {
        ball.headingX=-ball.headingX;
        playersScore+=10;
    }
    /* let computer track ball's movement */
    if (ball.x>PONG_SCREEN_RIGHT-18) computersPad.y=ball.y;

    /* if cheat enabled,let player track ball's movement */
    if (autoPlay>0)
    {

        if (ball.x<PONG_SCREEN_RIGHT-50) PlayersPad.y=ball.y;
        if (ball.x>PONG_SCREEN_LEFT+16)
        {
            if (PlayersPad.y<10) while (PlayersPad.y<10) PlayersPad.y++;
            if (PlayersPad.y>12) while (PlayersPad.y>10) PlayersPad.y--;
        }

    }
    /* check if ball misses pad, if true display you missed */
    if (ball.x<PONG_SCREEN_LEFT)
    {
        displayYouMissed();
        ball.x=75;
        ball.y=15;
        computersScore+=10;

    }
}

/*********************************
 * initGame()
 ********************************/

void  initGame()
{

    playersScore=0;
    computersScore=0;
    ball.x=15;
    ball.y=5;
    ball.headingX=1;
    ball.headingY=1;

    PlayersPad.x=5;
    PlayersPad.y=12;
    computersPad.x=75;
    computersPad.y=12;


    displayCheatEnabled();

    setcolor(15);
    clrscr();
    txtLine(6,0,74,0,1);
    txtLine(6,22,74,22,1);
    txtLine(6,23,74,23,1);

        autoPlay=1;
    if (autoPlay>0)
    {
        setcolor(79);gotoxy(62,23);printf("cheat on  " );setcolor(15);
    }
    else
    {
        setcolor(79);gotoxy(62,23);printf("cheat off  " );setcolor(15);
    }

    setcolor(31);gotoxy(62,22);printf("<F1> Help" );setcolor(15);
}

void displayHelp()
{
    clrbox(10,8,70,16,79);
    box(10,8,70,16,31,79,"Pong Game, Player vs Computer");

    gotoxy(18,10);cprintf("Controls  ");
    gotoxy(18,11);cprintf("<LEFT>   move pad left");
    gotoxy(18,12);cprintf("<RIGHT> move pad right");
    gotoxy(18,13);cprintf("<TAB>  cheat mode ON/OFF");
    gotoxy(18,15);cprintf("press any key to continue");
    keypress =getch();
    clrbox(10,8,75,21,0);



}

void displayYouMissed()
{
        clrbox(10,8,70,16,79);
        box(10,8,70,16,31,79,"You missed");

        gotoxy(18,10);cprintf("  ");
        gotoxy(18,11);cprintf("The ball has missed the paddle");
        gotoxy(18,12);cprintf("press press space to continue");
        gotoxy(18,13);cprintf("");
        gotoxy(18,14);cprintf("");
        keypress =getch();
        clrbox(10,8,75,21,0);


}

void displayCheatEnabled()
{
        clrbox(10,8,70,16,79);
        box(10,8,70,16,31,79,"Player vs. Computer");
        gotoxy(15,10);cprintf("The Cheat' pad will track  ball movement' is enabled");
        gotoxy(15,11);cprintf("press <TAB> to enable or disable the cheat.");
        gotoxy(15,13);cprintf("press press space to continue");
        gotoxy(18,14);cprintf("");
        gotoxy(18,14);cprintf("");
        keypress =getch();
        clrbox(10,8,75,21,0);


}

/*********************************
 * Keypressed()
 ********************************/

void Keypressed()
{
    if (kbhit())
    {
      keypress =getch();
        switch (keypress )
        {

            case key_F1:
                displayHelp();
            break;

            case key_LEFT:


            case key_RIGHT:

            break;

            case key_UP:
                   PlayersPad.y-=3;if (PlayersPad.y<0) PlayersPad.y=0;
            break;

            case key_DOWN:
                   PlayersPad.y+=3;if (PlayersPad.y>18) PlayersPad.y=18;
            break;

            case key_ENTER:
            break;

            case key_SPACE:
            break;

            case key_TAB:
                autoPlay=-autoPlay;
                if (autoPlay>0)
                {
                    setcolor(79);gotoxy(62,23);printf("cheat on  " );setcolor(15);
                }
                else
                {
                    setcolor(79);gotoxy(62,23);printf("cheat off  " );setcolor(15);
                }

            break;


        }
    }

}

/*********************************
 * Render_Game_At_60_Frames_Per_Second()
 ********************************/

void Render_Game_At_60_Frames_Per_Second()
{
    NewTicksPerSecond=GetTickCount();
    DeltaTicksPerSecond=NewTicksPerSecond-OldTicksPerSecond;
    frames++;

    if(DeltaTicksPerSecond>=PreferredFramesPerSecond)
    {


        OldTicksPerSecond=NewTicksPerSecond;
        InverseFramesPerSecond=1/((float)PreferredFramesPerSecond/1000.0f);
        OneFramePerSecond=(float)frames*InverseFramesPerSecond;
        fps+=OneFramePerSecond;
        fps/=2;
        setcolor(31);gotoxy(70,22);printf(" %d FPS ",DeltaTicksPerSecond );setcolor(15);
        frames=0;
        Refesh=60/fps;
    }
    Sleep(3);
}





        //*****************************************************************************
        //*                                                                           *
        //*****************************************************************************

        void gotoxy(int x, int y)
        {
            COORD coord;
            coord.X = x; coord.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            return;
        }
        //*****************************************************************************
        //*                                                                           *
        //*****************************************************************************

        void setcolor(WORD color)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
            return;
        }


        //
        //     colors:
        //     0 = Black
        //     1 = Blue
        //     2 = Green
        //     3 = Cyan
        //     4 = Red
        //     5 = Magenta
        //     6 = Yellow
        //     7 = LightGray
        //     8 = DarkGray
        //     9 = LightBlue
        //     10 = LightGreen
        //     11 = LightCyan
        //     12 = LightRed
        //     13 = LightMagenta
        //     14 = LightYellow
        //     15 = White


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

