#pragma once

#define Tipo_Vitamina 1
#define Tipo_OrangeBull 2
#define Tipo_Cafeina 3
#define Tipo_Pedra 4

typedef struct _item {

	int tipo;
	
} Item;



//typedef Item Vitamina;
//typedef Item OrangeBull;
//typedef Item Pedra;
//typedef Item Cafeina;

typedef struct _objectomapa {

	Item objecto[5];

} ObjectoMapa;
