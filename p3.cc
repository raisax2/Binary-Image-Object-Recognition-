#include <iostream>
#include <fstream>
#include <cmath>
#include "image.h" 

using namespace std;
using namespace ComputerVisionProjects;

void ComputeObjectAttributes(const Image &labeled_image, const string &output_description, const string &output_image) {
    ofstream out_file(output_description);
    Image image_with_orientation = labeled_image; // Copy for orientation drawing

    // Loop over all labels (objects)
    for (int label = 1; label <= labeled_image.num_gray_levels(); ++label) {
        // Initialize attributes for the object
        int sum_x = 0, sum_y = 0, area = 0;
        double a = 0, b = 0, c = 0;

        // Scan the image for the current label
        for (size_t i = 0; i < labeled_image.num_rows(); ++i) {
            for (size_t j = 0; j < labeled_image.num_columns(); ++j) {
                if (labeled_image.GetPixel(i, j) == label) {
                    sum_x += i;
                    sum_y += j;
                    area++;

                    // Update second moments
                    a += i * i;
                    b += i * j;
                    c += j * j;
                }
            }
        }

        // Skip if no pixels found for this label
        if (area == 0) continue;

        // Calculate centroid
        double x_centroid = static_cast<double>(sum_x) / area;
        double y_centroid = static_cast<double>(sum_y) / area;

        // Calculate second moments relative to the centroid
        a = a / area - x_centroid * x_centroid;
        b = b / area - x_centroid * y_centroid;
        c = c / area - y_centroid * y_centroid;

        // Calculate orientation angle
        double theta = 0.5 * atan2(2 * b, a - c);
        double theta_degrees = theta * 180.0 / M_PI;

        // Calculate minimum moment of inertia (Emin)
        double e_min = a * sin(theta) * sin(theta) - b * sin(2 * theta) + c * cos(theta) * cos(theta);

        // Calculate maximum moment of inertia (Emax)
        double e_max = a * cos(theta) * cos(theta) + b * sin(2 * theta) + c * sin(theta) * sin(theta);

        // Calculate roundness
        double roundness = e_min / e_max;

        // Write to the output file
        out_file << label << " " << x_centroid << " " << y_centroid << " " << e_min << " " << area << " " << roundness << " " << theta_degrees << endl;

        // Drawing a longer and thicker orientation line
        const int line_length = 30;  // Length multiplier for longer lines
        const int thickness = 1;     // Thickness for the line (number of parallel lines)
        
        // Calculate the end coordinates of the main line
        int x_end = static_cast<int>(x_centroid + line_length * cos(theta));
        int y_end = static_cast<int>(y_centroid + line_length * sin(theta));

        // Draw the main line and parallel lines for thickness
        for (int offset = -thickness; offset <= thickness; ++offset) {
            // Adjust endpoints for thickness
            int x_offset = static_cast<int>(offset * cos(theta + M_PI / 2)); // Perpendicular offset
            int y_offset = static_cast<int>(offset * sin(theta + M_PI / 2)); // Perpendicular offset

            // Draw line with offset
            DrawLine(x_centroid + x_offset, y_centroid + y_offset, x_end + x_offset, y_end + y_offset, 128, &image_with_orientation);
        }
    }

    out_file.close();

    // Save the image with orientations
    WriteImage(output_image, image_with_orientation);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " {input labeled image} {output object descriptions} {output image}" << endl;
        return 0;
    }

    const string input_image_file(argv[1]);
    const string output_description_file(argv[2]);
    const string output_image_file(argv[3]);

    Image labeled_image;
    if (!ReadImage(input_image_file, &labeled_image)) {
        cout << "Error reading image " << input_image_file << endl;
        return 0;
    }

    ComputeObjectAttributes(labeled_image, output_description_file, output_image_file);

    return 0;
}
