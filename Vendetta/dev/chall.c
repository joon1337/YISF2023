// gcc -o chall chall.c -O0 -z relro -z now -no-pie -fno-stack-protector

#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>

#define MAFIA_NUM 1
#define PLAYER_NUM 10

typedef struct guest {
    size_t len;
    char job[0x8];
    char name[0x10];
} guest_t;

typedef struct player {
    void (*func_ptr)(char*);
	char job[0x8];
	char name[0x10];
} player_t;

int day = 1;
void** players = NULL;
char guest_name[0x10];
char check[10] = {0, };
char is_dead[PLAYER_NUM] = {0, };
unsigned int guest_idx, doctor_idx, mafia_idx;

char* computer_name[] = {
    "Alice",
    "Bob",
    "Carol",
    "Dave",
    "Eve",
    "Faythe",
    "Grace",
    "Heidi",
    "Isaac",
    "Justin"
};
char* computer_chat_msg[] = {
    "Good morning!",
    "I'm not mafia.",
    "Hello world!",
    "I'm citizen.",
    "Who's mafia?",
    "Who's doctor?",
    "Don't forget to act innocent.",
    "Glad to see everyone survived the night!",
    "Who do you think is the most suspicious?",
    "Let's discuss any leads or suspicions we have.",
};
char* computer_vote_msg[] = {
    "Don't kill me!",
    "I want /bin/sh",
    "Before I go, I want to say that I am a innocent.",
    "To my fellow players, I plead for your rationality.",
    "It's unfortunate that I've become the primary suspect.",
    "Just remember, the real Mafia are still out there.",
    "I hope you uncover the truth after I'm gone."
};

void initialize() {
    alarm(120);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

void citizen_chat(char* ptr) {
    printf("%s\n", ptr);
}

void doctor_chat(char* ptr) {
    printf("If you eliminate me, you not only lose a skilled doctor but also jeopardize the well-being of this town.\n");
}

void mafia_chat(char* ptr) {
    printf("The real mafia member is still among us. Good luck.\n");
}

unsigned int get_rand(unsigned int start, unsigned int end) {
    int fd = 0;
    unsigned int rand_num = 0, range = 0;

    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        puts("DM admin plz..");
        exit(-1);
    }

    read(fd, &rand_num, sizeof(unsigned int));
    close(fd);
    range = end - start + 1;
    return start + (rand_num % range);
}

char* get_computer_name() {
    unsigned int rand_num = 0;

    while(1) {
        rand_num = get_rand(0, sizeof(computer_name)/sizeof(computer_name[0])-2);
        if (!check[rand_num]) {
            check[rand_num] = 1;
            return computer_name[rand_num];
        }
    }
}

void setup_player() {
    players = calloc(PLAYER_NUM, sizeof(void*));
    
    guest_idx = get_rand(0, PLAYER_NUM-1);
    while ((doctor_idx = get_rand(0, PLAYER_NUM-1)) == guest_idx);
    while (((mafia_idx = get_rand(0, PLAYER_NUM-1)) == guest_idx) || (mafia_idx == doctor_idx));

    players[guest_idx] = malloc(sizeof(guest_t));
    players[doctor_idx] = malloc(sizeof(player_t));
    players[mafia_idx] = malloc(sizeof(player_t));
    
    if (!players[guest_idx] || !players[doctor_idx] || !players[mafia_idx]) {
        puts("Memory allocation failed");
        exit(-1);
    }

    strcpy(((guest_t*)players[guest_idx])->name, guest_name);
    strcpy(((guest_t*)players[guest_idx])->job, "citizen");
    ((guest_t*)players[guest_idx])->len = 0x1f;

    strcpy(((player_t*)players[doctor_idx])->name, get_computer_name());
    strcpy(((player_t*)players[doctor_idx])->job, "doctor");
    ((player_t*)players[doctor_idx])->func_ptr = doctor_chat;

    strcpy(((player_t*)players[mafia_idx])->name, get_computer_name());
    strcpy(((player_t*)players[mafia_idx])->job, "mafia");
    ((player_t*)players[mafia_idx])->func_ptr = mafia_chat;

    for(int i=0; i<PLAYER_NUM; i++) {
        if (!players[i]) {
            players[i] = malloc(sizeof(player_t));
            strcpy(((player_t*)players[i])->name, get_computer_name());
            strcpy(((player_t*)players[i])->job, "citizen");
            ((player_t*)players[i])->func_ptr = citizen_chat;

            if (!players[i]) {
                puts("Memory allocation failed");
                exit(-1);
            }
        }
    }
}

void setup_game() {
    char buf[0x10] = {0, };
    size_t len = 0;

    printf("\n[+] Enter name: ");
    if (read(0, buf, sizeof(buf)-1) == -1) {
        perror("Error reading input");
        exit(-1);
    }
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
    strcpy(guest_name, buf);
    setup_player();
}

int mafia_shoot() {
    if(is_dead[mafia_idx])
        return -1;

    while(1) {
        int choice = get_rand(0, PLAYER_NUM-1);
        if (!is_dead[choice] && (choice != mafia_idx)) {
            return choice;
        }
    }
}

int doctor_heal() {
    int choice;
    if(is_dead[doctor_idx])
        return -1;
    
    while(1) {
        choice = get_rand(0, PLAYER_NUM-1);
        if (!is_dead[choice])
            return choice;
    }
}

char* pick_msg(int mode) {
    char* ptr = NULL;
    
    if (!mode) {
        ptr =  computer_chat_msg[get_rand(0, sizeof(computer_chat_msg)/sizeof(computer_chat_msg[0])-1)];
    } else {
        ptr = computer_vote_msg[get_rand(0, sizeof(computer_vote_msg)/sizeof(computer_vote_msg[0])-1)];
    }
    return ptr;
}

