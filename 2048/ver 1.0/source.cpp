/*
======================================
Game 2048
Author: Quoc Trong
Email: quoctrong.qb@gmail.com
Date: 15/09/2015
Description: Chuong trinh game 2048
======================================
*/

#include <iostream>
#include <cstdio>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <string>

#define consoleHeight 25
#define consoleWidth 80
#define gameboardWidth 24
#define gameboardHeight 16

using namespace std;

int boardGame[4][4] = { 0 };
int gameScore;
bool replayGame = false;

// Hàm ?n con tr?
void showCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

// L?nh xóa màn hình
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0, 0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}


// di chuy?n con tr? d?n v? trí column, line;
void gotoXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// tr? v? t?a d? x hi?n t?i c?a con trò chu?t
int whereX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}

// tr? v? t?a d? y hi?n t?i c?a con trò chu?t
int whereY()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}

// Ð?i màu text
void TextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void introduction()
{
	TextColor(11);
	printf("\n\n\n");
	printf("   222222222222222         000000000            444444444       888888888     \n"); Sleep(50);
	printf("  2:::::::::::::::22     00:::::::::00         4::::::::4     88:::::::::88   \n"); Sleep(50);
	printf("  2::::::222222:::::2  00:::::::::::::00      4:::::::::4   88:::::::::::::88 \n"); Sleep(50);
	printf("  2222222     2:::::2 0:::::::000:::::::0    4::::44::::4  8::::::88888::::::8\n"); Sleep(50);
	printf("              2:::::2 0::::::0   0::::::0   4::::4 4::::4  8:::::8     8:::::8\n"); Sleep(50);
	printf("              2:::::2 0:::::0     0:::::0  4::::4  4::::4  8:::::8     8:::::8\n"); Sleep(50);
	printf("           2222::::2  0:::::0     0:::::0 4::::4   4::::4   8:::::88888:::::8 \n"); Sleep(50);
	printf("      22222::::::22   0:::::0 000 0:::::04::::444444::::444  8:::::::::::::8  \n"); Sleep(50);
	printf("    22::::::::222     0:::::0 000 0:::::04::::::::::::::::4 8:::::88888:::::8 \n"); Sleep(50);
	printf("   2:::::22222        0:::::0     0:::::04444444444:::::4448:::::8     8:::::8\n"); Sleep(50);
	printf("  2:::::2             0:::::0     0:::::0          4::::4  8:::::8     8:::::8\n"); Sleep(50);
	printf("  2:::::2             0::::::0   0::::::0          4::::4  8:::::8     8:::::8\n"); Sleep(50);
	printf("  2:::::2       2222220:::::::000:::::::0          4::::4  8::::::88888::::::8\n"); Sleep(50);
	printf("  2::::::2222222:::::2 00:::::::::::::00         44::::::44 88:::::::::::::88 \n"); Sleep(50);
	printf("  2::::::::::::::::::2   00:::::::::00           4::::::::4   88:::::::::88   \n"); Sleep(50);
	printf("  22222222222222222222     000000000             4444444444     888888888     \n"); Sleep(50);
	printf("\n"); Sleep(50);
	printf("\n"); Sleep(50);
	TextColor(14);
	gotoXY(50, 24);
	printf("(c) quoctrong - ver 1.6");
	gotoXY(2, 22);
	showCur(true);
	TextColor(13);
	printf("Press any key to start game... ");

	_getch();
}

void drawFrame()
{
	for (int i = 0; i <= gameboardWidth; i++) {
		for (int j = 0; j <= gameboardHeight; j++) {
			if (j % 4 == 0) {
				gotoXY(i + 1, j + 1);
				TextColor(7);
				printf("%c", 219);
			}

			else if (i % 6 == 0) {
				gotoXY(i + 1, j + 1);
				TextColor(7);
				printf("%c", 219);
			}

			else {
				TextColor(3);
				gotoXY(i + 1, j + 1);
				printf("%c", 219);
			}
		}
	}
}


void init_gameBoard()
{
	clrscr();
	drawFrame();

	gameScore = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			boardGame[i][j] = 0;
		}
	}

	srand(time(NULL));
	int newTile[4];

	newTile[0] = rand() % 4;
	newTile[1] = rand() % 4;

	do {
		newTile[2] = rand() % 4;
		newTile[3] = rand() % 4;
	} while (newTile[0] == newTile[2] || newTile[1] == newTile[3]);

	// T?o 2 giá tr? d?u
	boardGame[newTile[0]][newTile[1]] = 2;
	boardGame[newTile[2]][newTile[3]] = 2;
}


