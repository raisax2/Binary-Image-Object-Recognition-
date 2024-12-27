#include "image.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace ComputerVisionProjects;
using namespace std;

// Structure to hold object description
struct ObjectDescription {
    int label;
    double center_row;
    double center_column;
    double Emin;
    double area;
    double roundness;
    double orientation;
};

// Function to compare two objects based on area, roundness, and orientation
bool CompareObjects(const ObjectDescription &img_object, const ObjectDescription &db_object) {
    const double kAreaThreshold = 0.15;      // Allowable difference in area (15%)
    const double kRoundnessThreshold = 0.2;  // Allowable difference in roundness
    const double kOrientationThreshold = 10; // Degrees difference allowed in orientation

    bool similar_area = fabs(img_object.area - db_object.area) < kAreaThreshold * db_object.area;
    bool similar_roundness = fabs(img_object.roundness - db_object.roundness) < kRoundnessThreshold;
    bool similar_orientation = fabs(img_object.orientation - db_object.orientation) < kOrientationThreshold;

    // Debugging print statements to help see comparison details
    cout << "Comparing objects: \n";
    cout << "Image object (Label: " << img_object.label << ") -> Area: " << img_object.area 
         << ", Roundness: " << img_object.roundness << ", Orientation: " << img_object.orientation << "\n";
    cout << "Database object (Label: " << db_object.label << ") -> Area: " << db_object.area 
         << ", Roundness: " << db_object.roundness << ", Orientation: " << db_object.orientation << "\n";
    cout << "Area Similar: " << similar_area << ", Roundness Similar: " << similar_roundness 
         << ", Orientation Similar: " << similar_orientation << "\n\n";

    return similar_area && similar_roundness && similar_orientation;
}

// Function to read object descriptions from the text file
void ReadObjectDescriptions(const string &filename, vector<ObjectDescription> &database) {
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Error: Cannot open object description file." << endl;
        exit(1);
    }

    ObjectDescription obj;
    while (input_file >> obj.label >> obj.center_row >> obj.center_column >> obj.Emin 
           >> obj.area >> obj.roundness >> obj.orientation) {
        database.push_back(obj);
    }
    input_file.close();
}

// Function to extract object properties from a labeled image (such as area, centroid, roundness, etc.)
ObjectDescription ExtractObjectProperties(int label, const Image &labeled_image) {
    ObjectDescription object;
    object.label = label;

    int total_pixels = 0;
    double sum_row = 0.0, sum_column = 0.0;
    double second_moment_row = 0.0, second_moment_column = 0.0;

    // Iterate over the image to extract properties
    for (int row = 0; row < labeled_image.num_rows(); ++row) {
        for (int col = 0; col < labeled_image.num_columns(); ++col) {
            if (labeled_image.GetPixel(row, col) == label) {
                total_pixels++;
                sum_row += row;
                sum_column += col;

                // Sum of second moments for roundness/orientation calculation
                second_moment_row += row * row;
                second_moment_column += col * col;
            }
        }
    }

    // Print debugging information to see if pixels are being counted correctly
    cout << "Extracting properties for label: " << label << "\n";
    cout << "Total pixels: " << total_pixels << "\n";

    // Calculate area (total number of pixels)
    object.area = total_pixels;

    // Calculate the centroid (center of mass)
    if (total_pixels > 0) {
        object.center_row = sum_row / total_pixels;
        object.center_column = sum_column / total_pixels;
    } else {
        object.center_row = object.center_column = 0;
    }

    // Roundness calculation based on second moments
    if (total_pixels > 0) {
        object.roundness = (second_moment_row + second_moment_column) / (total_pixels * total_pixels);
    } else {
        object.roundness = 0;
    }

    // Placeholder for orientation (could be improved based on actual data)
    object.orientation = atan2(second_moment_row, second_moment_column) * 180.0 / M_PI;

    // Print debugging information for extracted properties
    cout << "Extracted properties: Area = " << object.area << ", Centroid = (" 
         << object.center_row << ", " << object.center_column << "), Roundness = " 
         << object.roundness << ", Orientation = " << object.orientation << "\n\n";

    return object;
}

// Function to find the maximum label in the labeled image
int FindMaxLabel(const Image &labeled_image) {
    int max_label = 0;
    for (int row = 0; row < labeled_image.num_rows(); ++row) {
        for (int col = 0; col < labeled_image.num_columns(); ++col) {
            int label = labeled_image.GetPixel(row, col);
            if (label > max_label) {
                max_label = label;
            }
        }
    }
    return max_label;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " {input labeled image} {input object descriptions} {output image}" << endl;
        return 0;
    }

    const string input_image_file(argv[1]);
    const string input_description_file(argv[2]);
    const string output_image_file(argv[3]);

    // Read the labeled image
    Image labeled_image;
    if (!ReadImage(input_image_file, &labeled_image)) {
        cout << "Error: Cannot read image " << input_image_file << endl;
        return 0;
    }

    // Load the object descriptions into a vector
    vector<ObjectDescription> object_database;
    ReadObjectDescriptions(input_description_file, object_database);

    // Create the output image
    Image output_image = labeled_image;

    // Find the maximum label in the labeled image
    int max_label = FindMaxLabel(labeled_image);

    // Iterate over each unique label in the labeled image
    for (int label = 1; label <= max_label; ++label) {
        // Extract properties of this object from the labeled image
        ObjectDescription img_object = ExtractObjectProperties(label, labeled_image);

        // Compare the extracted object with the known objects in the database
        for (const ObjectDescription &db_object : object_database) {
            if (CompareObjects(img_object, db_object)) {
                // Recognized: Draw the dot and line on the output image
                DrawLine(img_object.center_column, img_object.center_row,
                         img_object.center_column + 10 * cos(img_object.orientation * M_PI / 180.0),
                         img_object.center_row + 10 * sin(img_object.orientation * M_PI / 180.0),
                         128, &output_image);  // Drawing orientation line
            }
        }
    }

    // Write the output image with recognized objects
    if (!WriteImage(output_image_file, output_image)) {
        cout << "Error: Cannot write image to " << output_image_file << endl;
        return 0;
    }

    return 0;
}



