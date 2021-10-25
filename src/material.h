#pragma once

#include "utility.h"

struct hit_record;

class Material {
public:
    virtual bool scatter(
        const Ray& r_in, const hit_record& rec, Ray& scattered
    ) const = 0;
};
