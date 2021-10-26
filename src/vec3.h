#pragma once

#include <iostream>

#include "utility.h"

class Vec3 {
public:
    double vec3[3];
public:
    // Class constructors
    Vec3() : vec3{0, 0, 0} {}
    Vec3(double x, double y, double z) : vec3{x, y, z} {}

    // Vec3[index] will return the value of vec3 at that index
    double operator[] (int i) const { return vec3[i]; }
    double& operator[] (int i) { return vec3[i]; }
    /**
     * The above two functions may seem similar at first glance but are different
     * that first one will copy the value of vec3[i] to a new variable and return it
     * while second one will return the reference to the value at vec3[i]
     * For e.g.
     * 
     * Vec3 color();
     * double i = color[0];
     * i = 34.0;
     * Changing the value of i, does not affect value of color
     * 
     * double& j = color[0];
     * j = 34.0
     * Changing value of j, will change value of color
     **/

    // -Vec3 will return a new Vec3 object with negative values
    Vec3 operator- () const {
        return Vec3(-vec3[0], -vec3[1], -vec3[2]);
    }

    /**
     * += operator to add a vector to current vector
     * 
     *For e.g.
     * color1 = Vec3(1, 0, 0);
     * color2 = Vec3(1, 2, 1);
     * 
     * color1 += color2;
     * The value of color will be changed to {2, 2, 1}
     */
    Vec3& operator+= (const Vec3 &v) {
        vec3[0] += v[0];
        vec3[1] += v[1];
        vec3[2] += v[2];
        return *this;
    }

    // Multiplies the vector by the double
    Vec3& operator*= (const double t) {
        vec3[0] *= t;
        vec3[1] *= t;
        vec3[2] *= t;
        return *this;
    }

    // Divides the vector by the double
    Vec3& operator/= (const double t) {
        return *this *= 1/t;
    }

    // Calculate the length of the vector from the origin
    double length() const {
        return sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return vec3[0]*vec3[0] + vec3[1]*vec3[1] + vec3[2]*vec3[2];
    }

    // Some utility functions to generate random vectors
    inline static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(double min, double max) {
        return Vec3(
            random_double(min, max),
            random_double(min, max),
            random_double(min, max)
        );
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(vec3[0]) < s) && (vec3[1] < s) && (fabs(vec3[2]) < s);
    }
};

// Utility functions

// std::cout << vec3; will print the value of the vector
std::ostream& operator<< (std::ostream &out, const Vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

// Add two vectors
inline Vec3 operator+ (const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

// Subtract two vectors
inline Vec3 operator- (const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

// Multiply two vectors
inline Vec3 operator* (const Vec3 &u, const Vec3 &v) {
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

// Multiply the vector with a double
inline Vec3 operator* (double t, const Vec3 &v) {
    return Vec3(t*v[0], t*v[1], t*v[2]);
}

// Multiply the vector with the double
inline Vec3 operator* (const Vec3 &v, double t) {
    return t * v;
}

// Divide the vector by the double
inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}


// https://www.math.ucla.edu/~josephbreen/Understanding_the_Dot_Product_and_the_Cross_Product.pdf

// Find the dot product between two vectors
inline double dot(const Vec3 &p1, const Vec3 &p2) {
    return p1[0] * p2[0]
         + p1[1] * p2[1]
         + p1[2] * p2[2];
}

// Find the cross product between two vectors
inline Vec3 cross(const Vec3 &p1, const Vec3 &p2) {
    return Vec3(
        p1[1] * p2[2] - p1[2] * p2[1],
        p1[2] * p2[0] - p1[0] * p2[2],
        p1[0] * p2[1] - p1[1] * p2[0]
    );
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

// Generate a random point within a sphere of 1 unit
inline Vec3 random_in_unit_sphere() {
    while (true) {
        // Generate a random point between -1 and 1
        // As random is static we use ::
        auto p = Vec3::random(-1, 1);

        // If the length from the origin to the point
        // is less 1 return p else continue

        // We should ideally use length function but we dont need to here
        // length function takes sqrt of the length squared
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

inline Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& center) {
    Vec3 in_unit_sphere = random_unit_vector();
    if (dot(in_unit_sphere, center) > 0.0)
        return in_unit_sphere;
    return -in_unit_sphere;
}

// Reflects a vector along a particular normal
Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v, n)*n;
}

Vec3 refract(const Vec3& uv, const Vec3& normal, double refractive_index) {
    // Find the cos theta, dot product of two vectors is
    // product of their length and the cos theta as normal and uv are expected
    // to be unit vector this will result in the cos of angle between them
    auto cos_theta = fmin(dot(-uv, normal), 1.0);

    // the following formulas correspond to
    // https://raytracing.github.io/books/RayTracingInOneWeekend.html scroll to snells law
    // trajectory vector perpendicular to the normal
    Vec3 r_perp = refractive_index * (uv + cos_theta*normal);
    // trajectory parallel parallel to the normal
    Vec3 r_parallel = -sqrt(fabs(1.0 - r_perp.lengthSquared())) * normal;

    // return the sum of the parallel and the perpendicular vector
    return r_perp + r_parallel;
}

// Type aliases for Vec3
using Point3 = Vec3;   // 3D point
using Color = Vec3;    // RGB color
