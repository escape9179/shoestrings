//
// Created by tr3lo on 5/25/2023.
//

#ifndef CPPTEST_BULLET_H
#define CPPTEST_BULLET_H

#include "Entity.h"

class Bullet : public Entity {
    static const float SPEED;
public:
    Bullet(float x, float y) : Entity(BULLET, x, y) {}

    void update(float deltaTime) override;
};


#endif //CPPTEST_BULLET_H