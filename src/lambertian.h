#include "utility.h"
#include "material.h"
#include "hittable.h"

class Lambertian : public Material {
public:
    // Class constructors
    Lambertian() {}

    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Ray& scattered
    ) const override;
};

bool Lambertian::scatter(const Ray& r_in, const hit_record& rec, Ray& scattered) const {
    // Scatter the ray within 1 unit sphere that is tangent to the hitpoint
    // rec.p is hitpoint
    auto scatter_direction = rec.p + random_in_hemisphere(rec.normal);

    // scattered ray is ray between hitpoint and the scatter direction
    scattered = Ray(rec.p, scatter_direction - rec.p);

    return true;
}