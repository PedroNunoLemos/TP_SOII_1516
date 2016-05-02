#pragma once

#include <windows.h>

void SetWindow(int Width, int Height);
void GoToXY(int column, int line);
void setcolor(WORD color);
void setForeGroundAndBackGroundColor(int ForeGroundColor,int BackGroundColor);
void clearBox();
void cursorVisible(int st);
