#include "Color.hpp"

Color::Color(char red_, char green_, char blue_) {
    red = red_;
    green = green_;
    blue = blue_;
}

char Color::getRed() {
    return red;
}

char Color::getGreen() {
    return green;
}

char Color::getBlue() {
    return blue;
}

Color::~Color() {
}

