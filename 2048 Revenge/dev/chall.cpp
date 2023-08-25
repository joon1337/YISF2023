#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <wchar.h>
#include <time.h>
#include <windows.h>

using namespace std;

#define is_debug 1
#define N 6
#define SIZE 20
#define TARGET 68719476736

int is_first = 1;
typedef struct obj {
    public:
        unsigned long long int n;
        size_t func_addr;
} obj;

class count {
    public:
        int idx;
        obj** obj_arr;
        unsigned long long int max;
        int func_idx[36] = { 0, 16, 21, 22, 30, 1, 11, 32, 34, 6, 28, 12, 25, 33, 29, 9, 4,
        26, 5, 18, 13, 23, 8, 31, 15, 2, 17, 7, 14, 20, 10, 19, 3, 24, 35, 27 };
        size_t func_offset[36] = { 0x7a6a77, 0x3f2ba2, 0x620c09, 0xb99a98, 0x519738, 0xfd1499, 0xaf1834, 0x99b07, 
            0x48081b, 0xb62cf9, 0x4c5f1a, 0x6c6cb9, 0x3cc2ad, 0x246d62, 0xc7ff22, 0xc2e591, 0xc11741, 0x60c58f, 
            0x46a7af, 0x4d38c0, 0x4f6498, 0xd967b9, 0xa4e61a, 0xacecc9, 0xec88e7, 0x35469f, 0x4ac91c, 0x77a9d8, 
            0xb09bb0, 0x233abd, 0xb63c55, 0xcd9ca6, 0x832131, 0xeaa019, 0x763968, 0x997fc6 };

        count() {
            this->obj_arr = (obj**)malloc(sizeof(obj*) * N * N);
            this->idx = 0;
            this->max = 0;
        }
        void add_num(unsigned long long int);
        void update(unsigned long long (*)[20]);
};

void count::add_num(unsigned long long int n) {
    obj* ptr = NULL;
    ptr = (obj*)malloc(sizeof(obj));
    if(!ptr) {
        printf("error\n");
        exit(-1);
    }
    ptr->n = n;
    ptr->func_addr = this->func_idx[this->idx];
    this->obj_arr[(this->idx)++] = ptr;
    this->max = n;
}

void count::update(unsigned long long (*board)[20]) {
    if(this->idx != 36) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if ((board[i][j] > this->max) && (this->max * 2 == board[i][j]) || (is_first && (board[i][j]==2))) {
                    count::add_num(board[i][j]);
                    return;
                }
            }
        }
    }
    return;
}

int table[] = {244, 74, 67, 198, 149, 127, 170, 108, 50, 171, 230, 155, 204, 163, 228, 150, 0, 78, 200, 4, 105, 251, 196, 144, 177, 199, 213, 46, 13, 207, 59, 214, 84, 34, 212, 247, 86, 253, 39, 103, 162, 233, 164, 71, 1, 109, 222, 60, 191, 116, 181, 72, 156, 182, 122, 77, 184, 37, 79, 211, 143, 88, 120, 98, 57, 174, 7, 250, 124, 126, 113, 48, 249, 186, 252, 248, 154, 167, 44, 240, 14, 220, 40, 11, 215, 205, 139, 54, 133, 195, 125, 117, 104, 20, 234, 27, 18, 65, 75, 158, 165, 208, 206, 53, 68, 15, 17, 66, 210, 218, 12, 179, 73, 29, 137, 217, 236, 97, 69, 185, 224, 110, 190, 38, 172, 115, 114, 192, 64, 188, 169, 119, 180, 136, 70, 6, 8, 134, 93, 35, 9, 242, 223, 159, 32, 216, 61, 243, 30, 28, 209, 80, 107, 227, 225, 232, 21, 10, 221, 42, 226, 45, 237, 202, 118, 47, 201, 36, 129, 89, 138, 130, 5, 101, 92, 31, 147, 100, 95, 94, 241, 194, 178, 135, 187, 76, 33, 111, 19, 235, 141, 43, 161, 153, 189, 245, 51, 25, 26, 142, 23, 151, 41, 231, 176, 152, 239, 52, 229, 145, 56, 148, 85, 166, 238, 121, 81, 255, 3, 102, 168, 146, 58, 87, 131, 160, 24, 183, 2, 99, 123, 91, 112, 49, 16, 90, 55, 140, 22, 63, 83, 246, 219, 173, 197, 96, 203, 132, 193, 62, 175, 82, 157, 128, 254, 106}; 
count* count_obj = NULL;

