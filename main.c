#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/* shoestrings
 * shoestring at bottom of screen
 * shooting falling sneakers. */

#define SCREEN_WIDTH 121
#define SCREEN_HEIGHT 25
#define MAX_BULLETS 10
#define PLAYER 'S'
#define ENEMY 'E'
#define BULLET 'B'
#define SLEEP_DUR 5

typedef char screen_t[SCREEN_HEIGHT][SCREEN_WIDTH];

void draw();

void clear_screen();

void update();

screen_t screen;
short int running = 1;
short int x = SCREEN_WIDTH / 2, y = SCREEN_HEIGHT - 1;
short int enemy_x = 0, enemy_y = 0;
static unsigned int frame_count = 0;
static int score = 0;
static struct bullet *bp_array[MAX_BULLETS];
static unsigned char bi;

struct bullet {
    unsigned char x, y;
};

int main(void) {
    while (running) {
        clear_screen();
        update();
        draw();
        _sleep(SLEEP_DUR);
    }
}

void update() {
    // Calculate new enemy position
    // every second
    if (frame_count % 10 == 0) {
        enemy_y += 1;
        if (enemy_y >= SCREEN_HEIGHT) {
            enemy_y = 0;
            enemy_x = (rand() / (float) RAND_MAX) * (SCREEN_WIDTH - 1);
        }
    }
    // move bullets
    if (frame_count % 5 == 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bp_array[i] == NULL);
            else if ((bp_array[i]->y) <= 0) {
                free(bp_array[i]);
            } else
                bp_array[i]->y--;
        }
    }
    // Check input
    if (_kbhit()) {
        char c = getch();
        if (c == 'a')x--;
        if (c == 'd')x++;
        if (c == 'w') {

            struct bullet *bp = (struct bulllet *) bp_array;
            bp = malloc(sizeof(struct bullet));
            bp->x = x, bp->y = y;
            bp++;


//            bp_array[bi] = malloc(sizeof(struct bullet));
//            bp_array[bi]->x = x, bp_array[bi]->y = y;
//            if (++bi > MAX_BULLETS)bi = 0;
        }
    }
}

void draw() {
    printf("Score: %07d\n", frame_count);
    printf("Debug: %s\n", dbgmsg);
    screen[y][x] = PLAYER;
    screen[enemy_y][enemy_x] = ENEMY;
//    for(int i=0;i<MAX_BULLETS;i++){
//        screen[bp_array[i]->y][bp_array[i]->x]=BULLET;
//    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        puts(screen[i]);
    }
    frame_count++;
}

void clear_screen() {
    system("cls");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (j == SCREEN_WIDTH - 1) {
                screen[i][j] = '\0';
                break;
            }
            screen[i][j] = ' ';
        }
    }
}
