//
// Created by tr3lo on 5/25/2023.
//

#include <cstdlib>
#include <cstdio>
#include "Bullet.h"
#include "Console.h"
#include "macros.h"

void Bullet::update(float deltaTime) {
    y = y - (speed * deltaTime);
}

CollisionResult Bullet::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::ENEMY) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}

void Bullet::draw() const {
    Console::setCursorPosition(x, y);
    Console::setForegroundColor(color.r, color.g, color.b);
    Console::enterLineDrawingMode();
    printf("x");
    Console::enterAsciiMode();
    Console::resetTextFormatting();
}

void Bullet::erase() const {
    Console::erasePosition(x, y);
}

Point Bullet::getNextPosition(float delta) const {
    return {(int) x, (int) (y - (speed * delta))};
}
