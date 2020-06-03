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
#define CLEAR_CHAR ' '

typedef char screen_t[SCREEN_HEIGHT][SCREEN_WIDTH];
typedef unsigned short ushort;

void draw();

void clr_scrn();

void update();

ushort rndscrnx();

ushort rndscrny();

screen_t screen;
short int running = 1;
short int px = SCREEN_WIDTH / 2, py = SCREEN_HEIGHT - 1;
short int ex = 0, ey = 0;
static unsigned int fc = 0;
static int scr = 0;
static struct bullet *bparr[MAX_BULLETS];

struct bullet {
    unsigned char x, y;
};

int main(void) {
    while (running) {
        clr_scrn();
        update();
        draw();
        _sleep(SLEEP_DUR);
    }
}

void update() {
    // Calculate new enemy position
    // every second
    if (fc % 10 == 0) {
        ey += 1;
        if (ey >= SCREEN_HEIGHT) {
            ey = 0;
            ex = rndscrnx();
        }
    }
    // move bullets
    if (fc % 1 == 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bparr[i]);
            else if ((bparr[i]->y) <= 0) {
                screen[bparr[i]->y][bparr[i]->x] = CLEAR_CHAR;
                free(bparr[i]);
                bparr[i] = NULL;
            } else {
                bparr[i]->y--;
                ushort bpx = bparr[i]->x;
                ushort bpy = bparr[i]->y;
                if (bpx == ex && bpy == ey) {
                    ex = rndscrnx(), ey = 0;
                    free(bparr[i]), bparr[i] = NULL;
                }
            }
        }
    }
    // Check input
    if (_kbhit()) {
        char c = getch();
        if (c == 'a')px--;
        if (c == 'd')px++;
        if (c == 'q')exit(EXIT_SUCCESS);
        if (c == 'w') {
            static int i = 0;
            bparr[i] = malloc(sizeof(struct bullet));
            bparr[i]->x = px, bparr[i]->y = py - 1;
            if (++i >= MAX_BULLETS - 1)i = 0;
        }
    }
}

void draw() {
    printf("Score: %07d\n", fc);
    screen[py][px] = PLAYER;
    screen[ey][ex] = ENEMY;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bparr[i]) {
            screen[bparr[i]->y][bparr[i]->x] = BULLET;
        }
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        puts(screen[i]);
    }
    fc++;
}

void clr_scrn() {
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

ushort rndscrnx() {
    return (rand() / (float) RAND_MAX) * SCREEN_WIDTH - 1;
}

ushort rndscrny() {
    return (rand() / (float) RAND_MAX) * SCREEN_HEIGHT - 1;
}