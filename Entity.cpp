//
// Created by tr3lo on 5/22/2023.
//

#include "Entity.h"
#include "Color.h"

int Entity::lastId = 0;

Entity::Entity() : color{Color::MAGENTA}, type{PLAYER} {}

Entity::Entity(EntityType type) : Entity(type, 0, 0, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y) : Entity(type, x, y, getColorOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y, Color color) : type{type}, x{x}, y{y}, color{color} {
    id = lastId++;
}

Color Entity::getColorOfEntity(EntityType type) const {
    if (type == PLAYER) return Color::WHITE;
    else if (type == ENEMY) return Color::RED;
    else if (type == BULLET) return Color::BLUE;
    else return Color::WHITE;
}