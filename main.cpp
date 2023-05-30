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
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;
int constexpr SCREEN_BOTTOM = 29;
int constexpr SCREEN_RIGHT = 120;
int constexpr SCREEN_TOP = 1;
int constexpr SCREEN_LEFT = 1;
int constexpr STATUS_MESSAGE_ROW = SCREEN_BOTTOM + 1;

std::vector<Entity *> entities;

Entity *player;
int score = 0;

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

void destroyEntity(Entity *entity) {
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

void drawBulletEntity(const Entity &entity) {
    printf(ESC "(0");
//    drawEntity(entity);
    entity.draw();
    printf(ESC "(B");
}

void drawEntities() {
    player->draw();
    for (const auto entity: entities) {
//        drawEntityByType(*entity);
        entity->draw();
    }
}

std::vector<Entity *> getEntitiesAtPosition(int x, int y) {
    std::vector<Entity *> entitiesAtPosition;
    for (auto const entity: entities) {
        if ((int) entity->getX() == x && (int) entity->getY() == y) {
            entitiesAtPosition.push_back(entity);
        }
    }
    return entitiesAtPosition;
}

void moveEntity(Entity &entity, int x, int y) {
    if (SCREEN_LEFT > x || x > SCREEN_RIGHT) return;
    if (SCREEN_TOP > y || y > SCREEN_BOTTOM) return;
    Console::erasePosition(entity.getX(), entity.getY());
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
        case VK_E:
            movePlayer(player->getX(), player->getY() + 1);
            break;
        case VK_O:
            movePlayer(player->getX() - 1, player->getY());
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

bool entityCollidedWithScreenBorder(Entity *entity) {
    float x = (int) entity->getX();
    float y = (int) entity->getY();
    return SCREEN_LEFT > x || x > SCREEN_RIGHT || SCREEN_TOP > y || y > SCREEN_BOTTOM;
}

void setEntityPositionAtIndex(int index, int x, int y) {
    entities[index]->setX(x);
    entities[index]->setY(y);
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
        entities[i]->update(delta);
        int x2 = entities[i]->getX();
        int y2 = entities[i]->getY();

        if (x1 != x2 || y1 != y2) {
            Console::setCursorPosition(x1, y1);
            printf(CSI "1X");
        }

        /* If the entity collided with the border of a screen then set the entities position to the last position it
         * was at before it collided with the border. */
        if (entityCollidedWithScreenBorder(entities[i])) {
//            setEntityPositionAtIndex(i, x1, y1);
            entitiesForRemoval.insert(entities[i]);
        }

        auto entitiesAtSamePosition = getOtherEntitiesAtPositionOf(entities[i]);
        for (Entity *otherEntity : *entitiesAtSamePosition) {
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
                    break;
                case CollisionResult::DO_NOTHING:
                    break;
            }
        }

        delete entitiesAtSamePosition;
    }

    for (int i = 0; i < entities.size(); i++) {
        for (auto & j : entitiesForRemoval) {
            if (*j == *entities[i]) {
                Console::erasePosition(j->getX(), j->getY());
                entities.erase(entities.begin() + i);
            }
        }
    }
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
        setStatusMessage("entities: %i\tdelta: %f\tups: %f", entities.size(), deltaTime.count(), 1 / deltaTime.count());
    }
}

int main() {
    player = new Player(10, 10);

    enableVirtualTerminalProcessing();
    printf(CSI "?25l"); // Hide the cursor
    printf(CSI "?1049h");

    enterGameLoop();
}