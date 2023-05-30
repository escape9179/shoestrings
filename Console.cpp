//
// Created by tr3lo on 5/29/2023.
//

#include <cstdio>
#include "Console.h"
#include "macros.h"

void Console::setCursorPosition(int x, int y) {
    printf(CSI "%i;%iH", y, x);
}

void Console::erasePosition(int x, int y) {
    setCursorPosition(x, y);
    printf(CSI "1X");
}