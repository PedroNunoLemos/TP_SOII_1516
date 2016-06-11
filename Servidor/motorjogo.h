#pragma once

void forcaDadosServidor(JogoServidor *serv, JogoCliente* jogcl);

int validaMovimentoJogador(JogoServidor *serv, JogoCliente *jog, int nx, int ny);

void atualizaMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1);

void atualizaJogadorCliente(JogoServidor *serv, JogoCliente* jogcl);

void atualizaMapaEntreClientes(JogoCliente *origem, JogoCliente *dest);

void atualizaPosicao(JogoServidor *serv, JogoCliente *jogcl, int px, int py);

void criaJogo(JogoServidor *jog);

void atualizaMapaServidor(JogoServidor *serv, JogoCliente *jogcl, int oldx, int oldy);

void criaJogador(JogoServidor *jogo, JogoCliente *clt);

void criaObjectosMapa(JogoServidor *serv);

void criaMonstrosIniciais(JogoServidor *serv);

