/* shoestrings
 * shoestring at bottom of screen
 * shooting falling sneakers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define SCREEN_WIDTH 230
#define SCREEN_HEIGHT 50
#define MAX_BULLETS 10
#define SLEEP_DUR 5
#define PLAYER 193
#define ENEMY 254
#define BC 254
#define BULLET 179
#define CC ' '
#define SCORE_INC 1
#define PLAYER_HEIGHT 12
#define PLAYER_WIDTH 3
#define SHOE_HEIGHT 8
#define SHOE_WIDTH 12

typedef unsigned char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
typedef unsigned short ushort;

struct position {
    ushort x, y;
} pp, sp, *bparr[MAX_BULLETS];

screen scrn;
short int running = 1;
static unsigned int fc = 0;
static int score = 0;

static char pcharr[PLAYER_HEIGHT][PLAYER_WIDTH] = {
        {CC, BC, CC},
        {CC, BC, CC},
        {CC, BC, CC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
        {BC, BC, BC},
};

static char scharr[][12] = {
        {CC, CC, BC, BC, BC, BC, CC, CC, CC, CC, CC, CC},
        {CC, CC, BC, BC, BC, BC, CC, CC, CC, CC, CC, CC},
        {CC, CC, BC, BC, BC, BC, CC, CC, CC, CC, CC, CC},
        {CC, BC, BC, BC, BC, BC, BC, CC, CC, CC, CC, CC},
        {CC, BC, BC, BC, BC, BC, BC, BC, BC, CC, CC, CC},
        {BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, CC},
        {BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC},
        {BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC, BC}
};

/**
 * Draw entities and other data on the screen.
 */
void draw();

/**
 * Clear the console of all characters.
 */
void clrscrn();

/**
 * Update positions of entities and other data.
 */
void update();

/**
 * Get a random coordinate along the x-axis.
 */
ushort rndscrnx();

void drawobj(int, int, int, int, char **);

int main(void) {
    pp.x = 2, pp.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
    while (running) {
        clrscrn();
        update();
        draw();
        _sleep(SLEEP_DUR);
    }
}

void update() {

    // Calculate new enemy position
    // every tick.
    if (fc % 10 == 0) {
        sp.y += 1;
        if (sp.y + SHOE_HEIGHT >= SCREEN_HEIGHT) {
            sp.y = 0;
            sp.x = rndscrnx();
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
                scrn[bparr[i]->y][bparr[i]->x] = CC;
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
                if (bpx >= sp.x && bpx <= (sp.x + SHOE_HEIGHT) && bpy >= sp.y && bpy <= sp.y + SHOE_WIDTH) {
                    sp.x = rndscrnx(), sp.y = 0;
                    free(bparr[i]), bparr[i] = NULL;
                    score += SCORE_INC;
                }
            }
        }
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
            bparr[i]->x = pp.x + 1, bparr[i]->y = pp.y - 1;
            if (++i >= MAX_BULLETS - 1)i = 0;
        }
    }
}

void drawobj(int epx, int epy, int epw, int eph, char **charr) {
    for (int i = epy; i < eph + epy; i++) {
        for (int j = epx; j < epw + epx; j++) {
            scrn[i][j] = *((char *) charr + ((i - epy) * epw) + (j - epx));
        }
    }
}

void draw() {
    printf("Score: %07d\n", score);

    drawobj(pp.x, pp.y, PLAYER_WIDTH, PLAYER_HEIGHT, (char **) pcharr);
    drawobj(sp.x, sp.y, SHOE_WIDTH, SHOE_HEIGHT, (char **) scharr);

    // Draw bullets.
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bparr[i])
            scrn[bparr[i]->y][bparr[i]->x] = BULLET;
    }

    // Draw character buffer to screen.
    for (int i = 0; i < SCREEN_HEIGHT; i++)
        puts(scrn[i]);
    fc++;
}

void clrscrn() {
    system("cls");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if (j == SCREEN_WIDTH - 1) {
                scrn[i][j] = '\0';
                break;
            }
            scrn[i][j] = CC;
        }
    }
}

ushort rndscrnx() {
    return (rand() / (float) RAND_MAX) * SCREEN_WIDTH - 1;
}