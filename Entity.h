//
// Created by tr3lo on 5/22/2023.
//

#ifndef CPPTEST_ENTITY_H
#define CPPTEST_ENTITY_H


#include "Color.h"
#include "EntityType.h"
#include "Point.h"

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
    float speed = 0;
    Color color;
    EntityType type;

public:
    Entity(EntityType type);

    Entity(EntityType type, float x, float y);

    Entity(EntityType type, float x, float y, Color color, float speed);

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

    void setSpeed(float value) {
        speed = value;
    }

    float getSpeed() const {
        return speed;
    }

    virtual Point getNextPosition(float delta) const;

    virtual CollisionResult getResultFromCollisionWith(Entity *entity) const = 0;

    virtual void update(float deltaTime) = 0;

    virtual void draw() const = 0;

    virtual void erase() const = 0;

    bool operator==(const Entity &other) const {
        return id == other.id;
    }

    bool operator!=(const Entity &other) const {
        return id != other.id;
    }

    Entity();

private:
    Color getColorOfEntity(EntityType type) const;

    float getSpeedOfEntity(EntityType type) const;
};


#endif //CPPTEST_ENTITY_H
