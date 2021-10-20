#include <iostream>

#include "vec3.h"
#include "ray.h"
#include "color.h"

Color ray_color(const Ray& r);

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / aspect_ratio);

    /**
     * The aspect ratio of the view port should be same as the image
     * You can definetly render without a view port 
     * but then you will have to adjust your focal length and
     * all those variables again and again if you change the image
     * width and height. Its always a better choice to choose a reference
     * view. So you get the same render for every image width and height
     * 
     * The view port is like a virtual screen.
     **/
    // The view port dimensions
    auto VIEW_HEIGHT = 2.0;
    auto VIEW_WIDTH = VIEW_HEIGHT * aspect_ratio;

    /**
     * So our task was to create a ray tracer. For that we need a
     * origin point from where the rays fall into the scene.
     * To create a ray tracer we will create rays from the origin and
     * make them fall on every point on the scene. For now we are defining
     * origin, and the horizontal and vertical vectors.
     * Later on we will see how to transverse the image.
     **/

    // Origin point x=0, y=0, z=0
    auto origin = Point3(0, 0, 0);

    // You can choose the origin anything you like
    // 0, 0, 0 will place it at the center

    // horizontal and vertical vectors
    auto horizontal = Vec3(VIEW_WIDTH, 0, 0);
    auto vertical = Vec3(0, VIEW_HEIGHT, 0);

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

    // Set focal length
    auto focal_length = 2;
    auto lower_left_corner = (origin - horizontal/2 - vertical/2) - Vec3(0, 0, focal_length);

    // PPM image headers
    std::cout << "P3" << '\n'
        << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << '\n'
        << 255 << '\n';
    
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

            // U specifies the horizontal distance, and goes from 0.0 to 1.0
            auto u = double(i) / (IMAGE_WIDTH-1);
            // v specifies the vertical distance, and goes from 1.0 to 0.0
            auto v = double(j) / (IMAGE_HEIGHT-1);

            // Remember we had a lower left corner on the view port. We can simply
            // Add the u, v value to it, if we chose upper right corner we would have to
            // subtract these values instead of adding that totally depends on you
            // Also we can't just simply add v, u values, we will have to scale them
            // to use the fit view so we multiply by horizontal and vertical vectors

            /**
             * Try changing the focal length to 0 so that you can clearly see the dot
             * from where the rays are coming from
             * Now try changing the horizontal and vertical values
             * You will see the origin is not at center anymore bcs it is not
             * correctly fitting the view port
             **/

            // Create the ray
            Ray r(origin, lower_left_corner + horizontal * u + vertical * v - origin);
            // Get the corresponding pixel color for the ray
            Color pixel_color = ray_color(r);

            // Output the color
            write_color(std::cout, pixel_color);
        }
    }

    // Print the done message
    std::cerr << '\n' << "Done" << '\n';
}

Color ray_color(const Ray& r) {
    // Get the unit vector from the ray
    // Think of a unit vector as a vector which gets
    // us the direction of the vector by dividing by the length

    // Note: that each value in unit_direction ranges from -1 to 1
    Vec3 unit_direction = unit_vector(r.direction());

    // Note that y for unit vector for pixels at the top will be more
    // y for unit vector at center will be 0
    // y for unit vector at bottom will be less than 0

    // t goes from 1 to 0 vertical direction
    // try changing unit_direction[1] to [0] what do you see?
    auto t = 0.5 * (unit_direction[1] + 1);

    // Start of the gradiend (Blue)
    Color startColor = Color(0, 0, 1);
    // End of the gradient (Red)
    Color endColor = Color(1, 0, 0);

    // We should see a gradient where at the top is red, and
    // at the bottom is blue

    // For t=1 it will return endColor and for t=0 it will return startColor
    // This is also known as a linear blend
    return startColor + t * (endColor - startColor);
}
