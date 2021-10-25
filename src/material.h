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

class Metal : public Material {
public:
    Color albedo;
    double fuzz;
public:
    // Class constructors
    Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
    ) const override {
        // Get the vector after reflection from normal
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

        // scattered ray is the ray join the hit point to the reflected vector
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;

        // return true if the direction of scatter and normal is on same side
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};