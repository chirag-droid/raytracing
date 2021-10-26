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

/** 
 * Clear materials such as water, glass, and diamonds are dielectrics.
 * When a light ray hits them, it splits into a reflected ray and a refracted ray.
 * We’ll handle that by randomly choosing between reflection or refraction,
 * and only generating one scattered ray per interaction.
 **/
class Dielectric : public Material {
public:
    // ir specifies the index of refraction
    double ir;
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered
    ) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        // change the input ray to unit vector bcs refract expects us to have unit vectors
        Vec3 unit_direction = unit_vector(r_in.direction());

        // dot product of two vector is the product of their length and cos of the angles
        // as we are using unit vectors, finding the dot product gets us the cos of theta
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        // This is basically derived from sin^2 + cos^2 = 1
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        // If the product of refraction ration and sin theta is greater than 1
        // thhen the ray can not refract bcs sin of an angle can not be greater than 1
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        // direction is the direction of the scattered ray
        Vec3 direction;

        // If the ray can not refract, direction is reflected ray else refract
        if (cannot_refract)
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        // scattered is the ray between the hit point and the dirction of scattered ray
        scattered = Ray(rec.p, direction);
        return true;
    }
};