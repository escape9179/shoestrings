//
// Created by tr3lo on 5/22/2023.
//

#include "Entity.h"
#include "Color.h"

int Entity::lastId = 0;

Entity::Entity(EntityType type) : Entity(type, 0, 0, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, int x, int y) : Entity(type, x, y, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, int x, int y, Color color) : type{type}, x{x}, y{y}, color{color} {
    id = lastId++;
    character = getCharacterByEntityType(type);
}

int Entity::getId() const {
    return id;
}

int Entity::getX() const {
    return x;
}

void Entity::setX(int x) {
    this->x = x;
}

int Entity::getY() const {
    return y;
}

void Entity::setY(int y) {
    this->y =y;
}

char Entity::getChar() const {
    return character;
}

Color Entity::getColor() const{
    return color;
}

EntityType Entity::getType() const {
    return type;
}

bool Entity::operator==(const Entity &other) const {
    return id == other.id;
}

char Entity::getCharacterByEntityType(EntityType type) const {
    if (type == PLAYER) return PLAYER_CHAR;
    else if (type == ENEMY) return ENEMY_CHAR;
    else if (type == BULLET) return BULLET_CHAR;
    else return QUESTION_MARK_CHAR;
}

Color Entity::getColorOfEntity(EntityType type) const {
    if (type == PLAYER) return Color::getGreen();
    else if (type == ENEMY) return Color::getRed();
    else if (type == BULLET) return Color::getBlue();
    else return Color::getWhite();
}