unsigned long long int board[SIZE][SIZE];
int C[N][N] = {0, };
int B[N][N] = {
    {2, 6, 2, 0, 0, 1},
    {9, 4, 2, 1, 0, 0},
    {3, 5, 3, 2, 9, 6},
    {6, 3, 4, 2, 7, 8},
    {0, 1, 3, 3, 7, 7},
    {0, 1, 9, 8, 3, 1}
};

typedef void(*func_ptr)(unsigned long long (*arr)[6]);

void func3(unsigned long long (*arr)[6]) {
    // 테이블 값과 치환
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<36; i++) {
        unsigned char c = table[ptr[i]];
        ptr[i] = c;
    }
}

void func1(unsigned long long (*arr)[6]) {
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<strlen((char*)ptr); i++) {
        ptr[i] = ((unsigned char*)ptr)[i];
    }
}

void func9(unsigned long long (*arr)[6]) {
    unsigned char(*ptr)[6] = (unsigned char(*)[6])arr;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            long long int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += B[i][k] * ptr[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void func4(unsigned long long (*arr)[6]) {
    // B 행렬과 xor
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<6; i++) {
        for(int j=0; j<6; j++) {
            ptr[6*i+j] ^= B[i][j];
        }
    }
}

void func5(unsigned long long (*arr)[6]) {
    // n 번째 바이트와 n+2 번째 바이트의 n+1 번째 비트 변경
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<36; i++) {
        unsigned char bitoffset = (i + 1) % 36;
        unsigned char first = ptr[i % 36];
        unsigned char second = ptr[(i+2) % 36];
        
        int bit1 = (first >> bitoffset) & 1;
        int bit2 = (second >> bitoffset) & 1;
        if (bit1 != bit2) {
            ptr[i % 36] ^= (1 << bitoffset);
            ptr[(i+2) % 36] ^= (1 << bitoffset);
        }
    }
}

void func2(unsigned long long (*arr)[6]) {
    // 짝수 비트 <-> 홀수 비트
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<36; i++) {
        unsigned char c = ptr[i];
        ptr[i] = (((c&0b10101010)>>1) | ((c&0b01010101)<<1));
    }
}

void func8(unsigned long long (*arr)[6]) {
    // 대각선 원소일 때만 역순 정렬
    int temp;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                for (int k = 0; k < i; k++) {
                    temp = B[i][k];
                    B[i][k] = B[i][N - 1 - k];
                    B[i][N - 1 - k] = temp;
                }
            }
        }
    }
}

void func6(unsigned long long (*arr)[6]) {
    // flag 를 전치행렬로 변환
    unsigned char* ptr = (unsigned char*)arr;
    unsigned char temp;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            temp = ptr[i * N + j];
            ptr[i * N + j] = ptr[j * N + i];
            ptr[j * N + i] = temp;
        }
    }
}

void func7(unsigned long long (*arr)[6]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] *= B[i][j];
        }
    }
}

