#include <windows.h>
#include <cwchar>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "macros.h"
#include "Console.h"

int constexpr READ_BUFFER_SIZE = 32;
int constexpr VK_U = 0x55;
int constexpr VK_O = 0x4F;
int constexpr SCREEN_BOTTOM = 29;
int constexpr SCREEN_RIGHT = 120;
int constexpr SCREEN_TOP = 1;
int constexpr SCREEN_LEFT = 1;
int constexpr STATUS_MESSAGE_ROW = SCREEN_BOTTOM + 1;

std::vector<Entity *> entities;

Entity *player;
int score = 0;
bool gameOver = false;

template<typename... Args>
void setStatusMessage(const char *message, Args... args) {
    static int lastMessageLen = 0;
    printf(CSI "0m");
    printf(CSI "%i;%iH", STATUS_MESSAGE_ROW, SCREEN_LEFT);
    char buffer[SCREEN_RIGHT];
    int messageLen = sprintf(buffer, message, args...);
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
        case ENEMY:
            entities.push_back(new Enemy(x, y));
            break;
        default:
            entities.push_back(new Player(x, y));
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

void drawEntities() {
    player->draw();
    std::for_each(entities.begin(), entities.end(), [](const auto entity) { entity->draw(); });
}

void moveEntity(Entity &entity, int x, int y) {
    if (SCREEN_LEFT > x || x > SCREEN_RIGHT) return;
    if (SCREEN_TOP > y || y > SCREEN_BOTTOM) return;
    entity.erase();
    entity.setX(x);
    entity.setY(y);
}

void movePlayer(int x, int y) {
    moveEntity(*player, x, y);
}

void shootBullet() {
    int x = (int) player->getX();
    int y = (int) player->getY() - 1;
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
            movePlayer(player->getX() + 1, player->getY());
            break;
        case VK_O:
            movePlayer(player->getX() - 1, player->getY());
            break;
        case VK_SHIFT:
            // TODO Adjust entity speed?
            break;
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

bool entityCollidedWithScreenBorder(Entity *entity) {
    float x = (int) entity->getX();
    float y = (int) entity->getY();
    return SCREEN_LEFT > x || x > SCREEN_RIGHT || SCREEN_TOP > y || y > SCREEN_BOTTOM;
}

std::vector<Entity *> *getOtherEntitiesAtPositionOf(Entity *entity) {
    auto *otherEntities = new std::vector<Entity *>();
    for (Entity *otherEntity: entities) {
        if (*otherEntity == *entity) continue;
        if (entity->getPoint() == otherEntity->getPoint()) otherEntities->push_back(otherEntity);
    }
    return otherEntities;
}

void update(float delta) {
    std::set<Entity *> entitiesForRemoval;
    for (int i = 0; i < entities.size(); i++) {
        int x1 = entities[i]->getX();
        int y1 = entities[i]->getY();
        Point nextPosition = entities[i]->getNextPosition(delta);
        int x2 = nextPosition.x;
        int y2 = nextPosition.y;

        if (x1 != x2 || y1 != y2) {
            entities[i]->erase();
        }

        entities[i]->update(delta);

        if (entityCollidedWithScreenBorder(entities[i])) {
            entitiesForRemoval.insert(entities[i]);
        }

        auto entitiesAtSamePosition = getOtherEntitiesAtPositionOf(entities[i]);
        for (Entity *otherEntity: *entitiesAtSamePosition) {
            CollisionResult result = entities[i]->getResultFromCollisionWith(otherEntity);
            switch (result) {
                case CollisionResult::DESTROY_SELF:
                    entitiesForRemoval.insert(entities[i]);
                    break;
                case CollisionResult::DESTROY_OTHER:
                    entitiesForRemoval.insert(otherEntity);
                    break;
                case CollisionResult::DESTROY_BOTH:
                    entitiesForRemoval.insert(entities[i]);
                    entitiesForRemoval.insert(otherEntity);
                    if (entitiesForRemoval.contains(player))
                        gameOver = true;
                    if (entities[i]->getType() == EntityType::ENEMY && otherEntity->getType() == EntityType::BULLET)
                        score++;
                    break;
                case CollisionResult::DO_NOTHING:
                    break;
            }
        }

        delete entitiesAtSamePosition;
    }

    if (entities.empty() || entitiesForRemoval.empty())
        return;

    for (int i = 0; i < entities.size(); i++) {
        for (auto &j: entitiesForRemoval) {
            if (*j == *entities[i]) {
                j->erase();
                entities.erase(entities.begin() + i);
            }
        }
    }
}

void enterGameLoop() {
    auto previousTime = std::chrono::high_resolution_clock::now();
    while (true) {
        if (!gameOver) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTime = currentTime - previousTime;

            handleInput();
            update(deltaTime.count());
            drawEntities();

            previousTime = currentTime;
            setStatusMessage("UPS: %5.2f\tScore: %i", 1 / deltaTime.count(), score);
        } else setStatusMessage("GAME OVER");
    }
}

int main() {
    player = new Player(SCREEN_RIGHT / 2, SCREEN_BOTTOM);
    entities.push_back(player);

    enableVirtualTerminalProcessing();
    printf(CSI "?25l"); // Hide the cursor
    printf(CSI "?1049h");

    enterGameLoop();
}