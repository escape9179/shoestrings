#include <windows.h>
#include <cwchar>
#include <vector>
#include <chrono>
#include "Color.h"
#include "Entity.h"

#define ESC "\x1b"
#define CSI ESC "["

int constexpr READ_BUFFER_SIZE = 32;
int constexpr TARGET_FPS = 60;
std::chrono::duration<double, std::milli> FRAME_DURATION(1000 / TARGET_FPS);
int constexpr FALL_RATE = TARGET_FPS / 2;
int constexpr BULLET_MOVE_RATE = TARGET_FPS / 20;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
int constexpr SCREEN_BOTTOM = 29;
int constexpr SCREEN_RIGHT = 120;
int constexpr SCREEN_TOP = 1;
int constexpr SCREEN_LEFT = 1;
int constexpr STATUS_MESSAGE_ROW = SCREEN_BOTTOM + 1;
unsigned long frameCount = 0;

std::vector<Entity> entities;

Entity player;

template<typename... Args>
void setStatusMessage(const char *message, Args... args) {
    printf(CSI "0m");
    printf(CSI "%i;%iH", STATUS_MESSAGE_ROW, SCREEN_LEFT);
    printf(message, args...);
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

Entity *getEntityAtPosition(int x, int y) {
    auto result = std::find_if(entities.begin(), entities.end(), [=] (const Entity &entity) {
        return entity.getX() == x && entity.getY() == y;
    });
    return result == entities.end() ? nullptr : &*result;
}

void moveEntity(Entity &entity, int x, int y) {
    if (SCREEN_LEFT > x || x > SCREEN_RIGHT) return;
    if (SCREEN_TOP > y || y > SCREEN_BOTTOM) return;
    clearPosition(entity.getX(), entity.getY());

    /* Perform collision detection with other entities. */
    auto entityAtPosition = getEntityAtPosition(x, y);
    if (entityAtPosition == nullptr) {
        entity.setX(x);
        entity.setY(y);
        return;
    }

    if (entity.getType() == BULLET) {
        destroyEntity(*entityAtPosition);
    }
}

void movePlayer(int x, int y) {
    moveEntity(player, x, y);
}

void shootBullet() {
    int x = player.getX();
    int y = player.getY() - 1;
    spawnEntity(BULLET, x, y);
}

void moveBulletsUp() {
    for (Entity &entity: entities)
        if (entity.getType() == BULLET) {
            moveEntity(entity, entity.getX(), entity.getY() - 1);
        }
}

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord) {
    if (!keyEventRecord.bKeyDown)
        return;
    WORD keyCode = keyEventRecord.wVirtualKeyCode;
    switch (keyCode) {
        case VK_OEM_PERIOD:
            shootBullet();
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
        auto previousTime = std::chrono::high_resolution_clock::now();
        handleInput();
        if (frameCount % FALL_RATE == 0)
            moveEnemiesDownward();
        if (frameCount % BULLET_MOVE_RATE == 0)
            moveBulletsUp();
        drawEntities();
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = currentTime - previousTime;

        if (elapsedTime < FRAME_DURATION) {
            auto sleepDuration = FRAME_DURATION - elapsedTime;
            std::this_thread::sleep_for(sleepDuration);
        }

        previousTime = std::chrono::steady_clock::now();
    }
}

int main() {
    player = {PLAYER, 10 ,10, Color::GREEN};
    spawnEntity(ENEMY, 50, 20);
    spawnEntity(ENEMY, 10, 5);

    enableVirtualTerminalProcessing();
    printf(CSI "?25l"); // Hide the cursor
    printf(CSI "?1049h");

    enterGameLoop();
}