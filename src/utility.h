#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>
#include <memory>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Common header files
#include "ray.h"
#include "vec3.h"

// Constants
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// Utility functions
inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

// Some random number generation utilities
inline double random_double() {
    // Returns a random real in [0, 1)
    // rand function returns result between 0 and RAND_MAX
    // as we want the result between [0, 1) we divide by RAND_MAX + 1
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // return a random number in range [min, max)
    // as we know random double returns random in range [0, 1)
    // we can use that to generate random between [min, max)
    return min + (max - min) * random_double();
}
