#pragma once

#include "hittable.h"
#include "utility.h"

struct hit_record;

class Material {
public:
    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
    ) const = 0;
};

class Lambertian : public Material {
public:
    Color albedo;
public:
    // Class constructors
    Lambertian(const Color& a) : albedo(a) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
    ) const override {
        // Scatter the ray within 1 unit sphere that is tangent to the hitpoint
        // rec.p is hitpoint
        auto scatter_direction = rec.p + random_in_hemisphere(rec.normal);

        // scattered ray is ray between hitpoint and the scatter direction
        scattered = Ray(rec.p, scatter_direction - rec.p);
        attenuation = albedo;

        return true;
    };
};
