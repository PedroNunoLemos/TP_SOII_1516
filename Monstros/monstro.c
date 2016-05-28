//
//#include "../Controlador/monstro.h"
//#include "../Controlador/coordenada.h"
//#include "monstro.h"
//
//#define QUEUE_SIZE 5
//
//
//
//
//HANDLE hMapFile;
//HANDLE moveMutex;
//HANDLE hThread;
//sharedMemory* pBuf;
//
//Monstro me;
//int tamx;
//int tamy;
//int nSpaces;
//TCHAR processName[kBufferSize]; //Monster Process
//STARTUPINFO si;
//PROCESS_INFORMATION pi;
//
//
//
//typedef struct Order Order;
//struct Order {
//
//	LPSYSTEMTIME initialTime;
//	int cmd; //1-left 2-up 3-right 4-down
//};
//
//Order queue[QUEUE_SIZE];
//
//void openMappedMemory() {
//
//	hMapFile = OpenFileMapping(
//		FILE_MAP_ALL_ACCESS,   // read/write access
//		FALSE,                 // do not inherit the name
//		szName);               // name of mapping object
//
//	if (hMapFile == NULL)
//	{
//		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
//			GetLastError());
//		return NULL;
//	}
//
//	pBuf = (sharedMemory*)MapViewOfFile(hMapFile, // handle to map object
//		FILE_MAP_ALL_ACCESS,  // read/write permission
//		0,
//		0,
//		0); //all file is viewed
//
//	if (pBuf == NULL)
//	{
//		//_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
//
//		CloseHandle(hMapFile);
//
//		return NULL;
//	}
//}
//
//int validaJogadorposicao() {
//
//	Coordenada center;
//
//	center.y = me.coord.y - (MONSTER_VISION_RANGE / 2);
//	center.x = me.coord.x - (MONSTER_VISION_RANGE / 2);
//
//	for (int i = 0; i < MONSTER_VISION_RANGE; i++) {
//		for (int j = 0; j < MONSTER_VISION_RANGE; j++) {
//			if ((pBuf->map[center.y + i][center.x + j] == 1 ||
//				pBuf->map[center.y + i][center.x + j] == 4 || 
//				pBuf->map[center.y + i][center.x + j] == kMonsterDistractedAndJogador ||
//				pBuf->map[center.y + i][center.x + j] == kMonsterBullyAndJogador)
//				&& center.y + i < tamx && center.x + j < tamy) 
//			{
//				if (center.x + j < me.coord.x) {
//					return 1; //go left 
//				}
//				else if (center.y + i < me.coord.y) {
//					return 2; //go up
//				}
//				else if (center.x + j > me.coord.x) {
//					return 3; //go rigth
//				}
//				else {
//					return 4; //do down 
//				}
//			}
//		}
//	}
//
//	return 0;
//}
//
//void startBully() { //the same as distracted but if it sees he goes after the Jogador
//
//
//	int r = rand() % 100;
//	int before = 0;
//	int Jogadorposicao;
//
//	while (1) {
//
//		Jogadorposicao = validaJogadorposicao();
//
//		if (Jogadorposicao > 0) {
//
//			if (Jogadorposicao == 1) { // go left
//				pBuf->map[me.coord.y][me.coord.x] = before;
//				before = pBuf->map[me.coord.y][me.coord.x - 1];
//				if (before == kJogador) { //Jogador
//					pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterBullyAndJogador;
//				}
//				else {
//					if (me.type == 0) {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistracted; // 5 - Monster
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterBully; // 5 - Monster 
//					}
//				}
//				//update position on the variable
//				pBuf->monsters[me.monster_id].coord.x--;
//				me.coord.x--;
//			}
//			else if (Jogadorposicao == 2) { //go up
//				pBuf->map[me.coord.y][me.coord.x] = before;
//				before = pBuf->map[me.coord.y - 1][me.coord.x];
//				if (before == 4) { //Jogador
//					pBuf->map[me.coord.y - 1][me.coord.x] = kMonsterBullyAndJogador;
//				}
//				else {
//					if (me.type == 0) {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistracted; // 5 - Monster
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterBully; // 5 - Monster 
//					}
//				}
//				//update position on the variable
//				pBuf->monsters[me.monster_id].coord.y--;
//				me.coord.y--;
//			}
//			else if (Jogadorposicao == 3) { //go rigth
//				pBuf->map[me.coord.y][me.coord.x] = before;
//				before = pBuf->map[me.coord.y][me.coord.x + 1];
//				if (before == 4) { //Jogador
//					pBuf->map[me.coord.y][me.coord.x + 1] = kMonsterBullyAndJogador;
//				}
//				else {
//					if (me.type == 0) {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistracted; // 5 - Monster
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterBully; // 5 - Monster 
//					}
//				}
//				//update position on the variable
//				pBuf->monsters[me.monster_id].coord.x++;
//				me.coord.x++;
//			}
//			else if (Jogadorposicao == 4) { // do down
//				pBuf->map[me.coord.y][me.coord.x] = before;
//				before = pBuf->map[me.coord.y + 1][me.coord.x];
//				if (before == 4) { //Jogador
//					pBuf->map[me.coord.y + 1][me.coord.x] = kMonsterBullyAndJogador;
//				}
//				else {
//					if (me.type == 0) {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistracted; // 5 - Monster
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterBully; // 5 - Monster 
//					}
//				}
//				//update position on the variable
//				pBuf->monsters[me.monster_id].coord.y++;
//				me.coord.y++;
//			}
//		}
//
//		else {
//
//			if ((rand() % 100) < 75) {
//				r = rand() % 100;
//			}
//
//			for (int i = 0; i < N; i++) {
//				WaitForSingleObject(moveMutex, INFINITE);
//				if (r < 25) {
//					if (pBuf->map[me.coord.y][me.coord.x - 1] != 1 && pBuf->map[me.coord.y][me.coord.x - 1] != 2 && me.coord.y >= 0 && me.coord.x - 1 >= 0 && me.coord.y < tamx && me.coord.x - 1 < tamy) { //left
//						pBuf->map[me.coord.y][me.coord.x] = before;
//						before = pBuf->map[me.coord.y][me.coord.x - 1];
//						if (before == 4) { //Jogador
//							pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistractedAndJogador;
//						}
//						else {
//							pBuf->map[me.coord.y][me.coord.x - 1] = 5; // 5 - Monster //falta colocar os 7
//						}
//						//update position on the variable
//						pBuf->monsters[me.monster_id].coord.x--;
//						me.coord.x--;
//					}
//				}
//				else if (r >= 25 && r < 50) {
//					if (pBuf->map[me.coord.y - 1][me.coord.x] != 1 && pBuf->map[me.coord.y - 1][me.coord.x] != 2 && me.coord.y - 1 >= 0 && me.coord.x >= 0 && me.coord.y - 1 < tamx && me.coord.x < tamy) { //up
//						pBuf->map[me.coord.y][me.coord.x] = before;
//						before = pBuf->map[me.coord.y - 1][me.coord.x];
//						if (before == 4) { //Jogador
//							pBuf->map[me.coord.y - 1][me.coord.x] = kMonsterDistractedAndJogador;
//						}
//						else {
//							pBuf->map[me.coord.y - 1][me.coord.x] = 5; // 5 - Monster //falta colocar os 7
//						}
//						//update position on the variable
//						pBuf->monsters[me.monster_id].coord.y--;
//						me.coord.y--;
//					}
//				}
//				else if (r >= 50 && r < 75) {
//					if (pBuf->map[me.coord.y][me.coord.x + 1] != 1 && pBuf->map[me.coord.y][me.coord.x + 1] != 2 && me.coord.y >= 0 && me.coord.x + 1 >= 0 && me.coord.y < tamx && me.coord.x + 1 < tamy) { //rigth
//						pBuf->map[me.coord.y][me.coord.x] = before;
//						before = pBuf->map[me.coord.y][me.coord.x + 1];
//						if (before == 4) { //Jogador
//							pBuf->map[me.coord.y][me.coord.x + 1] = kMonsterDistractedAndJogador;
//						}
//						else {
//							pBuf->map[me.coord.y][me.coord.x + 1] = 5; // 5 - Monster //falta colocar os 7
//						}
//						//update position on the variable
//						pBuf->monsters[me.monster_id].coord.x++;
//						me.coord.x++;
//					}
//				}
//				else {
//					if (pBuf->map[me.coord.y + 1][me.coord.x] != 1 && pBuf->map[me.coord.y + 1][me.coord.x] != 2 && me.coord.y + 1 >= 0 && me.coord.x >= 0 && me.coord.y + 1 < tamx && me.coord.x < tamy) { //down
//						pBuf->map[me.coord.y][me.coord.x] = before;
//						before = pBuf->map[me.coord.y + 1][me.coord.x];
//						if (before == 4) { //Jogador
//							pBuf->map[me.coord.y + 1][me.coord.x] = kMonsterDistractedAndJogador;
//						}
//						else {
//							pBuf->map[me.coord.y + 1][me.coord.x] = 5; // 5 - Monster //falta colocar os 7
//						}
//						//update position on the variable
//						pBuf->monsters[me.monster_id].coord.y++;
//						me.coord.y++;
//					}
//				}
//
//				ReleaseMutex(moveMutex);
//				Sleep(1 / 15 * me.speed * 1000);
//			}
//
//		}
//	}
//}
//
//void startDistracted() {
//
//
//	int r = rand() % 100;
//	int before = 0;
//
//	while (1) {
//
//		if ((rand() % 100) < 75) {
//			r = rand() % 100;
//		}
//
//		for (int i = 0; i < N; i++) {
//			WaitForSingleObject(moveMutex, INFINITE);
//			if (r < 25) {
//				if (pBuf->map[me.coord.y][me.coord.x - 1] != 1 && pBuf->map[me.coord.y][me.coord.x - 1] != 2 && me.coord.y >= 0 && me.coord.x - 1 >= 0 && me.coord.y < tamx && me.coord.x - 1 < tamy) { //left
//					pBuf->map[me.coord.y][me.coord.x] = before;
//					before = pBuf->map[me.coord.y][me.coord.x - 1];
//					if (before == 4) { //Jogador
//						pBuf->map[me.coord.y][me.coord.x - 1] = kMonsterDistractedAndJogador;
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x - 1] = 5; // 5 - Monster //falta colocar os 7
//					}
//					//update position on the variable
//					pBuf->monsters[me.monster_id].coord.x--;
//					me.coord.x--;
//				}
//			}
//			else if (r >= 25 && r < 50) {
//				if (pBuf->map[me.coord.y - 1][me.coord.x] != 1 && pBuf->map[me.coord.y - 1][me.coord.x] != 2 && me.coord.y - 1 >= 0 && me.coord.x >= 0 && me.coord.y - 1 < tamx && me.coord.x < tamy) { //up
//					pBuf->map[me.coord.y][me.coord.x] = before;
//					before = pBuf->map[me.coord.y - 1][me.coord.x];
//					if (before == 4) { //Jogador
//						pBuf->map[me.coord.y - 1][me.coord.x] = kMonsterDistractedAndJogador;
//					}
//					else {
//						pBuf->map[me.coord.y - 1][me.coord.x] = 5; // 5 - Monster //falta colocar os 7
//					}
//					//update position on the variable
//					pBuf->monsters[me.monster_id].coord.y--;
//					me.coord.y--;
//				}
//			}
//			else if (r >= 50 && r < 75) {
//				if (pBuf->map[me.coord.y][me.coord.x + 1] != 1 && pBuf->map[me.coord.y][me.coord.x + 1] != 2 && me.coord.y >= 0 && me.coord.x + 1 >= 0 && me.coord.y < tamx && me.coord.x + 1 < tamy) { //rigth
//					pBuf->map[me.coord.y][me.coord.x] = before;
//					before = pBuf->map[me.coord.y][me.coord.x + 1];
//					if (before == 4) { //Jogador
//						pBuf->map[me.coord.y][me.coord.x + 1] = kMonsterDistractedAndJogador;
//					}
//					else {
//						pBuf->map[me.coord.y][me.coord.x + 1] = 5; // 5 - Monster //falta colocar os 7
//					}
//					//update position on the variable
//					pBuf->monsters[me.monster_id].coord.x++;
//					me.coord.x++;
//				}
//			}
//			else {
//				if (pBuf->map[me.coord.y + 1][me.coord.x] != 1 && pBuf->map[me.coord.y + 1][me.coord.x] != 2 && me.coord.y + 1 >= 0 && me.coord.x >= 0 && me.coord.y + 1 < tamx && me.coord.x < tamy) { //down
//					pBuf->map[me.coord.y][me.coord.x] = before;
//					before = pBuf->map[me.coord.y + 1][me.coord.x];
//					if (before == 4) { //Jogador
//						pBuf->map[me.coord.y + 1][me.coord.x] = kMonsterDistractedAndJogador;
//					}
//					else {
//						pBuf->map[me.coord.y + 1][me.coord.x] = 5; // 5 - Monster //falta colocar os 7
//					}
//					//update position on the variable
//					pBuf->monsters[me.monster_id].coord.y++;
//					me.coord.y++;
//				}
//			}
//
//			ReleaseMutex(moveMutex);
//			Sleep(1 / 15 * me.speed * 1000);
//		}
//	}
//
//}
//
//DWORD WINAPI validaPosition(LPVOID param)
//{
//	Jogador* sch;
//	Coordenada newMonster;
//	while (1) {
//		if (pBuf->map[me.coord.y][me.coord.x] == kMonsterBullyAndJogador) {
//			sch = pBuf->Jogadors;
//			for (; sch != &pBuf->Jogadors[kMaximumJogadors + 1];) {
//				if (sch->coord.y == me.coord.y && sch->coord.x == me.coord.x) {
//					sch->health--;
//				}
//			}
//		}
//		if (me.health >= (duplicateMonster * MONSTER_DISTRACTED_HEALTH) && me.type == 0) {
//
//			newMonster.y = me.coord.y;
//			if (pBuf->map[me.coord.y][me.coord.x + 1] != 1 && pBuf->map[me.coord.y][me.coord.x + 1] != 2) {
//				newMonster.x = me.coord.x + 1;
//			}
//			else {
//				newMonster.x = me.coord.x - 1;
//			}
//			me.health *= .8;
//
//			_stprintf_s(processName, kBufferSize, TEXT("%s %d %d %d %d %d %d %d"), TEXT("Monster"), me.type, ROWS, COLUMNS, newMonster.y, newMonster.x, kMaximumEnemies, me.health);
//
//			ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
//			si.cb = sizeof(STARTUPINFO);
//
//			CreateProcess(NULL, processName, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
//		}
//		else if (me.health >= (duplicateMonster * MONSTER_BULLY_HEALTH) && me.type == 1) {
//			newMonster.y = me.coord.y;
//			if (pBuf->map[me.coord.y][me.coord.x + 1] != 1 && pBuf->map[me.coord.y][me.coord.x + 1] != 2) {
//				newMonster.x = me.coord.x + 1;
//			}
//			else {
//				newMonster.x = me.coord.x - 1;
//			}
//			me.health *= .8;
//			_stprintf_s(processName, kBufferSize, TEXT("%s %d %d %d %d %d %d %d"), TEXT("Monster"), me.type, ROWS, COLUMNS, newMonster.y, newMonster.x, kMaximumEnemies, me.health);
//
//			ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
//			si.cb = sizeof(STARTUPINFO);
//
//			CreateProcess(NULL, processName, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
//		}
//
//		Sleep(1 / 15 * me.speed * 1000);
//	}
//
//
//}
//
////Method of use: Monster typeofMonster tamx nCOLUMS --- (1-Distraido / 2-Bully)
//int _tmain(int argc, LPTSTR argv[]) {
//
//
//	if (argc != 8) {
//		return -1;
//	}
//
//	srand(time(NULL));
//
//	me.type = _ttoi(argv[1]);
//	tamx = _ttoi(argv[2]);
//	tamy = _ttoi(argv[3]);
//	me.coord.y = _ttoi(argv[4]);
//	me.coord.x = _ttoi(argv[5]);
//	me.monster_id = _ttoi(argv[6]);
//	if (me.type == 0) {
//		_tcscpy(me.name, TEXT("Distracted"));
//		me.speed = MONSTER_DISTRACTED_SPEED;
//	}
//	else {
//		_tcscpy(me.name, TEXT("Bully"));
//		me.speed = MONSTER_BULLY_SPEED;
//	}
//	me.health = _ttoi(argv[7]);
//
//
//
//
//	moveMutex = CreateMutex(
//		NULL,
//		FALSE,
//		TEXT("moveMutex"));
//
//	if (moveMutex == NULL) {
//		_tprintf(TEXT("CreateMutex error: %d\n", getlastError()));
//		return 1;
//	}
//
//	openMappedMemory();
//
//
//	if (pBuf == NULL) {
//		_tprintf(TEXT("Error accessing mapped memory\n"));
//		return -1;
//	}
//
//	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)validaPosition, (LPVOID)NULL, 0, NULL);
//	if (me.type == 0) {
//		startDistracted();
//	}
//	else {
//		startBully();
//	}
//
//
//	UnmapViewOfFile(pBuf);
//
//	CloseHandle(hMapFile);
//
//
//	return 0;
//}
