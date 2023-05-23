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
    int x = 0;
    int y = 0;
    char character;
    Color color;
    EntityType type;

public:
    Entity(EntityType type);

    Entity(EntityType type, int x, int y);

    Entity(EntityType type, int x, int y, Color color);

    int getId() const;

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    char getChar() const;

    Color getColor() const;

    EntityType getType() const;

    bool operator==(const Entity &other) const;

private:
    char getCharacterByEntityType(EntityType type) const;

    Color getColorOfEntity(EntityType type) const;
};


#endif //CPPTEST_ENTITY_H
