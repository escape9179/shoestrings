#include <windows.h>
#include <cwchar>
#include <vector>
#include <memory>
#include "Color.h"
#include "Entity.h"

#define ESC "\x1b"
#define CSI ESC "["

int constexpr READ_BUFFER_SIZE = 32;
int constexpr FPS = 60;
int constexpr FALL_RATE = FPS / 2;
int constexpr UPDATES_PER_SECOND = 1000 / FPS;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
int constexpr SCREEN_BOTTOM = 30;
int constexpr SCREEN_RIGHT = 120;
int constexpr SCREEN_TOP = 1;
int constexpr SCREEN_LEFT = 1;
int constexpr SCREEN_WIDTH = SCREEN_RIGHT;
int constexpr SCREEN_HEIGHT = SCREEN_BOTTOM;
unsigned long frameCount = 0;

Color RED(255, 0, 0);
Color GREEN(0, 255, 0);
Color BLUE(0, 0, 255);
Color WHITE(255, 255, 255);

std::vector<Entity> entities;

Entity player(EntityType::PLAYER, 10, 10, GREEN);

void printDebugMessage(char const *message) {
//    printf(ESC "7"); // Save cursor position
    printf(CSI "%i;%i", 10, 10);
    printf(CSI "2K"); // Erase entire line
    puts(message);
//    printf(ESC "8"); // Restore cursor position
}

void spawnEntity(EntityType type, int x, int y) {
    entities.emplace_back(type, x, y);
}

void destroyEntity(const Entity &entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    entities.erase(it);
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

void drawEntity(const Entity &entity) {
    printf(CSI "38;2;%i;%i;%im", entity.getColor().r, entity.getColor().g, entity.getColor().b);
//    printf(CSI "38;2;%i;%i;%im", 255, 255, 255);
    printf(CSI "%i;%iH", entity.getY(), entity.getX());
    printf("%c", entity.getChar());
}

void drawBulletEntity(const Entity &entity) {
    printf(ESC "(0");
    drawEntity(entity);
    printf(ESC "(B");
}

void drawEntityByType(const Entity &entity) {
    switch (entity.getType()) {
        case ENEMY:
        case PLAYER:
            drawEntity(entity);
            break;
        case BULLET:
            drawBulletEntity(entity);
            break;
    }
}

void drawEntities() {
    drawEntity(player);
    for (const auto &entity: entities) {
        drawEntityByType(entity);
    }
}

void clearPosition(int x, int y) {
    printf(CSI "%i;%iH", y, x);
    printf(CSI "1X");
}

void moveEntityDownwardIfEnemy(Entity &entity) {
    if (entity.getType() == ENEMY) {
        clearPosition(entity.getX(), entity.getY());
        entity.setY(entity.getY() + 1);
    }
}

void moveEnemiesDownward() {
    for (auto &entity: entities) {
        moveEntityDownwardIfEnemy(entity);
    }
}

void movePlayer(int x, int y) {
    if (SCREEN_LEFT > x || x > SCREEN_WIDTH)x = player.getX();
    if (SCREEN_TOP > y || y > SCREEN_HEIGHT)y = player.getY();
    clearPosition(player.getX(), player.getY());
    player.setX(x);
    player.setY(y);
}

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord) {
    if (!keyEventRecord.bKeyDown)
        return;
    WORD keyCode = keyEventRecord.wVirtualKeyCode;
    switch (keyCode) {
        case VK_OEM_PERIOD:
            movePlayer(player.getX(), player.getY() - 1);
            break;
        case VK_U:
            movePlayer(player.getX() + 1, player.getY());
            break;
        case VK_E:
            movePlayer(player.getX(), player.getY() + 1);
            break;
        case VK_O:
            movePlayer(player.getX() - 1, player.getY());
            break;
        case VK_SHIFT:
            exit(0);
            break;
        default:
            printf("Pressed an unsupported key.\n");
            break;
    }
}
void handleInput() {
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecords[READ_BUFFER_SIZE];
    DWORD numEventsRead;
    PeekConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
    if (numEventsRead == 0)
        return;
    ReadConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
    for (int i = 0; i < numEventsRead; i++) {
        switch (inputRecords[i].EventType) {
            case KEY_EVENT:
                processKeyEvent(inputRecords[i].Event.KeyEvent);
                break;
        }
    }
}
void enterGameLoop() {
    while (true) {
        handleInput();
        if (frameCount % FALL_RATE == 0) {
            moveEnemiesDownward();
        }
        drawEntities();
        frameCount++;
        Sleep(UPDATES_PER_SECOND);
    }
}

int main() {
    spawnEntity(ENEMY, 50, 20);
    spawnEntity(ENEMY, 10, 5);
    spawnEntity(BULLET, 70, 20);

    enableVirtualTerminalProcessing();
    printf(CSI "?25l"); // Hide the cursor
    printf(CSI "?1049h");

    enterGameLoop();
}