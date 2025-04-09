/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

#include <ncurses.h>
#include <stdlib.h>

#define SIZE 3

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Player X
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Player O
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // Grid
}

void draw_board(char board[SIZE][SIZE]) {
    clear();
    mvprintw(0, 0, "Use keys 1-9 to play | 'q' to quit");

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int y = i * 2 + 2;
            int x = j * 4 + 2;

            if (board[i][j] == 'X')
                attron(COLOR_PAIR(1));
            else if (board[i][j] == 'O')
                attron(COLOR_PAIR(2));

            mvprintw(y, x, "%c", board[i][j]);

            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));

            if (j < SIZE - 1) mvprintw(y, x + 1, "|");
        }
        if (i < SIZE - 1) mvprintw(i * 2 + 3, 2, "-----------");
    }
    refresh();
}

int check_winner(char b[SIZE][SIZE], char p) {
    for (int i = 0; i < SIZE; ++i)
        if ((b[i][0] == p && b[i][1] == p && b[i][2] == p) ||
            (b[0][i] == p && b[1][i] == p && b[2][i] == p))
            return 1;

    if ((b[0][0] == p && b[1][1] == p && b[2][2] == p) ||
        (b[0][2] == p && b[1][1] == p && b[2][0] == p))
        return 1;

    return 0;
}

void end_game_message(char winner) {
    if (winner != ' ')
        mvprintw(SIZE * 2 + 4, 0, "🎉 Player %c wins! Press any key to exit.", winner);
    else
        mvprintw(SIZE * 2 + 4, 0, "🤝 It's a draw! Press any key to exit.");
    getch();
}

int main() {
    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int moves = 0;
    char player = 'X';
    int running = 1;

    init_ncurses();

    while (running) {
        draw_board(board);
        mvprintw(SIZE * 2 + 1, 0, "Player %c, choose (1-9): ", player);
        refresh();

        int ch = getch();
        if (ch == 'q') break;

        if (ch >= '1' && ch <= '9') {
            int pos = ch - '1';
            int row = pos / SIZE;
            int col = pos % SIZE;

            if (board[row][col] == ' ') {
                board[row][col] = player;
                moves++;

                if (check_winner(board, player)) {
                    draw_board(board);
                    end_game_message(player);
                    break;
                } else if (moves == SIZE * SIZE) {
                    draw_board(board);
                    end_game_message(' ');
                    break;
                }

                player = (player == 'X') ? 'O' : 'X';
            } else {
                mvprintw(SIZE * 2 + 2, 0, "❗ Cell already taken. Try another one.");
                refresh();
                napms(1000);
            }
        }
    }

    endwin();
    return 0;
}

