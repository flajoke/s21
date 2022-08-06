#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 25
#define TIME_SLEEP 100
#define SYMBOL_LIVE_CELL 'x'
#define DELTA_TIME 10
#define EXIT 'q'
#define ADD_SPEED 'a'
#define SLOW_SPEED 'z'


int game(WINDOW *win, int** matrix_old, int** matrix_new, int row, int col);
WINDOW* init_window(int row, int col);
int read_command(WINDOW *win, int* time);
void calc_next_generation(int** matrix_old, int** matrix_new, int row, int col);
int count_number_neighbor(int** matrix_old, int row, int col, int pos_row, int pos_col);
void matrix_exchange(int** matrix_old, int** matrix_new, int row, int col);
int matrix_comparison(int** matrix_old, int** matrix_new, int row, int col);
void clear_matrix(int** matrix, int row, int col);
int input(int **matrix, int n, int m);
int** array_memory_alloc(size_t row, size_t col);
void array_memory_free(int** matrix);
void output(int **matrix, int n, int m, WINDOW * win);


int main() {
    int **matrix_old = array_memory_alloc(HEIGHT, WIDTH);
    int **matrix_new = array_memory_alloc(HEIGHT, WIDTH);

    if (matrix_old != NULL && matrix_new != NULL) {
        if (input(matrix_old, HEIGHT, WIDTH) == 1 && freopen("/dev/tty", "r", stdin)) {
            WINDOW *win = init_window(HEIGHT, WIDTH);
            game(win, matrix_old, matrix_new, HEIGHT, WIDTH);
            endwin();
        }
    }

    if (matrix_old != NULL)
        array_memory_free(matrix_old);

    if (matrix_new != NULL)
        array_memory_free(matrix_new);

    return 0;
}

int game(WINDOW *win, int** matrix_old, int** matrix_new, int row, int col) {
    int time = TIME_SLEEP;
    int generations_same = 0;

    do {
        calc_next_generation(matrix_old, matrix_new, row, col);
        generations_same = matrix_comparison(matrix_old, matrix_new, row, col);
        matrix_exchange(matrix_old, matrix_new, row, col);
        clear_matrix(matrix_new, row, col);
        output(matrix_old, row, col, win);
        wrefresh(win);
        wtimeout(win, time);
    } while (generations_same != 1 && read_command(win, &time) != EXIT);

    return 0;
}

WINDOW* init_window(int row, int col) {
    initscr();
    noecho();
    cbreak();
    refresh();
    WINDOW *win = newwin(row + 2, col + 2, ((LINES - row - 2) / 2), ((COLS - col - 2) / 2));
    wtimeout(win, 0);
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    return win;
}

int read_command(WINDOW *win, int* time) {
    int command = wgetch(win);
    if (command == ADD_SPEED && *time > DELTA_TIME)
        *time -= DELTA_TIME;
    else if (command == SLOW_SPEED)
        *time += DELTA_TIME;
    return command;
}

void matrix_exchange(int** matrix_old, int** matrix_new, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix_old[i][j] = matrix_new[i][j];
        }
    }
}

int matrix_comparison(int** matrix_old, int** matrix_new, int row, int col) {
    int equal = 1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (matrix_old[i][j] != matrix_new[i][j]) {
                equal = 0;
                break;
            }
        }
    }
    return equal;
}

void clear_matrix(int** matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[i][j] = 0;
        }
    }
}

void calc_next_generation(int** matrix_old, int** matrix_new, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int count = count_number_neighbor(matrix_old, row, col, i, j);
            if (count == 3) {
                matrix_new[i][j] = 1;
            } else if (count == 2) {
                matrix_new[i][j] = matrix_old[i][j];
            } else {
                matrix_new[i][j] = 0;
            }
        }
    }
}

int count_number_neighbor(int** matrix_old, int row, int col, int pos_row, int pos_col) {
    int count = 0;
    for (int i = pos_row - 1; i <= pos_row + 1; i++) {
        for (int j = pos_col - 1; j <= pos_col + 1; j++) {
            int ki = i;
            int kj = j;
            if (i == pos_row && j == pos_col) continue;
            if (i < 0) ki = row - 1;
            if (j < 0) kj = col - 1;
            if (i >= row) ki = 0;
            if (j >= col) kj = 0;
            if (matrix_old[ki][kj] == 1) count += 1;
        }
    }

    return count;
}

int input(int **matrix, int n, int m) {
    int status = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                status = 0;
                break;
            }
        }
        if (status == 0) break;
    }

    return status;
}

int** array_memory_alloc(size_t row, size_t col) {
    int** arr =  (int**)malloc(row * sizeof(int*) + row * col * sizeof(int));
    int* start = (int*)(arr + row);

    for (int i = 0; i < (int)row; i++) {
        arr[i] = start + col * i;
    }

    return arr;
}

void array_memory_free(int** matrix) {
    free(matrix);
}

void output(int **matrix, int n, int m, WINDOW * win) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 1)
                mvwprintw(win, i + 1, j + 1, "%c", SYMBOL_LIVE_CELL);
            else
                mvwprintw(win, i + 1, j + 1, " ");
        }
    }
}

