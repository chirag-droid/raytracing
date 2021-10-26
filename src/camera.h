#pragma once

#include "utility.h"

class Camera {
private:
    // private class members
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
public:
    Camera(
      Point3 lookfrom, // the point to look from
      Point3 lookat, // the point to look to
      Vec3 vup, // the rotation around the lookat-lookfrom axis
      double vfov, // vertical field of view in degrees
      double aspect_ratio // the aspect ration for the camera
    ) {
        // The view port dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        // A unit vector which specifies the direction of lookfrom with respect to lookat
        auto w = unit_vector(lookfrom - lookat);

        // u, v are both unit vectors
        // refer this diagram https://raytracing.github.io/images/fig-1.16-cam-view-up.jpg
        // u will result in a vector that is perpendicular to both vup and w
        auto u = unit_vector(cross(vup, w));
        // v will result in a vector that is perpendicular to both w and u
        auto v = cross(w, u);

        // Origin point is same as the lookfrom point
        origin = lookfrom;

        // horizontal and vertical vectors
        horizontal = viewport_width * u;
        vertical = viewport_height * v;

        /**
          * The lower left corner is the lower left corner of the scene.
          * by adding 1/2 horizontal and subtracting we can get the lower left corner. 
          * 
          * We subtract w so that it places the lower left corner at right distance
          * as specified by the lookfrom and lookat
          **/
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    }

    Ray get_ray(double s, double t) const {
        // Remember we had a lower left corner on the view port. We can simply
        // Add the s, t value to it, if we chose upper right corner we would have to
        // subtract these values instead of adding that totally depends on you
        // Also we can't just simply add t, s values, we will have to scale them
        // to use the fit view so we multiply by horizontal and vertical vectors
        return Ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    }
};
