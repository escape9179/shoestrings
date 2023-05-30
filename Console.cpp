//
// Created by tr3lo on 5/29/2023.
//

#include <cstdio>
#include "Console.h"

void Console::setCursorPosition(int x, int y) {
    printf(CSI "%i;%iH", y, x);
}

void Console::erasePosition(int x, int y) {
    setCursorPosition(x, y);
    printf(CSI "1X");
}

void Console::enterLineDrawingMode() {
    printf(ESC "(0");
}

void Console::enterAsciiMode() {
    printf(ESC "(B");
}

void Console::setForegroundColor(int r, int g, int b) {
    printf(CSI "38;2;%i;%i;%im", r, g, b);
}

void Console::setBackgroundColor(int r, int g, int b) {
    printf(CSI "48;2;%i;%i;%im", r, g, b);
}

void Console::setTextFormatting(int value) {
    printf(CSI "%im", value);
}

void Console::resetTextFormatting() {
    printf(CSI "0m");
}