#include <iostream>  
#include <fstream>  
#include <vector>    
#include "image.h"   
#include "DisjSets.h" 

using namespace std;  
using namespace ComputerVisionProjects;  

// labeling connected components in a binary image.
// input binary image file (input_file), and the output labeled image file (output_file).
// It first reads the input image, applies connected component labeling using disjoint sets,
// normalizes the labels for grayscale representation, and finally writes the labeled image to the output file.
void p2(const std::string &input_file, const std::string &output_file) {
    
    // Declaring an Image object to hold the binary image
    Image binary_image;

    //  read an image from the input file.
    if (!ReadImage(input_file, &binary_image)) {
        cerr << "Could not read input image!" << endl;
        return;
    }

    // Retrieve the number of rows and columns in the binary image to use in the labeling process
    const int rows = binary_image.num_rows();
    const int cols = binary_image.num_columns();

    // Declaring another Image object to store the labeled image.
    // Allocate space for the labeled image and ensure it is of the same size as the binary image.
    Image labeled_image;
    labeled_image.AllocateSpaceAndSetSize(rows, cols);
    
    // Setting the number of gray levels in the image, ensuring it is set to 255 for grayscale visualization
    labeled_image.SetNumberGrayLevels(255);

    // Initialize disjoint sets, where each pixel starts as its own set (used to track connected components).
    DisjSets label_sets(rows * cols);
    
    // Variable to keep track of the next label to assign during the first pass of the image
    int next_label = 1;

    // First pass: connected component labeling.
    // Assign labels to white pixels and track equivalent labels for merging sets.
   std::cout << "First pass: Assign labels" << endl;

    // Loop through each pixel in the binary image
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // If the pixel is not black, it is considered foreground
            if (binary_image.GetPixel(i, j) != 0) {
                // Check the neighboring pixels (left and top) to see if they are already labeled
                int left_label = (j > 0) ? labeled_image.GetPixel(i, j - 1) : 0;
                int top_label = (i > 0) ? labeled_image.GetPixel(i - 1, j) : 0;

                // If neither neighbor has a label, assign a new label to the current pixel
                if (left_label == 0 && top_label == 0) {
                    labeled_image.SetPixel(i, j, next_label);  // Assign new label
                    //std::cout << "Assigning new label " << next_label << " at (" << i << ", " << j << ")" << endl;
                    ++next_label;  // Increment label for the next unique region
                }
                // If only the left neighbor is labeled, assign that label to the current pixel
                else if (left_label != 0 && top_label == 0) {
                    labeled_image.SetPixel(i, j, left_label);
                }
                // If only the top neighbor is labeled, assign that label to the current pixel
                else if (left_label == 0 && top_label != 0) {
                    labeled_image.SetPixel(i, j, top_label);
                }
                // If both neighbors are labeled, assign the smaller label and merge the sets
                else if (left_label != 0 && top_label != 0) {
                    // Ensure the labels are valid before merging
                    //if (left_label < rows * cols && top_label < rows * cols) {
                        int min_label = min(left_label, top_label);  // Choose the smaller label
                        labeled_image.SetPixel(i, j, min_label);
                        const int left_find = label_sets.find(left_label);
                        const int top_find = label_sets.find(top_label);
                    
                        if (left_find != top_find) label_sets.unionSets(left_find, top_find);  // Merge the two sets (labels are equivalent)
                        //cout << "Merging labels " << left_label << " and " << top_label << " at (" << i << ", " << j << ")" << endl;
                    //} else {
                    //    cerr << "Warning: Invalid label for union at (" << i << ", " << j << ")" << endl;
                    //}
                }
            }
        }
    }

    // Second pass: label equivalences by assigning each pixel the canonical label of its set.
  // std:: cout << "Second pass: Resolving equivalences" << endl;
    
    // std::cout << rows * cols << endl;
    // for (int i = 1; i < rows * cols; ++i) {
    //     std::cout << i << endl;
    //     std::cout << label_sets.find(i);
    // }

    // Loop through each pixel again to replace labels with their canonical representative
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            //cout << i << " " << j << endl;
            int label = labeled_image.GetPixel(i, j);
            //cout << "L: " << label << endl;
            if (label > 0) {
              //  cout << "M: " << i << " " << j << endl;
                int canonical_label = label_sets.find(label); 
                labeled_image.SetPixel(i, j, 20+canonical_label); 
                // Ensure the label is valid before attempting to resolve it
                // if (label < rows * cols) {
                //     int canonical_label = label_sets.find(label);  // Find the root of the disjoint set
                //     labeled_image.SetPixel(i, j, canonical_label);  // Assign the canonical label
                //     cout << "Label at (" << i << ", " << j << ") resolved to " << canonical_label << endl;
                // } else {
                //     cerr << "Warning: Invalid label access at (" << i << ", " << j << ")" << endl;
                // }
            }
        }
    }


   // reason for adding the function: The output pgm was pitch black
    // Normalize the labels to a grayscale range (1-255) to make them visually distinct
    int max_label = next_label - 1;  // Maximum label value used
    if (max_label > 255) max_label = 255;  // Ensure it doesn't exceed the grayscale range

    // Adjust the label values so that they are scaled proportionally within the grayscale range
    std::cout << "Normalizing labels to grayscale" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int label = labeled_image.GetPixel(i, j);
            if (label > 0) {
                // Map the label to a grayscale value by scaling it proportionally to the maximum label
                int grayscale_value = (label * 255) / max_label;
                labeled_image.SetPixel(i, j, grayscale_value);
            }
        }
    }

    // Write the labeled image to the output file
    if (!WriteImage(output_file, labeled_image)) {
        cerr << "Could not write output image!" << endl;
        return;
    }

    // debug: labeled image has been successfully saved
   std:: cout << "Labeled image saved to " << output_file << endl;
}


// checks if the correct number of arguments is provided 
// If valid,process the image.
int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " {input binary image} {output labeled image}" << endl;
        return 1;  
    }
    p2(argv[1], argv[2]);
    return 0;  
}
