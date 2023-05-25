//
// Created by tr3lo on 5/22/2023.
//

#include "Entity.h"
#include "Color.h"

int Entity::lastId = 0;

Entity::Entity() : character{QUESTION_MARK_CHAR}, color{Color::MAGENTA}, type{PLAYER} {}

Entity::Entity(EntityType type) : Entity(type, 0, 0, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y) : Entity(type, x, y, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y, Color color) : type{type}, x{x}, y{y}, color{color} {
    id = lastId++;
    character = getCharacterByEntityType(type);
}

int Entity::getId() const {
    return id;
}

float Entity::getX() const {
    return x;
}

void Entity::setX(float x) {
    this->x = x;
}

float Entity::getY() const {
    return y;
}

void Entity::setY(float y) {
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
    if (type == PLAYER) return Color::GREEN;
    else if (type == ENEMY) return Color::RED;
    else if (type == BULLET) return Color::BLUE;
    else return Color::WHITE;
}