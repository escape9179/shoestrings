//
// Created by tr3lo on 5/22/2023.
//

#ifndef CPPTEST_COLOR_H
#define CPPTEST_COLOR_H


struct Color {
    int r, g, b;

    static Color getRed();

    static Color getGreen();

    static Color getBlue();

    static Color getWhite();
};

#endif //CPPTEST_COLOR_H
