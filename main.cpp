#include <iostream>

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // PPM image headers
    std::cout << "P3" << '\n'
        << image_width << ' ' << image_height << '\n'
        << 255 << '\n';
    
    // Iterate over height and width
    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            // Colors are usually represented by color values from 0.0 to 1.0
    
            // Make r go from 0.0 to 1.0
            auto r = double(i) / (image_width-1);
            // Make g go from 1.0 to 0.0
            auto g = double(j) / (image_height-1);
            auto b = 0.25;

            /**
             * This also means that r is 0 at the left side,
             * g is 0 at the bottom so we can expect a bluish shade
             * at bottom left when r, g is 0 and blue still has some value
             **/

            /**
             * As we said colour values go from 0.0 to 1.0 so we somehow 
             * need to make them go from 0 to 255 integer values because
             * thats what ppm file format supports
             **/
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            // Print the pixel values like 255 0 255
            std::cout << ir << ' '
                << ig << ' '
                << ib << '\n';
        }
    }
}