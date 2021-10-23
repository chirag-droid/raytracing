#pragma once

#include <iostream>
#include "utility.h"

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    // Divide the color by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    auto r = scale * pixel_color[0];
    auto g = scale * pixel_color[1];
    auto b = scale * pixel_color[2];

    // Write the translated [0,255] value of each color component.
    // as colour values range from 0 to 1 we clamp them to 0 to 0.999
    // and then multiply by 256
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