// Các hàm c?p nh?t giá tr? m?i.

void goUp()
{
	// Ðua các giá lên trên cùng

	for (int j = 0; j < 4; j++) {
		int t = 0;
		for (int i = 0; i < 4; i++) {
			if (boardGame[i][j] != 0) {
				boardGame[t][j] = boardGame[i][j];
				t++;
			}
		}
		for (int i = t; i < 4; i++) boardGame[i][j] = 0;
	}


	// G?p các giá tr? trùng nhau

	for (int j = 0; j < 4; j++) {
		int t = 0;
		for (int i = 0; i < 4; i++) {
			if (boardGame != 0) {
				if (boardGame[i][j] == boardGame[i + 1][j]) {
					boardGame[t][j] = 2 * boardGame[i][j];
					gameScore += boardGame[t][j];
					t++;
					i++;
				}
				else {
					boardGame[t][j] = boardGame[i][j];
					t++;
				}
			}
		}
		for (int i = t; i < 4; i++) boardGame[i][j] = 0;
	}

}

void goDown()
{
	// Ðua các giá tr? xu?ng du?i

	for (int j = 0; j < 4; j++) {
		int t = 3;
		for (int i = 3; i >= 0; i--) {
			if (boardGame[i][j] != 0) {
				boardGame[t][j] = boardGame[i][j];
				t--;
			}
		}
		for (int i = t; i >= 0; i--) boardGame[i][j] = 0;
	}

	// G?p các giá tr? trùng nhau

	for (int j = 0; j < 4; j++) {
		int t = 3;
		for (int i = 3; i >= 0; i--) {
			if (boardGame != 0) {
				if (boardGame[i][j] == boardGame[i - 1][j]) {
					boardGame[t][j] = 2 * boardGame[i][j];
					gameScore += boardGame[t][j];
					t--;
					i--;
				}
				else {
					boardGame[t][j] = boardGame[i][j];
					t--;
				}
			}
		}
		for (int i = t; i >= 0; i--) boardGame[i][j] = 0;
	}

}

void turnLeft()
{
	// Ðua các giá tr? v? bên trái

	for (int i = 0; i < 4; i++) {
		int t = 0;
		for (int j = 0; j < 4; j++) {
			if (boardGame[i][j] != 0) {
				boardGame[i][t] = boardGame[i][j];
				t++;
			}
		}
		for (int j = t; j < 4; j++) boardGame[i][j] = 0;
	}

	// G?p các giá tr? trùng nhau

	for (int i = 0; i < 4; i++) {
		int t = 0;
		for (int j = 0; j < 4; j++) {
			if (boardGame[i][j] != 0) {
				if (boardGame[i][j] == boardGame[i][j + 1]) {
					boardGame[i][t] = 2 * boardGame[i][j];
					gameScore += boardGame[i][t];
					j++;
					t++;
				}
				else {
					boardGame[i][t] = boardGame[i][j];
					t++;
				}
			}
		}
		for (int j = t; j < 4; j++) boardGame[i][j] = 0;
	}

}

void turnRight()
{
	// Ðua các giá tr? v? bên ph?i

	for (int i = 0; i < 4; i++) {
		int t = 3;
		for (int j = 3; j >= 0; j--) {
			if (boardGame[i][j] != 0) {
				boardGame[i][t] = boardGame[i][j];
				t--;
			}
		}
		for (int j = t; j >= 0; j--) boardGame[i][j] = 0;
	}

	// G?p các giá tr? trùng nhau

	for (int i = 0; i < 4; i++) {
		int t = 3;
		for (int j = 3; j >= 0; j--) {
			if (boardGame[i][j] != 0) {
				if (boardGame[i][j] == boardGame[i][j - 1]) {
					boardGame[i][t] = 2 * boardGame[i][j];
					gameScore += boardGame[i][t];
					j--;
					t--;
				}
				else {
					boardGame[i][t] = boardGame[i][j];
					t--;
				}
			}
		}
		for (int j = t; j >= 0; j--) boardGame[i][j] = 0;
	}
}

void setColor(int value)
{
	switch (value) {
	case 2:		TextColor(48); break;
	case 4:		TextColor(49); break;
	case 8:		TextColor(50); break;
	case 16:	TextColor(52); break;
	case 32:	TextColor(53); break;
	case 64:	TextColor(54); break;
	case 128:	TextColor(55); break;
	case 256:	TextColor(56); break;
	case 512:	TextColor(57); break;
	case 1024:  TextColor(58); break;
	case 2048:  TextColor(59); break;
	case 4096:  TextColor(60); break;
	case 8192:  TextColor(61); break;
	case 16384: TextColor(62); break;
	}
}

