#pragma once

#include <iostream>
#include <windows.h>

#define C_NORM enum_ccolor::WHITE
#define C_HEAD enum_ccolor::CYAN
#define C_OPT enum_ccolor::RED
#define C_USER enum_ccolor::BRIGHT_WHITE

enum class enum_ccolor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

void SetConsoleColor(enum_ccolor color = C_NORM) {

    // Funktion, um die Konsolenfarbe zu setzen

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}


void printUser() {

    SetConsoleColor(enum_ccolor::GREEN);
    std::cout << "\nuser@generator";
    SetConsoleColor(enum_ccolor::WHITE);
    std::cout << ":";
    SetConsoleColor(enum_ccolor::LIGHT_MAGENTA);
    std::cout << "~ $ ";
    SetConsoleColor(enum_ccolor::WHITE);
}
