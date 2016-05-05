
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"

void CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas){
	
	
	int tamsalaw=10;
	int tamsal=10;
	int i=0;
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
    			
    			Sala->x=floor((aleatorio(1,100,i)/100)*lab->tamx);
			Sala->y=floor((aleatorio(1,100,i+1)/100)*lab->tamy);
			
			Sala->w=floor((aleatorio(1,tamsalaw,i+2)/100)*5);
			Sala->h=floor((aleatorio(1,tamsalah,i+3)/100)*5);
			
			
		}
		
	
	}	
	
}
