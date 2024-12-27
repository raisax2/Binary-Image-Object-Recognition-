#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char* argv[]) {
    // argc has to be 4, otherwise it outputs this message
    if (argc != 4) {
        printf("Usage: %s {input gray-level image} {input gray-level threshold} {output binary image}\n", argv[0]);
        return 0;
    }

    const string input_file(argv[1]); // Input image
    int threshold = stoi(argv[2]);    // Threshold Value
    const string output_file(argv[3]); // Output image

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout << "Can't open file " << input_file << endl;
        return 0;
    }

    for (int i = 0; i < an_image.num_rows(); ++i) {
        for (int j = 0; j < an_image.num_columns(); ++j) {
            const int pixel_value = an_image.GetPixel(i, j);
            if (pixel_value < threshold) {
                an_image.SetPixel(i, j, 0);
            } else {
                an_image.SetPixel(i, j, 255);
            }
        }
    }

    if (!WriteImage(output_file, an_image)) {
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    return 0;
}
