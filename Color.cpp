//
// Created by tr3lo on 5/22/2023.
//

#include "Color.h"

Color Color::getRed() {
    static Color red(255, 0, 0);
    return red;
}

Color Color::getGreen() {
    static Color green(0, 255, 0);
    return green;
}

Color Color::getBlue() {
    static Color blue(0, 0, 255);
    return blue;
}

Color Color::getWhite() {
    static Color white(255, 255, 255);
    return white;
}