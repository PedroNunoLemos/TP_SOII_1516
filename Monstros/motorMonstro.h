#pragma once

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\memoria.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\uteis.h"

int criaMonstro(JogoServidor *serv, int tipo, int energia, int n);
void AtualizaMapaMonstro(JogoServidor *jogo, int mid, int ox, int oy);