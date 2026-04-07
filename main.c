#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // <--- 檢查有沒有這行，沒有請補上



#define SIZE 4



int board[SIZE][SIZE];

int score = 0;



// --- 核心邏輯函數 ---



// 在隨機空格產生數字

void add_random_tile() {

    int empty[SIZE * SIZE][2], count = 0;

    for (int i = 0; i < SIZE; i++)

        for (int j = 0; j < SIZE; j++)

            if (board[i][j] == 0) { empty[count][0] = i; empty[count][1] = j; count++; }



    if (count > 0) {

        int r = rand() % count;

        board[empty[r][0]][empty[r][1]] = (rand() % 10 == 0) ? 4 : 2;

    }

}



// 矩陣旋轉 90 度 (順時針)

void rotate_board() {

    int temp[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++)

        for (int j = 0; j < SIZE; j++)

            temp[j][SIZE - 1 - i] = board[i][j];

    for (int i = 0; i < SIZE; i++)

        for (int j = 0; j < SIZE; j++)

            board[i][j] = temp[i][j];

}



// 核心左移邏輯 (包含擠壓與合併)

void move_left() {

    for (int i = 0; i < SIZE; i++) {

        int new_row[SIZE] = {0}, pos = 0;

        // 1. 擠壓非零項

        for (int j = 0; j < SIZE; j++)

            if (board[i][j] != 0) new_row[pos++] = board[i][j];

       

        // 2. 合併相同項

        for (int j = 0; j < pos - 1; j++) {

            if (new_row[j] != 0 && new_row[j] == new_row[j+1]) {

                new_row[j] *= 2;

                score += new_row[j];

                new_row[j+1] = 0;

            }

        }



        // 3. 再次擠壓並放回原陣列

        int final_row[SIZE] = {0};

        int final_pos = 0;

        for (int j = 0; j < SIZE; j++)

            if (new_row[j] != 0) final_row[final_pos++] = new_row[j];

       

        for (int j = 0; j < SIZE; j++) board[i][j] = final_row[j];

    }

}



// 根據輸入方向處理移動

void handle_move(char dir) {

    if (dir == 'a' || dir == 'A') { // 左

        move_left();

    } else if (dir == 'd' || dir == 'D') { // 右 (旋轉180度->左移->旋轉180度)

        rotate_board(); rotate_board(); move_left(); rotate_board(); rotate_board();

    } else if (dir == 'w' || dir == 'W') { // 上 (旋轉270度->左移->旋轉90度)

        rotate_board(); rotate_board(); rotate_board(); move_left(); rotate_board();

    } else if (dir == 's' || dir == 'S') { // 下 (旋轉90度->左移->旋轉270度)

        rotate_board(); move_left(); rotate_board(); rotate_board(); rotate_board();

    } else {

        return; // 無效輸入不動作

    }

    add_random_tile();

}



// --- 顯示函數 ---



void print_ui() {

    system("cls");

    printf("\033[1;36mEE243A Midterm Project: 2048\033[0m\n");

    printf("\033[1;33mScore: %d\033[0m\n", score);

    printf("---------------------\n");

   

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {

            if (board[i][j] == 0) {

                printf(".    ");

            } else {

                // 根據數字大小給予不同顏色

                int val = board[i][j];

                if (val <= 4) printf("\033[0;32m%-5d\033[0m", val);      // 2, 4 綠色

                else if (val <= 64) printf("\033[0;33m%-5d\033[0m", val); // 8-64 黃色

                else if (val <= 512) printf("\033[0;35m%-5d\033[0m", val);// 128-512 紫色

                else printf("\033[0;31m%-5d\033[0m", val);               // 1024以上 紅色

            }

        }

        printf("\n\n");

    }

    printf("---------------------\n");

    printf("\033[1;32mW(上) A(左) S(下) D(右)\033[0m | \033[1;31mQ(退出)\033[0m\n");

    printf("你的下一步: ");

}



int main() {
    srand(time(NULL));
    
    // 初始化棋盤為 0
    for (int i=0; i<SIZE; i++) 
        for (int j=0; j<SIZE; j++) 
            board[i][j] = 0;

    add_random_tile();
    add_random_tile();

    char op; // 宣告用來存按鍵的變數
    
    // --- 下面這段就是你要放的位置 ---
    while (1) {
        print_ui();
        op = _getch(); // 直接偵測按鍵，不用按 Enter
        
        if (op == 'q' || op == 'Q') break;
        
        handle_move(op);
    }
    // ----------------------------

    printf("\n遊戲結束！你的最終分數為: %d\n", score);
    return 0;
}