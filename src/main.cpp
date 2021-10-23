#include <iostream>

#include "utility.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

Color ray_color(const Ray& r, const Hittable& , int max_depth);

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / aspect_ratio);

    // Number of samples to take for each pixel
    // When rendering a pixel, samples around the pixel will be taken
    // and then averaged to create a antialiased pixel
    const int SAMPLES_PER_PIXEL = static_cast<int>(
        clamp(9000000 / (IMAGE_WIDTH * IMAGE_HEIGHT), 1, 100)
    );

    // A high sample per pixel will slow down rendering. For smaller resolutions
    // more samples per pixel are needed and for high resolutions relatively
    // less samples per pixel are needed

    // So i just use a formula to determine the samples per pixel
    // I don't think its very good but works fine

    // Max depth is the ray bounce limit
    const int MAX_DEPTH = 50;

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

            // Create a color
            Color pixel_color(0, 0, 0);

            // Take SAMPLES_PER_PIXEL samples for each pixel
            for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
                // u specifies the horizontal distance, and goes from 0.0 to 1.0
                auto u = double(i + random_double()) / (IMAGE_WIDTH-1);
                // v specifies the vertical distance, and goes from 1.0 to 0.0
                auto v = double(j + random_double()) / (IMAGE_HEIGHT-1);

                // Get the ray from the camera
                Ray r = camera.get_ray(u, v);

                // Get the corresponding pixel color for the ray and the world
                pixel_color += ray_color(r, world, MAX_DEPTH);
            }

            // Output the color
            write_color(std::cout, pixel_color, SAMPLES_PER_PIXEL);
        }
    }

    // Print the done message
    std::cerr << '\n' << "Done" << '\n';
}

Color ray_color(const Ray& r, const Hittable& world, int depth) {
    // Create a record of the hits
    hit_record rec;

    // If we reach the max depth limit return black
    if (depth <= 0)
        return Color(0, 0, 0);

    // Check if the rays hit the world
    if (world.hit(r, 0.001, INF, rec)) {
        // If the rays hit, get the point within the
        // 1 unit sphere that is tangent to it

        // rec.p + rec.normal is the center of the one unit sphere
        // than is tangent to it
        // random_int_unit_sphere() generates a random point within a 1 unit
        // sphere we add it the center of it.
        Point3 target = rec.p + rec.normal + random_in_unit_sphere();

        // Create a ray between the hit point and the target
        // multiply by 0.5 bcs we want to reflect only 50% light
        // multipling by 1 will reflect 100% light
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth-1);
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
