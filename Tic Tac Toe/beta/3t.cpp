#include <iostream>
#include <Windows.h>

using namespace std;

char gameBoard[5][5];

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

void init_game() {
   for (int i = 1; i <= 3; i++) {
      for (int j = 1; j <= 3; j++) {
         gameBoard[i][j] = ' ';
      }
   }
   //gameBoard[1][1] = 'X';
}

void draw() {
   clrscr();
   printf(" %c | %c | %c\n", gameBoard[1][1], gameBoard[1][2], gameBoard[1][3]);
   printf("---+---+---\n");
   printf(" %c | %c | %c\n", gameBoard[2][1], gameBoard[2][2], gameBoard[2][3]);
   printf("---+---+---\n");
   printf(" %c | %c | %c\n", gameBoard[3][1], gameBoard[3][2], gameBoard[3][3]);
}

int check_win(int depth) {
   int dx[] = {1, 0,-1, 1};
   int dy[] = {0, 1, 1, 1};
   
   for (int i = 1; i <= 3; i++) {
      for (int j = 1; j <= 3; j++) {
         if (gameBoard[i][j] != ' ') {
            for (int k = 0; k <= 3; k++) {
               int cnt = 0, x = i, y = j;
               for(; 1 <= x && x <= 3 && 1 <= y && y <= 3; x += dx[k], y += dy[k]) {
                  if (gameBoard[x][y] == gameBoard[i][j]) cnt++;
                  else break;
               }
               if (cnt == 3) return ((gameBoard[i][j] == 'O') ? (10 - depth) : (depth - 10));
            }
         }
      }
   }
   return 0;
}

bool is_full() {
   for (int i = 1; i <= 3; i++) {
      for (int j = 1; j <= 3; j++) {
         if (gameBoard[i][j] == ' ') return false;
      }
   }
   return true;
}

int find_best(int turn, int depth) {
   if (check_win(depth) != 0) return check_win(depth);
   if (is_full()) return 0;
   
   //try: 
   //if (is_full()) return check_win();
   
   int ans = ((turn == 1) ? -1000 : 1000);
   
   for (int i = 1; i <= 3; i++) {
      for (int j = 1; j <= 3; j++) {
         if (gameBoard[i][j] != ' ') continue;
         if (turn == 1) {
            gameBoard[i][j] = 'O';
            ans = max(ans, find_best(!turn, depth+1));
         }
         else {
            gameBoard[i][j] = 'X';
            ans = min(ans, find_best(!turn, depth+1));
         }
         gameBoard[i][j] = ' ';
      }
   }
   //cout << ans << endl;
   return ans;
}

int main() {
   
   init_game();
   while (true) {
      int res = -1000, best_x, best_y;
      
      for (int i = 1; i <= 3; i++) {
         for (int j = 1; j <= 3; j++) {
            if (gameBoard[i][j] == ' ') {
               gameBoard[i][j] = 'O';
               if (find_best(0, 1) > res) {
                  res = find_best(0, 1);
                  best_x = i;
                  best_y = j;
               }
               gameBoard[i][j] = ' ';
            }
         }
      }
      
      gameBoard[best_x][best_y] = 'O';
      draw();
      
      if (is_full() || (check_win(1) != 0)) {
         printf("Game Over!");
         break;
      }
      
      int x, y;
      while (cin >> x >> y) {
         if (x == 0) {
            init_game();
            break;
         }
         else if (x < 1 || y < 1 || x > 3 || y > 3 || gameBoard[x][y] != ' ') {
            printf("try again!");
         }
         else {
            gameBoard[x][y] = 'X';
            break;
         }
      }
      draw();
      if (is_full() || (check_win(1) != 0)) {
         printf("Game Over!");
         break;
      }
   }
   
   return 0;
}
