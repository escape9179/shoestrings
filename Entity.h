//
// Created by tr3lo on 5/22/2023.
//

#ifndef CPPTEST_ENTITY_H
#define CPPTEST_ENTITY_H


#include "Color.h"
#include "EntityType.h"

char constexpr PLAYER_CHAR = 'o';
char constexpr ENEMY_CHAR = 'e';
char constexpr BULLET_CHAR = 'x';
char constexpr QUESTION_MARK_CHAR = '?';

using enum EntityType;

class Entity {
    static int lastId;
    int id = 0;
    float x = 0;
    float y = 0;
    char character;
    Color color;
    EntityType type;

public:
    Entity(EntityType type);

    Entity(EntityType type, float x, float y);

    Entity(EntityType type, float x, float y, Color color);

    int getId() const;

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    char getChar() const;

    Color getColor() const;

    EntityType getType() const;

    virtual void update(float deltaTime);

    bool operator==(const Entity &other) const;

    Entity();

private:
    char getCharacterByEntityType(EntityType type) const;

    Color getColorOfEntity(EntityType type) const;
};


#endif //CPPTEST_ENTITY_H
