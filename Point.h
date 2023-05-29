//
// Created by tr3lo on 5/29/2023.
//

#ifndef CPPTEST_POINT_H
#define CPPTEST_POINT_H


struct Point {
    int x;
    int y;

    Point(int x, int y) : x{x}, y{y} {}

    bool operator==(Point other) const {
        return x == other.x && y == other.y;
    }
};


#endif //CPPTEST_POINT_H
