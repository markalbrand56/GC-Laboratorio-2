#include <iostream>
#include "color.h"

Color::Color(unsigned char red, unsigned char green, unsigned char blue)
        : r(clamp(red)), g(clamp(green)), b(clamp(blue)) {
}

unsigned char Color::clamp(unsigned char value) {
    if (value > 255) {
        return 255;
    } else if (value < 0) {
        return 0;
    }
    return value;
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
    os << "R: " << static_cast<int>(color.r)
       << " G: " << static_cast<int>(color.g)
       << " B: " << static_cast<int>(color.b);
    return os;
}

Color Color::operator+(const Color &other) {
    return Color(clamp(r + other.r), clamp(g + other.g), clamp(b + other.b));
}

Color Color::operator*(float value) {
    return Color(clamp(r * value), clamp(g * value), clamp(b * value));
}
