//
// Created by tr3lo on 5/25/2023.
//

#include "Enemy.h"

const float Enemy::SPEED = 5.0f;

void Enemy::update(float deltaTime) {
    y = y + (SPEED * deltaTime);
}
