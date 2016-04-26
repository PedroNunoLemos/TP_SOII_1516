
#include "labirinto.h"

void drawlabirinto(void ){

int x=0;
int y=0;
int rows=10;
int columns=10;



    for (x = 0; x < rows; x++) 
    { 
        for (y = 0; y <= columns; y++) 
        {
            if (x == 0 || x == rows -1)            
            {
                if (y == 0)  
                printf("+");
                else if (y == columns)
                printf("+\n");
                else
                printf("-");
            }
        
            else
            {
                if (y == 0)  
                printf("|");
                else if (y == columns)
                printf("|\n");
                else
                printf(".");
            }   
        }


}
