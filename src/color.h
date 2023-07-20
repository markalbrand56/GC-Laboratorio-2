#pragma once

#include <iostream>

class Color {
public:
    unsigned char r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(unsigned char red, unsigned char green, unsigned char blue);

    static unsigned char clamp(unsigned char value);

    friend std::ostream& operator<<(std::ostream& os, const Color& color);

    Color operator+(const Color& other) const;

    Color operator*(float value) const;
};
