#include <stdio.h>
#include <conio.h>

#define N 6

char* flag = "YISF{0Hh_4r3_y0U_900d_4t_2048_94m3?}";
void multiply_matrices(int A[N][N], int B[N][N], int C[N][N]) {
    int i, j, k;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_matrix(int matrix[N][N]) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int A[N][N] = {{4, 7, 2, 7, 0, 1},
                   {5, 7, 6, 3, 5, 2},
                   {5, 9, 4, 8, 1, 7},
                   {9, 7, 5, 8, 5, 9},
                   {5, 4, 2, 5, 4, 3},
                   {8, 8, 9, 7, 5, 4}};

    int B[N][N] = {{6, 2, 1, 7, 7, 2},
                   {7, 4, 8, 9, 2, 7},
                   {8, 1, 9, 2, 1, 4},
                   {2, 9, 3, 1, 3, 2},
                   {9, 1, 6, 0, 3, 0},
                   {5, 2, 9, 0, 1, 3}};

    int C[N][N];

    multiply_matrices(A, B, C);

    printf("A:\n");
    print_matrix(A);

    printf("\nB:\n");
    print_matrix(B);

    printf("\nC:\n");
    print_matrix(C);

    getch();
    return 0;
}
