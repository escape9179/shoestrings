//
// Created by tr3lo on 5/29/2023.
//

#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include "macros.h"

namespace Console {
    void setCursorPosition(int x, int y);

    void erasePosition(int x, int y);

    void enterLineDrawingMode();

    void enterAsciiMode();

    void setForegroundColor(int r, int g, int b);

    void setBackgroundColor(int r, int g, int b);

    void setTextFormatting(int value);

    void resetTextFormatting();
}

#endif //CPPTEST_CONSOLE_H
