//
// Created by tr3lo on 5/22/2023.
//

#ifndef CPPTEST_ENTITY_H
#define CPPTEST_ENTITY_H


#include "Color.h"
#include "EntityType.h"
#include "Point.h"

char constexpr PLAYER_CHAR = 'o';
char constexpr ENEMY_CHAR = 'e';
char constexpr BULLET_CHAR = 'x';
char constexpr QUESTION_MARK_CHAR = '?';

using enum EntityType;

enum CollisionResult {
    DESTROY_BOTH, DESTROY_SELF, DESTROY_OTHER, DO_NOTHING
};

class Entity {
    static int lastId;
protected:
    int id = 0;
    float x = 0;
    float y = 0;
    Color color;
    EntityType type;

public:
    Entity(EntityType type);

    Entity(EntityType type, float x, float y);

    Entity(EntityType type, float x, float y, Color color);

    int getId() const {
        return id;
    }

    Point getPoint() const {
        return { (int) x, (int) y };
    }

    float getX() const {
        return x;
    }

    void setX(float x) {
        this->x = x;
    }

    float getY() const {
        return y;
    }

    void setY(float y) {
        this->y = y;
    }

    Color getColor() const {
        return color;
    }

    EntityType getType() const {
        return type;
    }

    virtual CollisionResult getResultFromCollisionWith(Entity *entity) const = 0;

    virtual void update(float deltaTime) = 0;

    virtual void draw() const = 0;

    bool operator==(const Entity &other) const {
        return id == other.id;
    }

    bool operator!=(const Entity &other) const {
        return id != other.id;
    }

    Entity();

private:
    char getCharacterByEntityType(EntityType type) const;

    Color getColorOfEntity(EntityType type) const;
};


#endif //CPPTEST_ENTITY_H
