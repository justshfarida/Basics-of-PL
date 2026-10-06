#include <stdio.h>

#define ROWS 5
#define COLS 5

int main() {
    int matrix[ROWS][COLS] = {
        {1,  2,  3,  4,  5},
        {6,  7,  8,  9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    // Equivalent to NumPy matrix[1:4, 1:4]
    int row_start = 1;
    int row_end = 4;
    int col_start = 1;
    int col_end = 4;

    printf("Original matrix:\n");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nSliced matrix:\n");

    for (int i = row_start; i < row_end; i++) {
        for (int j = col_start; j < col_end; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
