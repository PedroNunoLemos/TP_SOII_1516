#pragma once


void atualizaMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1);

void criaJogo(JogoServidor *jog);

void atualizaJogadorServidor(JogoServidor *jog, JogoCliente jogcl);

void criaJogador(JogoServidor *jogo, TCHAR nome[], DWORD pid);

void atualizaJogadoresMapaCliente(JogoServidor *serv, JogoCliente *jogcl, int x1, int y1, DWORD pid);