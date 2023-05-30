//
// Created by tr3lo on 5/25/2023.
//

#include <cstdlib>
#include <cstdio>
#include "Bullet.h"
#include "Console.h"
#include "macros.h"

const float Bullet::SPEED = 30.0f;

void Bullet::update(float deltaTime) {
    y = y - (SPEED * deltaTime);
}

CollisionResult Bullet::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::ENEMY) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}

void Bullet::draw() const {
    Console::setCursorPosition(x, y);
    printf(CSI "38;2;%i;%i;%im", color.r, color.g, color.b);
    printf("\u2588");
}
