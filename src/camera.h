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
      double vfov,
      double aspect_ratio
    ) {
        // The view port dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        /**
          * So our task was to create a ray tracer. For that we need a
          * origin point from where the rays fall into the scene.
          * To create a ray tracer we will create rays from the origin and
          **/

        // Origin point x=0, y=0, z=0
        origin = Point3(0, 0, 0);

        // You can choose the origin anything you like
        // 0, 0, 0 will place it at the center

        // horizontal and vertical vectors
        horizontal = Vec3(viewport_width, 0, 0);
        vertical = Vec3(0, viewport_height, 0);

        // Set focal length
        auto focal_length = 1.0;

        /**
          * The lower left corner is the lower left corner of the scene.
          * The lower left corner is calculated by adding 1/2 horizontal and subtracting
          * 1/2 vertical. It could have been upper left corner or something but we are choosing
          * lower left corner.
          * Now why are we subtracting the focal length from the z value? It just places the scenes's
          * lower left corner focal length away from the origin of the ray.
          * 
          * Try changing the value of focal_length to very small, what happens? You should
          * see a dot forming on the image from where the rays are emerging. Because the 
          * origin is close to the scene
          * 
          * A large focal length just places the origin away from the scene.
          * 
          * Basically, focal length specifies the distance between the origin and the scene
          **/
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);
    }

    Ray get_ray(double u, double v) const {
        // Remember we had a lower left corner on the view port. We can simply
        // Add the u, v value to it, if we chose upper right corner we would have to
        // subtract these values instead of adding that totally depends on you
        // Also we can't just simply add v, u values, we will have to scale them
        // to use the fit view so we multiply by horizontal and vertical vectors
        return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }
};
