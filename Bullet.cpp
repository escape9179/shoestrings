//
// Created by tr3lo on 5/25/2023.
//

#include <cstdlib>
#include "Bullet.h"

const float Bullet::SPEED = 10.0f;

void Bullet::update(float deltaTime) {
    y = y - (SPEED * deltaTime);
}