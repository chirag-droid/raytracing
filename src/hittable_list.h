#pragma once

#include "ray.h"
#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;
public:
    // Class constructors
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) {
        add(object);
    }

    // Erase all elements from the hittable list
    void clear() { objects.clear(); }
    // Add an element to hittable list
    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    // As we did in other hittables the function returns true if the object is hit
    // else false and also change the hit_record.
    // We do the same thing here except for the fact that we check for every object
    // in the objects vector and return accordingly

    // a temp record
    hit_record temp_rec;
    bool hit_anything = false;

    // As we only care about the objects that are closest to the ray
    // When we hit an object we change the closest_so_far, and only care
    // about the closest object
    auto closest_so_far = t_max;

    // iterate over all the object in objects
    for (const auto& object : objects) {
        // If the object gets hit
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            // Change hit anything to true
            hit_anything =  true;
            // Update closest so far, as in next iteration we only care
            // about if the object is more closer than the previous object
            closest_so_far = temp_rec.t;

            // change the rec to temp_record
            rec = temp_rec;
        }
    }

    // return hit_anythin
    return hit_anything;
}
