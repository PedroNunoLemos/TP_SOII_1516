
#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "..\Controlador\uteis.h"

void CriaLabirinto(Labirinto *lab, int tamx, int tamy, int salas){
	
	
	int tamsalaw=10;
	int tamsal=10;
	int i=0;
	int r = 0;
	int numSala=0;
	int ok=0;
	int  corr_ajustar=10;
	int cnt=0;
	int x=0;
	int y=0;
	
	
	Sala *salasLigadas;
	salasLigadas=malloc(sizeof(Sala));
	
	lab=malloc(sizeof(Labirinto));
	lab->tamx=tamx;
	lab->tamy=tamy;
	
	lab->tamsalas=salas;
	
	// cria salas no labirinto
	for (i = 0 ; i < salas; i++){
   		ok= false;
   		
   		//cria sala aleatoria e procura espaço vazio no labirinto para colocar a sala
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
        		
        	// valida outras salas no labirinto
        
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
		        lab->salas[i]=&sala;	
			
	    } //fim criação e colocação de sala
		
	
	} // fim criação de salas	

	//liga  as salas com corredores e coloca no labirinto
	
	
	for (i = 0; i < salas; i++)
	{
   		Sala *salaA = lab->salas[i];
       		
       		numSala = i;
    
       		while(numSala == i){
           		numSala = floor((aleatorio(1,100,i+1)/100)*lab->tamsalas);
       		}
    
       		 Sala *salaB = lab->salas[numSala];
    
   	//Aumentar o numero aumenta o corredor
   	//Diminuir o numero encolhe o corredor
   	
   	 corr_ajustar  = 10;
   	 
       	Coordenada *pontoA;
       	
       	pontoA=malloc(sizeof(Coordenada));
       	
       	pontoA->x = floor((aleatorio(1,100,i+1)/100)*salaA->w)+salaA->x;
       	pontoA->y = floor((aleatorio(1,100,i+1)/100)*salaA->h)+salaA->y;
       	
       	Coordenada *pontoB;
       	
       	pontoB=malloc(sizeof(Coordenada));
       	
       	pontoB->x = floor((aleatorio(1,100,i+1)/100)*salaB->w)+salaB->x;
       	pontoB->y = floor((aleatorio(1,100,i+1)/100)*salaB->h)+salaB->y;

   	//Algoritimo drunken/lazy walk   
    	
    	cnt=0;	
    	
    	while (pontoB->x !== pontoA->x || pontoB.y !== pontoA.y){
        	
        	int num = aleatorio(1,100,i+1));
      
        	if (num < corr_ajustar){
        		
            		if (pontoB->x !== pontoA->x){
            			
                		if(pontoB->x > pontoA->x){
                			pontoB->x--;
                		}else{
                			pontoB->x++;
                		}
            			
            		}
        		
        	}else if(pontoB->y !== pontoA->y){
        		
        		if(pontoB->y > pontoA->y){
        			pontoB->y--;
        		}else{
        			pontoB->y++;
        		}
        		
        	}
        
        	//preenche ponteiro de salas ligadas
		if(pontoB->x < tamx && pontoB->y < tamy){
            	
            		cnt++;
            		
            		salasLigadas=realloc(salasLigadas,sizeof(Sala)*cnt);
            		salasLigadas[cnt]->pontoB.x;
            		salasLigadas[cnt]->pontoB.y;
			
		}// fim ponteiro salas ligadas
		
    	}// fim lazy walk
    	
    	
    	free(pontoA);
    	free(pontoB);
    	
	} // Fim ligação das Salas (i)

	// preenche as salas no labirinto
	for(i = 0; i < salas; i++){
    		for(y = lab->salas[i].y; y < lab->salas[i].y + lab->salas[i].h; y++){
        		for( x = lab->salas[i].x; x < lab->salas[i].x + lab->salas[i].w; x++){
            		//console.log(y + " : " + x);
            			lab->celula[y][x] = 1;
        		}        
    		}
	} // fim preechimento salas labirinto

}
