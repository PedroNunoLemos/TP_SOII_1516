
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"

void CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas){
	
	
	int tamsalaw=10;
	int tamsalah=10;
	int i=0;
	int ok;
	
	lab=malloc(sizeof(Labirinto));
	lab->tamx=tamx;
	lab->tamy=tamy;
	
	lab->tamsalas=salas;
	
	
	for (i = 0 ; i < room_count; i++){
   		ok= false;
   		
   		while (!ok)
    		{
    			Sala *sala=malloc(sizeof(Sala));
    			Sala->x=aleatorio(1,tamsalaw,)
		
		}
		
	
	}	
	
}
