# **Binary Image Object Recognition System**

## **Description**
This project implements a system to process binary images and recognize specific two-dimensional objects. The program identifies and labels connected regions in an image, calculates their attributes, and performs object recognition based on their properties.

The system consists of four primary programs:
1. **Binary Conversion**: Converts grayscale images to binary using a threshold value.
2. **Region Labeling**: Segments binary images into connected regions with unique labels.
3. **Attribute Calculation**: Computes object attributes, including position, orientation, and shape properties.
4. **Object Recognition**: Recognizes and highlights objects based on their attributes.

---

## **Files**
- **Source Code**:
  - `p1.cc`: Binary conversion.
  - `p2.cc`: Region labeling.
  - `p3.cc`: Attribute calculation.
  - `p4.cc`: Object recognition.
  - `image.h`, `image.cc`: Image processing utilities.
  - `DisjSets.h`, `DisjSets.cc`: Disjoint sets data structure implementation for efficient region labeling.

- **Supporting Files**:
  - `Makefile`: Builds all executables (`p1`, `p2`, `p3`, `p4`).
  - `README.md`: This file.

---

## **Compilation and Execution**

### **Compilation**
Run the following command to compile all programs:
```bash
make all
```

### **Execution**
#### **Program 1: Binary Conversion**
```bash
./p1 {input_gray_level_image} {threshold_value} {output_binary_image}
```
- **Input**: Grayscale image and threshold value.
- **Output**: Binary image file.

#### **Program 2: Region Labeling**
```bash
./p2 {input_binary_image} {output_labeled_image}
```
- **Input**: Binary image.
- **Output**: Image with labeled regions.

#### **Program 3: Attribute Calculation**
```bash
./p3 {input_labeled_image} {output_object_descriptions} {output_image}
```
- **Input**: Labeled image.
- **Output**: Text file with object attributes and visual output showing positions and orientations.

#### **Program 4: Object Recognition**
```bash
./p4 {input_labeled_image} {input_object_descriptions} {output_image}
```
- **Input**: Labeled image and object descriptions.
- **Output**: Image with recognized objects highlighted.

---

## **Assumptions and Details**
1. Input images are in the PGM format.
2. Binary images are created using a uniform threshold for simplicity.
3. Region labeling employs a sequential algorithm with two passes to resolve label equivalences.
4. Object attributes include position, orientation, area, and shape properties.
5. Object recognition uses a comparison of attributes against a reference database.

---

## **Usage Notes**
- Ensure the programs run correctly in the specified environment.
- Visualize input/output images using tools like [GIMP](https://www.gimp.org).
- Use consistent file naming conventions for seamless execution.

---

For further details, refer to the source code and in-line comments. Happy coding!
