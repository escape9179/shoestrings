#include <windows.h>
#include <cwchar>

#define ESC "\x1b"
#define PLAYER_UP(a) "["#a"A"
#define PLAYER_DOWN(a) "["#a"B"
#define PLAYER_LEFT(a) "["#a"D"
#define PLAYER_RIGHT(a) "["#a"C"

int constexpr SCREEN_WIDTH = 120;
int constexpr SCREEN_HEIGHT = 30;
int constexpr READ_BUFFER_SIZE = 32;
int constexpr UPDATES_PER_SECOND = 1000 / 30;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
char constexpr PLAYER_CHAR = 'o';
int screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int playerX = 0;
int playerY = 0;

bool enableVirtualTerminalProcessing();

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord);

void renderScreen();

void moveDown(int x, int y);

int main() {
    enableVirtualTerminalProcessing();
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    printf(ESC "[?25l"); // Hide the cursor
    while (true) {
        renderScreen();
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

void renderScreen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            printf(ESC "[%i;%iH", y, x);
            printf("%c", screenBuffer[y][x]);
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

void moveDown() {
    screenBuffer[playerY][playerX] = ' ';
    playerY++;
    screenBuffer[playerY][playerX] = PLAYER_CHAR;
}

void moveUp() {
    screenBuffer[playerY][playerX] = ' ';
    playerY--;
    screenBuffer[playerY][playerX] = PLAYER_CHAR;
}

void moveLeft() {
    screenBuffer[playerY][playerX] = ' ';
    playerX--;
    screenBuffer[playerY][playerX] = PLAYER_CHAR;
}

void moveRight() {
    screenBuffer[playerY][playerX] = ' ';
    playerX++;
    screenBuffer[playerY][playerX] = PLAYER_CHAR;
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