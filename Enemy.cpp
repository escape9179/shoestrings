//
// Created by tr3lo on 5/25/2023.
//

#include <cstdio>
#include "Enemy.h"
#include "macros.h"
#include "Console.h"

const float Enemy::SPEED = 2.0f;

void Enemy::update(float deltaTime) {
    y = y + (SPEED * deltaTime);
}

void Enemy::draw() const {
    Console::setCursorPosition(x, y);
    printf(CSI "38;2;%i;%i;%im", color.r, color.g, color.b);
    printf("\u2588");
}

CollisionResult Enemy::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::PLAYER) return CollisionResult::DESTROY_BOTH;
    if (entity->getType() == EntityType::BULLET) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}
