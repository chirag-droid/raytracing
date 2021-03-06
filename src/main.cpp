#include "utility.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

Color ray_color(const Ray& r, const Hittable& , int max_depth);

HittableList random_scene() {
    HittableList world;

    // random elements
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.  add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Basic elements
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    auto metal = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    auto dielectric = make_shared<Dielectric>(1.5);
    auto lamber = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));

    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, metal));
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, dielectric));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, lamber));

    return world;
}

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / aspect_ratio);

    // Number of samples to take for each pixel
    // When rendering a pixel, samples around the pixel will be taken
    // and then averaged to create a antialiased pixel
    const int SAMPLES_PER_PIXEL = static_cast<int>(
        clamp(90000000 / (IMAGE_WIDTH * IMAGE_HEIGHT), 1, 500)
    );
    // 1440 width results in about 130 samples per pixel

    // A high sample per pixel will slow down rendering. For smaller resolutions
    // more samples per pixel are needed and for high resolutions relatively
    // less samples per pixel are needed.
    // So i just use a formula to determine the samples per pixel
    // I don't think its very good but works fine

    // Max depth is the ray bounce limit
    const int MAX_DEPTH = 50;

    // PPM image headers
    std::cout << "P3" << '\n'
        << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << '\n'
        << 255 << '\n';

    // Camera
    Point3 lookfrom(13, 2, 3);
    Point3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Create a hittable_list world
    auto world = random_scene();

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
    // we dont care about the rays at less than (t=0.001) because these rays
    // are reflecting the object they are reflecting
    if (world.hit(r, 0.001, INF, rec)) {
        Color attenuation;
        // The ray generated after hitting the world
        Ray scattered;

        // If the ray hits succesfully, scatter it using the material abstraction
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            // multiply by 0.5 bcs we want to reflect only 50% light
            // multipling by 1 will reflect 100% light
            // Return the color of the scattered ray
            return attenuation * ray_color(scattered, world, depth-1);
        
        return Color(0, 0, 0);
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
