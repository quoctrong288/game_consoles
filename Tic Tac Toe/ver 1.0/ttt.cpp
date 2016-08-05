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
}

void draw() {
   clrscr();
   printf(" %c | %c | %c\n", gameBoard[1][1], gameBoard[1][2], gameBoard[1][3]);
   printf("---+---+---\n");
   printf(" %c | %c | %c\n", gameBoard[2][1], gameBoard[2][2], gameBoard[2][3]);
   printf("---+---+---\n");
   printf(" %c | %c | %c\n\n", gameBoard[3][1], gameBoard[3][2], gameBoard[3][3]);
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
   return ans;
}

void tick(int turn) {
   int pos = 0;
   while (pos < 1 || pos > 9 || gameBoard[(pos-1)/3 + 1][(pos-1)%3 +1] != ' ') {
      printf("\nPlayer %d turn, enter position: ", turn);
      scanf("%d", &pos);
   }
   gameBoard[(pos-1)/3 + 1][(pos-1)%3 +1] = ((turn == 1) ? 'X' : 'O');
}

void AI_turn() {
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
}

void one_player() {
   int cur_turn, pos, play_again = 0;
   printf("First move (you (1) or computer (2) : "); scanf("%d", &cur_turn);
   draw();
   
   while (true) {
      
      ((cur_turn == 1) ? tick(cur_turn) : AI_turn());
      
      draw();
      
      if (check_win(1) != 0) {
         if (cur_turn == 2) printf("Computer won!\nPlay again? (0/1): ");
         else printf("Draw!\nPlay again? (0/1): ");
         
         scanf("%d", &play_again);
         
         if (play_again == 0) return;
         else {
            cur_turn = 3 - cur_turn;
            init_game();
            draw();
         }
      }
      
      else if (is_full()) {
         printf("Draw!\nPlay again? (0/1): ");
         scanf("%d", &play_again);
         
         if (play_again == 0) return;
         else {
            cur_turn = 3 - cur_turn;
            init_game();
            draw();
         }
      }
      cur_turn = 3 - cur_turn;
   }
}

void two_player() {
   int cur_turn, pos, play_again = 0;
   
   printf("First move (1 (X) or 2 (O)) :  "); scanf("%d", cur_turn);
   draw();
   
   while (true) {
      tick(cur_turn);
      draw();
      
      if (check_win(1) != 0) {
         printf("Player %d won!\nPlay again? (0/1): ", cur_turn);
         scanf("%d", &play_again);
         if (play_again == 0) return;
         else {
            cur_turn = 3 - cur_turn;
            init_game();
            draw();
         }
      }
      
      else if (is_full()) {
         printf("Draw!\nPlay again? (0/1): ");
         scanf("%d", &play_again);
         if (play_again == 0) return;
         else {
            cur_turn = 3 - cur_turn;
            init_game();
            draw();
         }
      }
      
      cur_turn = 3 - cur_turn;
   }
}

int main() {
   init_game();
   
   int c;
   while (true) {
      clrscr();
      printf("Choose mode: \n1: One Player \n2: Two Player\n3: Exit\n");
      scanf("%d", &c);
      (c == 3) ? exit(0): ((c == 1) ? one_player() : two_player());
   }
   
   return 0;
}
