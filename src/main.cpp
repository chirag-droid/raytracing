#include <iostream>

#include "utility.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

Color ray_color(const Ray& r, const Hittable& world);

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / aspect_ratio);

    // PPM image headers
    std::cout << "P3" << '\n'
        << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << '\n'
        << 255 << '\n';

    // Camera
    Camera camera;

    // Create a hittable_list world with two sphere
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Iterate over height and width
    for (int j = IMAGE_HEIGHT-1; j >= 0; j--) {
        /**
         * We are outputting it as error because error output does not
         * get redirected to the file by default we use std::flush,
         * because otherwise this whole thing can get printed after
         * the program finishes, which is kinda useless
         * 
         * r character escape is for carriage return, which just
         * positions the cursor to the beginning
         **/
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < IMAGE_WIDTH; i++) {
            // Now we are iterating over every point on the scene

            // u specifies the horizontal distance, and goes from 0.0 to 1.0
            auto u = double(i) / (IMAGE_WIDTH-1);
            // v specifies the vertical distance, and goes from 1.0 to 0.0
            auto v = double(j) / (IMAGE_HEIGHT-1);

            // Get the ray from the camera
            Ray r = camera.get_ray(u, v);

            // Get the corresponding pixel color for the ray and the world
            Color pixel_color = ray_color(r, world);

            // Output the color
            write_color(std::cout, pixel_color);
        }
    }

    // Print the done message
    std::cerr << '\n' << "Done" << '\n';
}

Color ray_color(const Ray& r, const Hittable& world) {
    // Create a record of the hits
    hit_record rec;

    // Check if the rays hit the world
    if (world.hit(r, 0.0, INF, rec)) {
        // If the rays hit, get the normal at the hit point and
        // return the corresponding colours
        return 0.5 * Color(rec.normal + Color(1, 1, 1));
    }

    // Get the unit vector from the ray
    // Think of a unit vector as a vector which gets
    // us the direction of the vector by dividing by the length

    // Note: that each value in unit_direction ranges from -1 to 1
    Vec3 unit_direction = unit_vector(r.direction());

    // Note that y for unit vector for pixels at the top will be more
    // y for unit vector at center will be 0
    // y for unit vector at bottom will be less than 0

    // t goes from 0 to 1 vertical direction
    // try changing unit_direction[1] to [0] what do you see?
    auto t = 1 - (0.5 * (unit_direction[1] + 1));

    // Start of the gradiend (Sky blue)
    Color startColor(0.5, 0.7, 1.0);
    // End of the gradient (White)
    Color endColor(1, 1, 1);

    // We should see a gradient where at the top is sky blue, and
    // at the bottom is white

    // For t=1 it will return endColor and for t=0 it will return startColor
    // This is also known as a linear blend
    return startColor + t * (endColor - startColor);
}