void check_result(){
    int citizen_num = 0;

    if(is_dead[guest_idx]) {
        printf("\x1b[31m[+] You died !\x1b[0m");
        exit(0);
    }
    
    if(is_dead[mafia_idx]) {
        printf("\x1b[36m[+] You win !\x1b[0m");
        exit(0);
    }
    
    for(int i=0; i<PLAYER_NUM; i++) {
        if (i != mafia_idx && !is_dead[i])
            citizen_num++;
    }

    if (MAFIA_NUM >= citizen_num) {
        printf("\x1b[31m[+] You lose !\x1b[0m");
        exit(0);
    }
    return;
}

void update_statistics(int idx) {
    char buf[0x100];
    size_t n = 0;
    void* ptr = players[idx];

    memset(buf, 0, sizeof(buf));
    if(!strcmp(ptr + 0x10, guest_name)) {
        n = ((guest_t*)ptr)->len;
        printf("\x1b[34m[+] You got the most votes in the vote.\x1b[0m\n[>] ");
        read(0, buf, n);
        printf("[+] %s said : %s\n", guest_name, buf);
    } else {
        printf("\x1b[34m[+] %s was got the most votes in the vote.\x1b[0m\n", ((player_t*)ptr)->name);
        printf("[+] %s said : ", ((player_t*)ptr)->name);
        ((player_t*)ptr)->func_ptr(pick_msg(1));
    }
    is_dead[idx] = 1;
    return;
}

int vote() {
    char target[0x10] = {0, };
    int count[PLAYER_NUM] = {0, };
    int rand_num = 0, max = 0, duplicate = 0, idx = 0;
    usleep(1000000);
    
    if (day == 3) {
        memset(target, 0, sizeof(target));
        printf("\x1b[35m[+] Who do you want to vote?\x1b[0m\n[>] ");
        read(0, target, sizeof(target)-1);
        for(int i=0; i<PLAYER_NUM; i++) {
            if(!strncmp(players[i]+0x10, target, strlen(players[i]+0x10)) && !is_dead[i])
                return i;
        }
        return -1;
    }

    for(int i=0; i<PLAYER_NUM; i++) {
        if (!is_dead[i] && i != guest_idx) {
            while(1) {
                rand_num = get_rand(0, PLAYER_NUM-1);
                if(is_dead[rand_num] != 1)
                    break;
            }
            count[rand_num]++;
        }
    }
    
    for(int i=0; i<PLAYER_NUM; i++) {
        if (count[i] > max)
            max = count[i];
    }
    for(int i=0; i<PLAYER_NUM; i++) {
        if (count[i] == max) {
            duplicate++;
            idx = i;
        }
    }
    
    if (duplicate == 1)
        return idx;
    else
        return -1;
}

void* game() {
        char chat_msg[0x100];
        int mafia_choice = 0;
        int doctor_choice = 0;
        int vote_result = 0;

        system("clear");
        printf("[+] Your number is <%d>\n", guest_idx);
        while (1) {
            memset(chat_msg, 0, sizeof(chat_msg));

            /* Night */
            printf("\n\x1b[32m[+] Day %d - Night\x1b[0m\n", day);
            usleep(2000000);

            mafia_choice = mafia_shoot();
            doctor_choice = doctor_heal();
            
            if((doctor_choice == mafia_choice) && (doctor_choice != -1)) {
                printf("\x1b[34m[+] %s was survived by doctor\x1b[0m\n", ((player_t*)players[mafia_choice])->name);
            }
            else {
                is_dead[mafia_choice] = 1;
                printf("\x1b[31m[+] %s was killed by mafia\x1b[0m\n", ((player_t*)players[mafia_choice])->name);
            }
            check_result();
            day++;

            /* Morning */
            printf("\x1b[32m[+] Day %d - Morning\x1b[0m\n\n", day);

            for(int i=0; i<PLAYER_NUM; i++) {
                char* name = ((player_t*)players[i])->name;
                if(strcmp(name, guest_name) && !is_dead[i])
                    printf("[%s] %s\n", name, pick_msg(0));
                usleep(100000);
            }

            printf("[%s] ", ((player_t*)players[guest_idx])->name);
            read(0, chat_msg, sizeof(chat_msg)-1);
            usleep(2000000);
            
            puts("\n\x1b[33m[+] Vote start\x1b[0m");
            vote_result = vote();
            puts("\x1b[33m[+] Vote end\x1b[0m\n");

            if (vote_result != -1) {
                update_statistics(vote_result);
                check_result();
            }
            else {
                printf("\x1b[34m[+] No one died by vote\x1b[0m\n");
            }
    }

    return;
}

void print_menu() {
    puts("\n[1] Game Start");
    puts("[2] Rule");
    puts("[3] Gift");
    printf("[>] ");
}

void rule() {
    puts("\nWelcome to mafia game.");
    puts("Survive until the end.");
    puts("You start as a citizen.");
    puts("Additionally you are not allowed to vote.");
}

void gift() {
    puts("\nThank you to everyone who participated in YISF 2023.");
    puts("Enjoy!");
}

int main() {
    int loop = 1;
    int choice = 0;

    initialize();
    while(loop) {
        print_menu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                setup_game();
                game();
                loop = 0;
                break;
            case 2:
                rule();
                break;
            case 3:
                gift();
                break;
            default:
                puts("[+] Choose a number between 1 and 3");
                break;
        }
    }
}
