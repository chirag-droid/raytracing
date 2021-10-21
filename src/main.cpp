#include <iostream>
#include <cmath>

#include "vec3.h"
#include "ray.h"
#include "color.h"

double hit_sphere(const Point3& center, double radius, const Ray& r);
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
    auto focal_length = 1;
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

double hit_sphere(const Point3& center, double radius, const Ray& r) {
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
        return -1;
    return (-half_b - sqrt(discriminant)) / a;
}

Color ray_color(const Ray& r) {
    // Center of the sphere
    Point3 center(0, 0, -1);

    // Find at what point the ray hit the sphere
    auto t = hit_sphere(center, 0.5, r);
    if (t >= 0.0) {
        // Find the normal vector
        // r.at(t) is the point at which the ray hit the sphere
        // The point on the sphere - center gives us the normal vector

        Vec3 normal = unit_vector(r.at(t) - center);

        // normal has value ranging from -1 to 1 so we add 1 to it
        // and divide by 2 to make it in range 0 to 1
        return 0.5*Color(normal[0]+1, normal[1]+1, normal[2]+1);
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
    t = 1 - (0.5 * (unit_direction[1] + 1));

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
