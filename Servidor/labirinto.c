
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
	
	Sala *salasLigadas;
	
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
	
	
	for (i = 0; i < connectionCount; i++)
	{
   		var roomA = roomList[i],
       		roomNum = i;
    
       		while(roomNum == i){
           		roomNum = ~~(Math.random()*roomList.length);
       		}
    
       		roomB = roomList[roomNum];
    
   	//Increasing this number will make hallways straighter
   	//Decreasing this number will make halways skewer
   	var sidestepChance = 10,
       		pointA = {x : ~~(Math.random()*roomA.w) + roomA.x,
                	 y : ~~(Math.random()*roomA.h) + roomA.y},
                	 
       		pointB = {x : ~~(Math.random()*roomB.w) + roomB.x,
       			y : ~~(Math.random()*roomB.h) + roomB.y};

   	//This is a type of drunken/lazy walk algorithm    
    		
    		while (pointB.x !== pointA.x || pointB.y !== pointA.y){
        	
        	var num = Math.random()*100;
      
        	if (num < sidestepChance){
        		
            		if (pointB.x !== pointA.x){
            			
                		if(pointB.x > pointA.x){
                			pointB.x--;
                		}else{
                			pointB.x++;
                		}
            			
            		}
        		
        	}else if(pointB.y !== pointA.y){
        		
        		if(pointB.y > pointA.y){
        			pointB.y--;
        		}else{
        			pointB.y++;
        		}
        		
        	}
        
		if(pointB.x < width && pointB.y < height){
            		connectedCells.push({x:pointB.x, y:pointB.y});
			
		}
    	}// fim lazy walk
	} // Fim ligação de Salas

	
}
