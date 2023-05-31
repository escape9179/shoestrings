//
// Created by tr3lo on 5/22/2023.
//

#include "Entity.h"
#include "Color.h"

int Entity::lastId = 0;

Entity::Entity() : color{Color::MAGENTA}, type{PLAYER} {}

Entity::Entity(EntityType type) : Entity(type, 0, 0, getColorOfEntity(type), getSpeedOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y) : Entity(type, x, y, getColorOfEntity(type), getSpeedOfEntity(type)) {}

Entity::Entity(EntityType type, float x, float y, Color color, float speed) : type{type}, x{x}, y{y}, color{color}, speed{speed} {
    id = lastId++;
}

Point Entity::getNextPosition(float delta) const {
    return { (int) x , (int) y };
}

Color Entity::getColorOfEntity(EntityType type) const {
    if (type == PLAYER) return Color::WHITE;
    else if (type == ENEMY) return Color::RED;
    else if (type == BULLET) return Color::BLUE;
    else return Color::WHITE;
}

float Entity::getSpeedOfEntity(EntityType type) const {
    if (type == PLAYER) return 1.0f;
    else if (type == ENEMY) return 2.0f;
    else if (type == BULLET) return 30.0f;
    else return 0.0f;
}