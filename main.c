/* shoestrings
 * shoestring at bottom of screen
 * shooting falling sneakers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define SCREEN_WIDTH 121
#define SCREEN_HEIGHT 25
#define MAX_BULLETS 10
#define SLEEP_DUR 5
#define PLAYER 'S'
#define ENEMY 'E'
#define BULLET 'B'
#define CLEAR_CHAR ' '
#define SCORE_INC 1

typedef char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
typedef unsigned short ushort;
typedef unsigned char uchar;

struct position {
    ushort x, y;
} pp, ep, *bparr[MAX_BULLETS];

screen scrn;
uchar running;
uchar playing;
static unsigned int fc = 0;
static int score = 0;

/**
 * Draw entities and other data on the screen.
 */
void draw();

/**
 * Clear the console of all characters.
 */
void clr_scrn();

/**
 * Update positions of entities and other data.
 */
void update();

/**
 * Get a random coordinate along the x-axis.
 */
ushort rndscrnx();

void updateents() {
    // Calculate new enemy position
    // every tick.
    if (fc % 10 == 0) {
        ep.y += 1;
        if (ep.y >= SCREEN_HEIGHT) {
            ep.y = 0;
            ep.x = rndscrnx();
        }
    }

    // Move the bullets, performing any necessary
    // bound checks.
    if (fc % 1 == 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {

            // If the bullet pointer is null, do nothing.
            if (!bparr[i]);

                // If the bullet has reached the top of
                // the screen, free the memory associated
                // with the bullet and nullify it.
            else if ((bparr[i]->y) <= 0) {
                scrn[bparr[i]->y][bparr[i]->x] = CLEAR_CHAR;
                free(bparr[i]);
                bparr[i] = 0;
            } else {

                // Move the bullet up one character
                // on the y-axis.
                bparr[i]->y--;
                ushort bpx = bparr[i]->x;
                ushort bpy = bparr[i]->y;

                // Check if the bullet position
                // is equal to the position of the enemy
                // and delete the enemy if so. Then increment
                // the score.
                if (bpx == ep.x && bpy == ep.y) {
                    ep.x = rndscrnx(), ep.y = 0;
                    free(bparr[i]), bparr[i] = NULL;
                    score += SCORE_INC;
                }
            }
        }
    }

}

int main(void) {
    pp.x = SCREEN_WIDTH / 2, pp.y = SCREEN_HEIGHT - 1;
    running = 1;
    while (running) {
        clr_scrn();
        update();
        draw();
        _sleep(SLEEP_DUR);
    }
}

void update() {
    // If the game is being played update all entities.
    if (playing)updateents();

        // Show main menu.
    else {
        static char *optns[][20] = {"Play", "Quit", "Scores (WIP)"};
        static char *ymid = scrn[SCREEN_HEIGHT / 2];
        ymid = "Test";
    }

    // Get non-blocking keyboard input.
    if (_kbhit()) {
        char c = getch();
        if (c == 'a')pp.x--;
        if (c == 'd')pp.x++;
        if (c == 'q')exit(EXIT_SUCCESS);
        if (c == 'w') {

            // Create another bullet one character
            // above the player.
            static int i = 0;
            bparr[i] = malloc(sizeof(struct position));
            bparr[i]->x = pp.x, bparr[i]->y = pp.y - 1;
            if (++i >= MAX_BULLETS - 1)i = 0;
        }
    }
}

void draw() {
    printf("Score: %07d\n", score);
    scrn[pp.y][pp.x] = PLAYER;
    scrn[ep.y][ep.x] = ENEMY;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bparr[i])
            scrn[bparr[i]->y][bparr[i]->x] = BULLET;
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        puts(scrn[i]);
    }
    fc++;
}

void clr_scrn() {
    system("cls");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (j == SCREEN_WIDTH - 1) {
                scrn[i][j] = '\0';
                break;
            }
            scrn[i][j] = ' ';
        }
    }
}

ushort rndscrnx() {
    return (rand() / (float) RAND_MAX) * SCREEN_WIDTH - 1;
}