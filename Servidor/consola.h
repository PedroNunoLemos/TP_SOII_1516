#pragma once

#include <windows.h>

void SetWindow(int Width, int Height);
void GoToXY(int column, int line);
void setcolor(WORD color);
void setForeGroundAndBackGroundColor(int ForeGroundColor,int BackGroundColor);
void clrbox(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char bkcol);
