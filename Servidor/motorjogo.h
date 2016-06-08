#pragma once


void atualizaMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1);

void atualizaMapaEntreClientes(JogoCliente *origem, JogoCliente *dest);

void atualizaPosicao(JogoServidor *serv, JogoCliente *jogcl, int px, int py);

void criaJogo(JogoServidor *jog);

void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy);

void criaJogador(JogoServidor *jogo, JogoCliente *clt);

void criaObjectosMapa(JogoServidor *serv);
