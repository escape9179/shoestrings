#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BUFFER_WIDTH 80
#define BUFFER_HEIGHT 40
#define X_MIN 1
#define Y_MIN 1
#define X_MAX BUFFER_WIDTH - 2
#define Y_MAX BUFFER_HEIGHT - 2
#define SLEEP_DELAY 20
#define SLEEPS_PER_TICK 10
#define SHOE_SPAWN_RATE 10
#define SHOE_MOVE_RATE 2
#define MAX_SHOES 10
#define MAX_BULLETS 10
#define PLAYER_CHAR 'p'
#define SHOE_CHAR 's'
#define BULLET_CHAR 'b'
#define CLEAR_CHAR ' '
#define BORDER_CHAR '#'

struct position {
    unsigned char x, y;
} player = {X_MIN, Y_MAX - 2}, *shoe_array[MAX_SHOES], *bullet_array[MAX_BULLETS];

void write_char(unsigned int, unsigned int, char);

void move_entity(unsigned int, unsigned int, unsigned int, unsigned int, char);

void write_text(unsigned int, unsigned int, char const *);

void update_score();

static unsigned char running;
static unsigned short sleep_count;
static unsigned short tick_count;
static unsigned short score;

int main(void) {

    // Do window initialization stuff
    MoveWindow(GetConsoleWindow(), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);

    // Draw play area
    COORD position = {0, 0};
    for (int i = Y_MIN; i < BUFFER_HEIGHT; i++) {
        position.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        write_char(BUFFER_WIDTH - 1, position.Y, BORDER_CHAR);
    }
    for (int i = BUFFER_WIDTH - 1; i > -1; i--) {
        position.X = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        write_char(position.X, position.Y, BORDER_CHAR);
    }
    for (int i = BUFFER_HEIGHT - 1; i > 0; i--) {
        position.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
        write_char(position.X, position.Y, BORDER_CHAR);
    }

    running = 1;

    // Set initial position of player
    move_entity(0, 0, player.x, player.y, PLAYER_CHAR);

    while (running) {

        // Do a tick
        if (sleep_count % SLEEPS_PER_TICK == 0) {
            int i;

            // Spawn new shoe every SHOE_SPAWN_RATE ticks
            if (tick_count % SHOE_SPAWN_RATE == 0) {
                for (i = 0; i < MAX_SHOES; i++) {
                    if (shoe_array[i]) continue;
                    shoe_array[i] = malloc(sizeof(struct position));
                    shoe_array[i]->x = (((float) rand() / RAND_MAX) * (X_MAX - 1)) + 1;
                    shoe_array[i]->y = Y_MIN;
                    break;
                }
            }

            // Move falling shoes
            if (tick_count % SHOE_MOVE_RATE == 0) {
                for (i = 0; i < MAX_SHOES; i++) {
                    if (!shoe_array[i]) continue;
                    move_entity(shoe_array[i]->x, shoe_array[i]->y, shoe_array[i]->x, shoe_array[i]->y + 1, SHOE_CHAR);

                    // Do boundary check on shoe and set it to null if it's out of bounds
                    if (++shoe_array[i]->y == Y_MAX - 1) {
                        write_char(shoe_array[i]->x, shoe_array[i]->y, CLEAR_CHAR);
                        shoe_array[i] = 0;
                    }
                }
            }

            // Move bullets
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!bullet_array[i])continue;
                move_entity(bullet_array[i]->x, bullet_array[i]->y, bullet_array[i]->x, bullet_array[i]->y - 1,
                            BULLET_CHAR);

                // Do boundary check on bullet and set it to null if it's out of bounds
                if (--bullet_array[i]->y <= Y_MIN) {
                    write_char(bullet_array[i]->x, bullet_array[i]->y, CLEAR_CHAR);
                    bullet_array[i] = 0;
                    continue;
                }

                // Check if the bullet hit a shoe
                for (int j = 0; j < MAX_SHOES; j++) {
                    if (!shoe_array[j])continue;
                    if (shoe_array[j]->x == bullet_array[i]->x && shoe_array[j]->y >= bullet_array[i]->y) {
                        write_char(shoe_array[j]->x, shoe_array[j]->y, CLEAR_CHAR);
                        write_char(bullet_array[i]->x, bullet_array[i]->y, CLEAR_CHAR);
                        shoe_array[j] = 0;
                        bullet_array[i] = 0;
                        score++;
                        break;
                    }
                }
            }

            tick_count++;
        }

        // Process input and move player
        if (_kbhit()) {
            char c = getch();
            if (c == 'w') {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullet_array[i]) {
                        bullet_array[i] = malloc(sizeof(struct position));
                        bullet_array[i]->x = player.x;
                        bullet_array[i]->y = player.y - 1;
                        write_char(bullet_array[i]->x, bullet_array[i]->y, BULLET_CHAR);
                        break;
                    }
                }
            } else if (c == 'a') {
                if (player.x - 1 < X_MIN) goto endif;
                move_entity(player.x, player.y, player.x - 1, player.y, PLAYER_CHAR);
                player.x--;
            } else if (c == 'd') {
                if (player.x + 1 > X_MAX) goto endif;
                move_entity(player.x, player.y, player.x + 1, player.y, PLAYER_CHAR);
                player.x++;
            } else if (c == 'q') {
                exit(EXIT_SUCCESS);
            }
        }
        endif:

        update_score(score);

        // Sleep
        _sleep(SLEEP_DELAY);
        if (sleep_count < 1000) sleep_count++;
        else sleep_count = 0;
    }
    return 0;
}

void write_char(unsigned int x, unsigned int y, char ch) {
    COORD position = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    putchar(ch);
}

void write_text(unsigned int x, unsigned int y, char const *text) {
    COORD position = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    puts(text);
}

void move_entity(unsigned int prevX, unsigned int prevY, unsigned int newX, unsigned int newY, char ch) {
    write_char(prevX, prevY, CLEAR_CHAR);
    write_char(newX, newY, ch);
}

void update_score() {
    static char score_text[30];
    sprintf(score_text, "Score: %010d", score);
    write_text(0, 0, score_text);
}
