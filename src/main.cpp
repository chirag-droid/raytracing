#include <iostream>

#include "vec3.h"
#include "color.h"

int main() {
    // Image dimensions
    auto aspect_ratio = 16.0 / 9.0;
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = static_cast<int>(IMAGE_WIDTH / aspect_ratio);

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
            // Colors are usually represented by color values from 0.0 to 1.0
    
            // Create pixel color with 3 values
            // first one representing r, 2nd one g, 3rd one b
            // r goes from 0.0 to 1.0 and g from 1.0 to 0.0
            Color pixel_color (
                double(i) / (IMAGE_WIDTH-1),
                double(j) / (IMAGE_HEIGHT-1),
                0.25
            );

            /**
             * This also means that r is 0 at the left side,
             * g is 0 at the bottom so we can expect a bluish shade
             * at bottom left when r, g is 0 and blue still has some value
             **/

            // output the color
            write_color(std::cout, pixel_color);
        }
    }

    // Print the done message
    std::cerr << '\n' << "Done" << '\n';
}