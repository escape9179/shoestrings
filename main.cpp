#include <windows.h>
#include <cwchar>
#include <vector>
#include <chrono>
#include <random>
#include "Color.h"
#include "Entity.h"
#include "Bullet.h"

#define ESC "\x1b"
#define CSI ESC "["

int constexpr READ_BUFFER_SIZE = 32;
float constexpr FALL_SPEED = 1.0f;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
int constexpr SCREEN_BOTTOM = 29;
int constexpr SCREEN_RIGHT = 120;
int constexpr SCREEN_TOP = 1;
int constexpr SCREEN_LEFT = 1;
int constexpr STATUS_MESSAGE_ROW = SCREEN_BOTTOM + 1;

std::vector<Entity *> entities;

Entity player;

void setCursorPosition(int x, int y) {
    printf(CSI "%i;%iH", y, x);
}

template<typename... Args>
void setStatusMessage(const char *message, Args... args) {
    static int lastMessageLen = 0;
    printf(CSI "0m");
    printf(CSI "%i;%iH", STATUS_MESSAGE_ROW, SCREEN_LEFT);
    char buffer[SCREEN_RIGHT];
    int messageLen = sprintf( buffer, message, args...);
    if (lastMessageLen != messageLen) {
        printf(CSI "1M");
        lastMessageLen = messageLen;
    }
    printf("%s", buffer);
}

void spawnEntity(EntityType type, int x, int y) {
    switch (type) {
        case BULLET:
            entities.push_back(new Bullet(x, y));
            break;
        default:
            entities.push_back(new Entity(type, x, y));
    }
}

void destroyEntity(const Entity &entity) {
    auto it = std::find(entities.begin(), entities.end(), &entity);
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
    setCursorPosition(entity.getX(), entity.getY());
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
    for (const auto entity: entities) {
        drawEntityByType(*entity);
    }
}

void clearPosition(int x, int y) {
    setCursorPosition(x, y);
    printf(CSI "1X");
}

void moveEntityDownwardIfEnemy(Entity &entity, float seconds) {
    if (entity.getType() == ENEMY) {
        float newY = entity.getY() + (FALL_SPEED * seconds);
        if ((int)newY != (int)entity.getY())
            clearPosition((int)entity.getX(), (int)entity.getY());
        entity.setY(newY);
    }
}

void moveEnemiesDownward(float seconds) {
    for (auto entity: entities) {
        moveEntityDownwardIfEnemy(*entity, seconds);
    }
}

Entity *getEntityAtPosition(int x, int y) {
    auto result = std::find_if(entities.begin(), entities.end(), [=] (const Entity *entity) {
        return entity->getX() == x && entity->getY() == y;
    });
    return result == entities.end() ? nullptr : *result;
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
        case VK_CONTROL: {
            std::random_device rd;
            std::uniform_int_distribution<int> distribution(SCREEN_LEFT, SCREEN_RIGHT);
            spawnEntity(ENEMY, distribution(rd), 1);
            break;
        }
        default:
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

static int updates = 0;
void update(float deltaTime) {
    moveEnemiesDownward(deltaTime);
    std::for_each(entities.begin(), entities.end(), [deltaTime] (Entity *entity) {
        float x = entity->getX();
        float y = entity->getY();
        entity->update(deltaTime);
        if ((int)entity->getY() != (int)y || (int)entity->getX() != (int)x)
            clearPosition(x, y);
    });
    //    std::for_each(bullets.begin(), bullets.end(), [deltaTime] (Bullet &bullet) { Bullet::update(deltaTime); });
    updates++;
}

void enterGameLoop() {
    auto previousTime = std::chrono::high_resolution_clock::now();
    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;

        handleInput();
        update(deltaTime.count());
        drawEntities();

        previousTime = currentTime;
        setStatusMessage("entities: %i\tdelta: %f\tups: %f", entities.size(), deltaTime.count(), 1/deltaTime.count());
    }
}

int main() {
    player = {PLAYER, 10, 10, Color::GREEN};

    enableVirtualTerminalProcessing();
    printf(CSI "?25l"); // Hide the cursor
    printf(CSI "?1049h");

    enterGameLoop();
}