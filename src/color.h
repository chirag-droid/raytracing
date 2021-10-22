#pragma once

#include <iostream>
#include "vec3.h"

void write_color(std::ostream &out, Color pixel_color) {
    // colour values go from 0.0 to 1.0 so we somehow 
    // need to make them go from 0 to 255 integer values because
    // thats what ppm file format supports
    pixel_color *= 255.999;
    out << static_cast<int>(pixel_color[0]) << ' '
        << static_cast<int>(pixel_color[1]) << ' '
        << static_cast<int>(pixel_color[2]) << '\n';
}
