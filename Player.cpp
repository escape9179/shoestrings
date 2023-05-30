//
// Created by tr3lo on 5/28/2023.
//

#include <iostream>
#include <cstdio>
#include "Player.h"
#include "Console.h"
#include "macros.h"

CollisionResult Player::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::ENEMY) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}

void Player::draw() const {
    Console::setCursorPosition(x, y - 1);
    Console::setForegroundColor(color.r, color.g, color.b);
    Console::enterLineDrawingMode();
    printf("x");
    Console::setCursorPosition(x, y);
    Console::enterAsciiMode();
    printf(CSI "47m");
    printf(" ");
    Console::setTextFormatting(0);
}

void Player::erase() const {
    Console::erasePosition(x, y - 1);
    Console::erasePosition(x, y);
}
