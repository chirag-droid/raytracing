#pragma once

#include "ray.h"

struct hit_record {
    Point3 p;
    Vec3 normal;
    double t;
    // We somehow need to keep track of whether the ray
    // intersected on the front face or from the inside
    bool front_face;

    // If the ray intersects from outside the normal is against the ray
    // If the ray intersects from inside, the normal is with the ray
    // We can use dot product which will be less than 0 if the rays are against each other
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // Find the dot product between the ray and the normal
        // and if the rays are against each other(<0) set front_face true
        front_face = dot(r.direction(), outward_normal) < 0;
        
        // Set the normal
        // If the ray hit at front dont change sign, otherwise change
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};