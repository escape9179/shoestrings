//
// Created by tr3lo on 5/25/2023.
//

#include <cstdlib>
#include "Bullet.h"

const float Bullet::SPEED = 30.0f;

void Bullet::update(float deltaTime) {
    y = y - (SPEED * deltaTime);
}

CollisionResult Bullet::getResultFromCollisionWith(Entity *entity) const {
    if (entity->getType() == EntityType::ENEMY) return CollisionResult::DESTROY_BOTH;
    return CollisionResult::DO_NOTHING;
}