void func10(unsigned long long (*arr)[6]) {}
void func11(unsigned long long (*arr)[6]) {}
void func12(unsigned long long (*arr)[6]) {}
void func13(unsigned long long (*arr)[6]) {}
void func14(unsigned long long (*arr)[6]) {}
void func15(unsigned long long (*arr)[6]) {}
void func16(unsigned long long (*arr)[6]) {}
void func17(unsigned long long (*arr)[6]) {}
void func18(unsigned long long (*arr)[6]) {}
void func19(unsigned long long (*arr)[6]) {}
void func20(unsigned long long (*arr)[6]) {}
void func21(unsigned long long (*arr)[6]) {}
void func22(unsigned long long (*arr)[6]) {}
void func23(unsigned long long (*arr)[6]) {}
void func24(unsigned long long (*arr)[6]) {}
void func25(unsigned long long (*arr)[6]) {}
void func26(unsigned long long (*arr)[6]) {}
void func27(unsigned long long (*arr)[6]) {}
void func28(unsigned long long (*arr)[6]) {}
void func29(unsigned long long (*arr)[6]) {}
void func30(unsigned long long (*arr)[6]) {}
void func31(unsigned long long (*arr)[6]) {}
void func32(unsigned long long (*arr)[6]) {}
void func33(unsigned long long (*arr)[6]) {}
void func34(unsigned long long (*arr)[6]) {}
void func35(unsigned long long (*arr)[6]) {}
void func36(unsigned long long (*arr)[6]) {}

int check_validate() {
    long long int lst[6][6] = {4170, 1752, 7576, 8131, 5112, 8096, 
        16104, 7061, 13860, 17980, 4755, 14877, 
        20727, 8451, 19733, 29389, 17124, 25329, 
        26635, 10210, 18349, 30123, 13720, 24617, 
        16024, 5873, 13277, 19728, 10782, 17628, 26894, 
        25512, 24341, 12939, 28739, 22839
    };

    for(int i=0; i<6; i++) {
        for(int j=0; j<6; j++) {
            if(C[i][j] != lst[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}


void init() __attribute__((constructor));
int main();

func_ptr func_table[36] = {&func1-0x7a6a77, &func6-0xfd1499, &func26-0x35469f, &func33-0x832131, 
&func17-0xc11741, &func19-0x46a7af, &func10-0xb62cf9, &func28-0x77a9d8, 
&func23-0xa4e61a, &func16-0xc2e591, &func31-0xb63c55, &func7-0xaf1834, 
&func12-0x6c6cb9, &func21-0x4f6498, &func29-0xb09bb0, &func25-0xec88e7, 
&func2-0x3f2ba2, &func27-0x4ac91c, &func20-0x4d38c0, &func32-0xcd9ca6, &func30-0x233abd,
 &func3-0x620c09, &func4-0xb99a98, &func22-0xd967b9, &func34-0xeaa019, &func13-0x3cc2ad, 
 &func18-0x60c58f, &func36-0x997fc6, &func11-0x4c5f1a, &func15-0xc7ff22, &func5-0x519738, 
 &func24-0xacecc9, &func8-0x99b07, &func14-0x246d62, &func9-0x48081b, &func35-0x763968};

void init_window() {
    HWND console = GetConsoleWindow();
    int window_width = 920;
    int window_height = 740;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    int window_x = (screen_width - window_width) / 2;
    int window_y = (screen_height - window_height) / 2;

    SetWindowPos(console, NULL, window_x, window_y, window_width, window_height, SWP_SHOWWINDOW);
}

void init_board() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

void add_number() {
    int i, j;
    int count = 0;
    int temp[SIZE * SIZE][2];

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                temp[count][0] = i;
                temp[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int index = rand() % count;
        board[temp[index][0]][temp[index][1]] = 2;
    }
}

void print_board() {
    system("cls");

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("\x1b[35m+----\x1b[0m");
        }
        printf("\x1b[35m+\x1b[0m\n");

        for (int j = 0; j < SIZE; j++) {
            printf("\x1b[35m|\x1b[0m");
            if (board[i][j] != 0) {
                printf("%4llu", board[i][j]);
            } else {
                printf("    ");
            }
        }
        printf("\x1b[35m|\x1b[0m\n");
    }

    for (int j = 0; j < SIZE; j++) {
        printf("\x1b[35m+----\x1b[0m");
    }
    printf("\x1b[35m+\x1b[0m\n");
}

void input_flag() {
    char* flag = NULL;
    if(count_obj->idx != 36) exit(-1);
    
    flag = (char*)malloc(0x40);
    if (!flag) {
        printf("error!");
        exit(-1);
    }
    
    const wchar_t* str = L"[+] input flag\n> ";
    wprintf(str);

    scanf("%s", flag);
    if(strlen(flag) != 36) {
        exit(-1);
    }

    count_obj->idx = 0;
    for(int i=0; i<36; i++) {
        size_t addr = (size_t)func_table[count_obj->func_idx[count_obj->idx]];
        int offset = (count_obj->func_offset)[count_obj->idx];
        ((func_ptr)(addr+offset))((long long unsigned int (*)[6])flag);
        count_obj->idx++;
    }

    if(check_validate()) {
        printf("Correct!");
        getch();
    }
    else {
        printf("Fail!");
        getch();
    }
    getch();
}

int check_win() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == TARGET) {
                input_flag();
                return 1;
            }
        }
    }
    return 0;
}

