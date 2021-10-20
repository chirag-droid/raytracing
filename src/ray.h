#pragma once

#include "vec3.h"

class Ray {
public:
    // The origin is the starting point of the ray
    Point3 orig;
    // The direction specifies the direction of the ray
    Vec3 dir;
    // The origin and dir both defines a ray with a starting
    // point and a direction
public:
    // Class constructors
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction)
        : orig(origin), dir(direction)
    {}

    // Getter methods
    // Get the origin of the ray
    Point3 origin() const { return orig; }
    // Get the direction of the ray
    Vec3 direction() const { return dir; }

    // Get the point on the ray at specific distance
    // All we are doing is adding t distance to the origin
    // of the ray
    Point3 at(double t) const {
        return orig + t*dir;
    }
};
