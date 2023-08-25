#include <stdio.h>
#include <conio.h>

void func1(int* a) {
    int b = 7;
    int c = 5;
    int i = 0, j = 0;
    int d = 0;
    for (i = 0; i < b; i++) {
        for (j = 0; j < c; j++) {
            d = (i + j) * (i - j) + (i * j);
            a[i * c + j] = d;
        }
    }
}

void func2(char* a, int* b, int* c, int d) {
    int i = 0;
    for (i = 0; i < d; i++) {
        a[i] = b[i] ^ c[i];
    }
}

int main() {
    int a[35];
    char b[36] = {0, };
    int c[36] = {89,-74,-81,-79,-117,86,105,-53,-52,-86,53,48,91,54,-76,56,57,52,49,49,115,120,75,36,32,70,42,119,122,66,64,25,30,18,81};
    
    func1(a);
    func2(b, a, c, 35);
    puts(b);
    getch();

    return 0;
}