int move_left() {
    int moved = 0;
    int merged[SIZE][SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        for (int j = 1; j < SIZE; j++) {
            if (board[i][j] != 0) {
                for (int k = j; k > 0; k--) {
                    if (board[i][k - 1] == 0) {
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        moved = 1;
                    } else if (board[i][k - 1] == board[i][k] && merged[i][k - 1] == 0 && merged[i][k] == 0) {
                        board[i][k - 1] *= 2;
                        board[i][k] = 0;
                        merged[i][k - 1] = 1; // 합쳐진 블록 표시
                        moved = 1;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

int move_right() {
    int moved = 0;
    int merged[SIZE][SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 2; j >= 0; j--) {
            if (board[i][j] != 0) {
                for (int k = j; k < SIZE - 1; k++) {
                    if (board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        moved = 1;
                    } else if (board[i][k + 1] == board[i][k] && merged[i][k + 1] == 0 && merged[i][k] == 0) {
                        board[i][k + 1] *= 2;
                        board[i][k] = 0;
                        merged[i][k + 1] = 1; // 합쳐진 블록 표시
                        moved = 1;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

int move_up() {
    int moved = 0;
    int merged[SIZE][SIZE] = {0};

    for (int j = 0; j < SIZE; j++) {
        for (int i = 1; i < SIZE; i++) {
            if (board[i][j] != 0) {
                for (int k = i; k > 0; k--) {
                    if (board[k - 1][j] == 0) {
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        moved = 1;
                    } else if (board[k - 1][j] == board[k][j] && merged[k - 1][j] == 0 && merged[k][j] == 0) {
                        board[k - 1][j] *= 2;
                        board[k][j] = 0;
                        merged[k - 1][j] = 1; // 합쳐진 블록 표시
                        moved = 1;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

int move_down() {
    int moved = 0;
    int merged[SIZE][SIZE] = {0};

    for (int j = 0; j < SIZE; j++) {
        for (int i = SIZE - 2; i >= 0; i--) {
            if (board[i][j] != 0) {
                for (int k = i; k < SIZE - 1; k++) {
                    if (board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        moved = 1;
                    } else if (board[k + 1][j] == board[k][j] && merged[k + 1][j] == 0 && merged[k][j] == 0) {
                        board[k + 1][j] *= 2;
                        board[k][j] = 0;
                        merged[k + 1][j] = 1;
                        moved = 1;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

void init(void) {
    count_obj = new count;

    if(!count_obj) {
        puts("error");
        exit(-1);
    }
    
    return;
}

int main() {
    srand(time(NULL));
    init_window();
    init_board();

    add_number();
    add_number();
    system("cls");

    while (1) {
        print_board();
        count_obj->update(board);
        is_first = 0;
        
        if (check_win())
            break;
            
        int direction = getch();
        switch (direction) {
            case 'a':
                if (move_left())
                    add_number();
                break;
            case 'd':
                if (move_right())
                   add_number();
                break;
            case 'w':
                if (move_up())
                    add_number();
                break;
            case 's':
                if (move_down())
                    add_number();
                break;
        }
    }

    return 0;
}