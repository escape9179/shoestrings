#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/* shoestrings
 * shoestring at bottom of screen
 * shooting falling sneakers. */

#define SCREEN_WIDTH 121
#define SCREEN_HEIGHT 25
#define PLAYER 'S'
#define ENEMY 'E'
#define SLEEP_DUR 5

typedef char screen_t[SCREEN_HEIGHT][SCREEN_WIDTH];

void draw();

void clear_screen();

void update();

screen_t screen;
short int running = 1;
short int x = SCREEN_WIDTH / 2, y = SCREEN_HEIGHT - 1;
short int enemy_x = 0, enemy_y = 0;
unsigned int frame_count = 0;
static int score = 0;

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
    // Check input
    if (_kbhit()) {
        char c = getch();
        if (c == 'a')x--;
        if (c == 'd')x++;
    }
}

void draw() {
    printf("Score: %07d\n", frame_count);
    screen[y][x] = PLAYER;
    screen[enemy_y][enemy_x] = ENEMY;
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