// Ghi di?m cao vào file
void write_bestScore(int s)
{
	fstream fileScore;
	fileScore.open("score.txt", ios::out);
	fileScore << s;
	fileScore.close();
}

// Tr? v? di?m cao
int bestScore()
{
	int score;
	fstream fileScore;
	fileScore.open("score.txt", ios::in);
	fileScore >> score;
	fileScore.close();
	return score;
}

// Hi?n th? các giá tr? ra màn hình

void showGame()
{
	if (gameScore > bestScore()) {
		write_bestScore(gameScore);
	}
	gotoXY(35, 5);
	TextColor(11);
	printf("Best Score: %d", bestScore());
	//TextColor();
	
	gotoXY(35, 7);
	TextColor(15);
	printf("Intruction: ");
	gotoXY(35, 8);
	TextColor(10);
	printf("_A: Turn Left");
	gotoXY(35, 9);
	TextColor(9);
	printf("_S: Go Down");
	gotoXY(35, 10);
	TextColor(14);
	printf("_D: Turn Right");
	gotoXY(35, 11);
	TextColor(13);
	printf("_W: Go Up");

	gotoXY(35, 13);
	TextColor(15);
	printf("Press 'r' to replay, 'x' to exit!");

	for (int i = 3; i <= 21; i += 6) {
		for (int j = 3; j <= 15; j += 4) {
			TextColor(51);
			gotoXY(i, j); printf("%c%c%c%c", 219, 219, 219, 219);
			setColor(boardGame[(j - 3) / 4][(i - 3) / 6]);
			if (boardGame[(j - 3) / 4][(i - 3) / 6] == 0) {
				TextColor(51);
				gotoXY(i, j); printf("%c%c%c%c", 219, 219, 219, 219);
			}
			else if (boardGame[(j - 3) / 4][(i - 3) / 6] < 100) {
				gotoXY(i + 1, j); 
				printf("%d", boardGame[(j - 3) / 4][(i - 3) / 6]);
			}
			else {
				gotoXY(i, j);
				printf("%d", boardGame[(j - 3) / 4][(i - 3) / 6]);
			}
			//gotoXY(i + 6, j + 6); printf("%d %d", i, j);
		}
	}
	// Hi?n th? game Score
	gotoXY(35, 3);
	TextColor(11);
	printf("Score: ");
	TextColor(12);
	printf("%d", gameScore);
}

// ki?m tra còn v? trí tr?ng hay không
bool checkZero()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (boardGame[i][j] == 0) {
				return true;
			}
		}
	}
	return false;
}

// T?o m?t s? ng?u nhiên lên các v? trí tr?ng

void addTile()
{
	if (checkZero() == false) return;

	// Sinh 1 s? t?i v? trí x, y;
	int x, y;
	srand(time(NULL));

	do {
		x = rand() % 4;
		y = rand() % 4;
	} while (boardGame[x][y] != 0);

	// T? l? s? 4 / s? 2 = 1 / 10;
	int s = rand() % 100;
	if (s > 89) boardGame[x][y] = 4;
	else boardGame[x][y] = 2;
}

int boardCheck[4][4];

void creCheck()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			boardCheck[i][j] = boardGame[i][j];
		}
	}
}

bool checkMove()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (boardCheck[i][j] != boardGame[i][j]) return true;
		}
	}
	return false;
}

bool checkGameOver()
{
	if (checkZero() == false) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (boardGame[i][j] == boardGame[i][j + 1] || 
					boardGame[i][j] == boardGame[i + 1][j]) return false;
			}
		}
	}
	else if (checkZero() == true) return false;
	return true;
}



int main()
{
	introduction();
	init_gameBoard();

	while (true) {

		if (replayGame == true) {
			init_gameBoard();
			replayGame = false;
		}
		showCur(false);
		showGame();

		if (checkGameOver() == true) {
			gotoXY(35, 16);
			printf("GAME OVER!");
		}

		char key = _getch();
		creCheck();

		if (key == 'w' || key == 'W') {
			goUp();
		}
		else if (key == 's' || key == 'S') {
			goDown();
		}
		else if (key == 'a' || key == 'A') {
			turnLeft();
		}
		else if (key == 'd' || key == 'D') {
			turnRight();
		}
		else if (key == 'r' || key == 'R') {
			replayGame = true;
		}
		else if (key == 'x' || key == 'X') {
			exit(true);
		}
		else continue;

		if (checkMove() == false) continue;
		addTile();
	}

	_getch();

	return 0;
}
