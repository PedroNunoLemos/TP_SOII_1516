
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"

void CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas){
	
	
	int tamsalaw=10;
	int tamsal=10;
	int i=0;
	int r = 0;
	int ok;
	
	
	lab=malloc(sizeof(Labirinto));
	lab->tamx=tamx;
	lab->tamy=tamy;
	
	lab->tamsalas=salas;
	
	
	for (i = 0 ; i < salas; i++){
   		ok= false;
   		
   		while (!ok)
    		{
    			Sala *sala=malloc(sizeof(Sala));
    			
    			sala->x=floor((aleatorio(1,100,i)/100)*lab->tamx);
			sala->y=floor((aleatorio(1,100,i+1)/100)*lab->tamy);
			
			sala->w=floor((aleatorio(1,tamsalaw,i+2)/100)*5);
			sala->h=floor((aleatorio(1,tamsalah,i+3)/100)*5);
			 
			 // check bounds    
        		if(sala.x + sala.w >= width || sala.y + sala.h >= height){
                		continue;
        		}
        		
        		        // check against other rooms
        
        	for(r = 0; r < salas ; r++){
        		
            	if(sala->x > lab->salas[r].x && 
                	sala->x < lab->salas[r]->x + sala->w && 
                	sala->y > lab->salas[r]->y && 
                	sala->y < lab->salas[r]->y + sala->h)
                	{
                		ok = 0;
                		break;
            		}
        	}
		
		        ok = 1;
		        lab->salas[]=&sala;	
			
		}//fim while
		
	
	}	
	
}
