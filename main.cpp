#include <windows.h>
#include <cwchar>

#define ESC "\x1b"
#define CSI ESC "["
#define PLAYER_UP(a) "["#a"A"
#define PLAYER_DOWN(a) "["#a"B"
#define PLAYER_LEFT(a) "["#a"D"
#define PLAYER_RIGHT(a) "["#a"C"

int constexpr SCREEN_WIDTH = 120;
int constexpr SCREEN_HEIGHT = 30;
int constexpr READ_BUFFER_SIZE = 32;
int constexpr UPDATES_PER_SECOND = 1000 / 60;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
char constexpr PLAYER_CHAR = 'o';
int screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
bool update = false;

bool enableVirtualTerminalProcessing();

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord);

void drawPlayer();

void clearPosition(int, int);

void movePlayer(int, int);

struct Player {
    int x = 0;
    int y = 0;
} player;

int main() {
    enableVirtualTerminalProcessing();
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    printf(ESC "[?25l"); // Hide the cursor
    printf(CSI "?1049h");
    while (true) {
        Sleep(UPDATES_PER_SECOND);
        printf(ESC "[2 q");
        INPUT_RECORD inputRecords[READ_BUFFER_SIZE];
        DWORD numEventsRead;
        PeekConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
        if (numEventsRead == 0)
            continue;
        ReadConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
        for (int i = 0; i < numEventsRead; i++) {
            switch (inputRecords[i].EventType) {
                case KEY_EVENT:
                    processKeyEvent(inputRecords[i].Event.KeyEvent);
                    break;
            }
        }
    }
}

bool enableVirtualTerminalProcessing() {
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle == INVALID_HANDLE_VALUE)
        return false;
    DWORD consoleMode;
    if (!GetConsoleMode(outputHandle, &consoleMode))
        return false;
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(outputHandle, consoleMode))
        return false;
    return true;
}

void drawPlayer() {
    printf(CSI "%i;%iH", player.y, player.x);
    printf("%c", PLAYER_CHAR);
}

void clearPosition(int x, int y) {
    printf(CSI "%i;%iH", y, x);
    printf(CSI "1X");
}

void movePlayer(int x, int y) {
    clearPosition(player.x, player.y);
    player.x = x;
    player.y = y;
    drawPlayer();
}

void moveDown() {
    movePlayer(player.x, player.y + 1);
}

void moveUp() {
    movePlayer(player.x, player.y - 1);
}

void moveLeft() {
    movePlayer(player.x - 1, player.y);
}

void moveRight() {
    movePlayer(player.x + 1, player.y);
}

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord) {
    if (!keyEventRecord.bKeyDown)
        return;
    WORD keyCode = keyEventRecord.wVirtualKeyCode;
    switch (keyCode) {
        case VK_OEM_PERIOD:
            moveUp();
            break;
        case VK_U:
            moveRight();
            break;
        case VK_E:
            moveDown();
            break;
        case VK_O:
            moveLeft();
            break;
        default:
            printf("Pressed an unsupported key.\n");
            break;
    }
}