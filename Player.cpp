//
// Created by tr3lo on 5/28/2023.
//

#include <cstdio>
#include "Player.h"
#include "Console.h"
#include "macros.h"

CollisionResult Player::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::ENEMY) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}

void Player::draw() const {
    Console::setCursorPosition(x, y);
    printf(CSI "38;2;%i;%i;%im", color.r, color.g, color.b);
    printf("\u2588");
}
