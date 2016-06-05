#pragma once


void atualizaMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1);

void criaJogo(JogoServidor *jog);

void atualizaJogadorServidor(JogoServidor *jog, JogoCliente *jogcl);
void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy);

void criaJogador(JogoServidor *jogo, JogoCliente *clt);

void criaObjectosMapa(JogoServidor *serv);
