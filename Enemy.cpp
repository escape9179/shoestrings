//
// Created by tr3lo on 5/25/2023.
//

#include <cstdio>
#include "Enemy.h"
#include "macros.h"
#include "Console.h"

void Enemy::update(float deltaTime) {
    y = y + (speed * deltaTime);
}

void Enemy::draw() const {
    Console::setBackgroundColor(color.r, color.g, color.b);
    Console::setCursorPosition(x - 1, y);
    printf("  ");
    Console::setCursorPosition(x, y - 1);
    printf(" ");
    Console::resetTextFormatting();
}

void Enemy::erase() const {
    Console::erasePosition(x - 1, y);
    Console::erasePosition(x, y);
    Console::erasePosition(x, y - 1);
}

CollisionResult Enemy::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::PLAYER) return CollisionResult::DESTROY_BOTH;
    if (entity->getType() == EntityType::BULLET) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}

Point Enemy::getNextPosition(float delta) const {
    return { (int) x, (int) (y + (speed * delta))};
}
