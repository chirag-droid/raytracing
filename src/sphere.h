#include "hittable.h"
#include "vec3.h"
#include "ray.h"

class Sphere : public Hittable {
public:
    Point3 center;
    double radius;
public:
    Sphere() {}
    Sphere(Point3 cen, double r)
        : center(cen), radius(r) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec)
        const override;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    /**
     * Any point on the sphere should satisfy the following mathematical property
     * (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2= r^2 or,
     * (Vec(x, y, z) - Vec(Cx, Cy, Cz)) . (Vec(x, y, z) - Vec(Cx, Cy, Cz)) or
     * (P - C) . (P - C) = r^2
     * 
     * Any point P that satisfy this equation is on the sphere.
     * And point P is the point on the ray
     * ray.at(t) = P
     * ray_origin + t * direction = P
     * 
     * (A + t * b - C) . (A + t * b - C) = r^2
     * Let's apply the vector algebra
     * (A-C).(A-C) + (A-c).t*b + (t*b).(A-C) + (t*b).(t*b) = r^2
     * (A-C).(A-C) + 2tb.(A-C) + t^2 (b . b) = r^2
     * t^2(b.b) + 2tb.(A-C) + (A-C).(A-C) - r^2 = 0
     * 
     * We already know all other values, we need to solve for t
     * Notice: The equation is quadratic in t
     * If the discriminant is > 0 means the roots are real
     **/

    // oc is our A-C term
    Vec3 oc = r.origin() - center;

    auto a = r.direction().lengthSquared(); // a.a = a.length_squared()
    auto half_b = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius * radius;

    // Find the discriminant of the equation
    auto discriminant = half_b*half_b - a*c;

    // If the discriminant is less than 0, then roots are unreal and return -1
    if (discriminant < 0)
        return false;
    
    // Find the nearest root that lies in the acceptable range
    // Square root of the discriminant
    auto sqrt_d = sqrt(discriminant);

    // remember there are two roots of a quadratic equation
    // so we need to check for both whether they are in range
    auto root = (-half_b - sqrt(discriminant)) / a;
    if (root < t_min || t_max < root) {
        // Check for other root
        root = (-half_b + sqrt_d) / a;

        if (root < t_min || t_max < root)
            return false;
    }


    rec.t = root;
    rec.p = r.at(rec.t);

    // Calculate the normal and set normal
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
