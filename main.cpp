#include <windows.h>
#include <cwchar>

#define ESC "\x1b"
#define CURSOR_UP(a) "["#a"A"
#define CURSOR_DOWN(a) "["#a"B"
#define CURSOR_LEFT(a) "["#a"D"
#define CURSOR_RIGHT(a) "["#a"C"

int constexpr READ_BUFFER_SIZE = 32;
int constexpr UPDATES_PER_SECOND = 1000 / 60;
int constexpr VK_U = 0x55;
int constexpr VK_E = 0x45;
int constexpr VK_O = 0x4F;

bool enableVirtualTerminalProcessing();

void processKeyEvent(KEY_EVENT_RECORD keyEventRecord);

int main() {
    HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    enableVirtualTerminalProcessing();
    while (true) {
        Sleep(UPDATES_PER_SECOND);
        INPUT_RECORD inputRecords[READ_BUFFER_SIZE];
        DWORD numEventsRead;
        PeekConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
        if (numEventsRead == 0)
            continue;
        ReadConsoleInput(inputHandle, inputRecords, READ_BUFFER_SIZE, &numEventsRead);
        for (int i = 0; i < numEventsRead; i++) {
            switch (inputRecords[i].EventType) {
                case KEY_EVENT: {
                    processKeyEvent(inputRecords[i].Event.KeyEvent);
                    break;
                }
            }
        }
    }
}

bool enableVirtualTerminalProcessing() {
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (outputHandle == INVALID_HANDLE_VALUE)
        return false;
    DWORD consoleMode;
    if (!GetConsoleMode(outputHandle, &consoleMode))
        return false;
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(outputHandle, consoleMode))
        return false;
    return true;
}


void processKeyEvent(KEY_EVENT_RECORD keyEventRecord) {
    if (!keyEventRecord.bKeyDown)
        return;
    WORD keyCode = keyEventRecord.wVirtualKeyCode;
    switch (keyCode) {
        case VK_OEM_PERIOD:
            printf(ESC CURSOR_UP(1));
            break;
        case VK_U:
            printf(ESC CURSOR_RIGHT(1));
            break;
        case VK_E:
            printf(ESC CURSOR_DOWN(1));
            break;
        case VK_O:
            printf(ESC CURSOR_LEFT(1));
            break;
        default:
            printf("Pressed an unsupported key.\n");
            break;
    }
}