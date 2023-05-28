//
// Created by tr3lo on 5/28/2023.
//

#ifndef CPPTEST_PLAYER_H
#define CPPTEST_PLAYER_H


#include "Entity.h"

class Player : public Entity {
public:
    Player(float x, float y) : Entity(PLAYER, x, y) {

    }

private:
    void update(float deltaTime) override {

    }
};


#endif //CPPTEST_PLAYER_H
