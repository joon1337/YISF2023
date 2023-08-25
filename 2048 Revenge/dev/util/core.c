#include <stdio.h>
#include <string.h>
#include <conio.h>
#define N 6

int table[] = {244, 74, 67, 198, 149, 127, 170, 108, 50, 171, 230, 155, 204, 163, 228, 150, 0, 78, 200, 4, 105, 251, 196, 144, 177, 199, 213, 46, 13, 207, 59, 214, 84, 34, 212, 247, 86, 253, 39, 103, 162, 233, 164, 71, 1, 109, 222, 60, 191, 116, 181, 72, 156, 182, 122, 77, 184, 37, 79, 211, 143, 88, 120, 98, 57, 174, 7, 250, 124, 126, 113, 48, 249, 186, 252, 248, 154, 167, 44, 240, 14, 220, 40, 11, 215, 205, 139, 54, 133, 195, 125, 117, 104, 20, 234, 27, 18, 65, 75, 158, 165, 208, 206, 53, 68, 15, 17, 66, 210, 218, 12, 179, 73, 29, 137, 217, 236, 97, 69, 185, 224, 110, 190, 38, 172, 115, 114, 192, 64, 188, 169, 119, 180, 136, 70, 6, 8, 134, 93, 35, 9, 242, 223, 159, 32, 216, 61, 243, 30, 28, 209, 80, 107, 227, 225, 232, 21, 10, 221, 42, 226, 45, 237, 202, 118, 47, 201, 36, 129, 89, 138, 130, 5, 101, 92, 31, 147, 100, 95, 94, 241, 194, 178, 135, 187, 76, 33, 111, 19, 235, 141, 43, 161, 153, 189, 245, 51, 25, 26, 142, 23, 151, 41, 231, 176, 152, 239, 52, 229, 145, 56, 148, 85, 166, 238, 121, 81, 255, 3, 102, 168, 146, 58, 87, 131, 160, 24, 183, 2, 99, 123, 91, 112, 49, 16, 90, 55, 140, 22, 63, 83, 246, 219, 173, 197, 96, 203, 132, 193, 62, 175, 82, 157, 128, 254, 106}; 
long long int C[N][N] = {0, };
long long int B[N][N] = {
    {2, 6, 2, 0, 0, 1},
    {9, 4, 2, 1, 0, 0},
    {3, 5, 3, 2, 9, 6},
    {6, 3, 4, 2, 7, 8},
    {0, 1, 3, 3, 7, 7},
    {0, 1, 9, 8, 3, 1}
};

void func1(unsigned long long (*arr)[6]) {
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<strlen((char*)ptr); i++) {
        ptr[i] = ((unsigned char*)ptr)[i];
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

void func3(unsigned long long (*arr)[6]) {
    // 테이블 값과 치환
    unsigned char* ptr = (unsigned char*)arr;
    for(int i=0; i<36; i++) {
        unsigned char c = table[ptr[i]];
        ptr[i] = c;
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
    long long int lst[6][6] = {4170, 1752, 7576, 8131, 5112, 8096, 16104, 7061, 13860, 17980, 4755, 14877, 20727, 8451, 19733, 29389, 17124, 25329, 26635, 10210, 18349, 30123, 13720, 24617, 16024, 5873, 13277, 19728, 10782, 17628, 26894, 25512, 24341, 12939, 28739, 22839};
    for(int i=0; i<6; i++) {
        for(int j=0; j<6; j++) {
            if(C[i][j] != lst[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// flag: YISF{0Hh_4r3_y0U_900d_4t_2048_94m3?}
int main() {
    char flag[0x30] = {0, };
    scanf("%s", flag);

    func1((long long unsigned int (*)[6])flag);
    func2((long long unsigned int (*)[6])flag);
    func3((long long unsigned int (*)[6])flag);
    func4((long long unsigned int (*)[6])flag);
    func5((long long unsigned int (*)[6])flag);
    func6((long long unsigned int (*)[6])flag);
    func7((long long unsigned int (*)[6])flag);
    func8((long long unsigned int (*)[6])flag);
    func9((long long unsigned int (*)[6])flag);
    
    if(check_validate()) {
        printf("Correct!");
        getch();
    }
    else {
        printf("Fail!");
        getch();
    }
}