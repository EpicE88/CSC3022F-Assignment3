# 🖼️ CSC3022F - Assignment 3: Image Processing with Connected Components

## 👨‍🎓 Student Information
- **Name**: Ethan Ngwetjana  
- **Student Number**: NGWETH001  
- ***Email***: ngwth001@myuct.ac.za
- **Date**: April 2025  

## 📝 Program Description
This C++ program performs connected component analysis on PGM/PPM images, allowing for component extraction, filtering, and visualization with bounding boxes.

## ✨ Features
- 📖 Reads both PGM (grayscale) and PPM (color) images
- 🔍 Extracts connected components using 4-connectivity
- 🎚️ Filters components by size range
- 💾 Outputs components to new image files
- 🟥 Draws bounding boxes around components
- 📊 Provides component data

## ‼️ Requirements

- **C++ Compiler**:
    - This program requires a compatible compiler (g++).
- **PGM/PPM Image Support**:
    - This program works with PGM (Portable Grey Map) and PPM (Portable Pixel Map) images. Ensure you have images for input (some images have been provided)
    - If you want to use your own PGM/PPM images make sure to convert them from PNG to PPM/PGM correctly, otherwise the program will not compile
- **Makefile**: 
    - A suitable Makefile to compile the code is provided.

## 🛠️ Building the program
1. Ensure you have `make` and `g++` installed
2. Navigate to the project directory
3. Run:  
   ```bash
   make clean
   make

This will compile the executable findcomp

## 🚀 How to use the program
Run:
    ```bash
    ./bin/findcomp [options] <inputPGMfile>

⚙️***Options:***
- -m <int>: Set minimum size for valid components (default: 1)
- -f <min> <max>: Filter components by size range
- -t <int>: Set threshold for component detection (0-255, default: 128)
- -p: Print component data
- -w <filename>: Write components to output file (PGM/PPM)
- -b <filename>: Draw bounding boxes and output to PPM file

⚠️***Important Notes:***
- You must use either -w or -b, but not both
- Output filenames must not start with '-'
- Input image cannot be left empty

🚀***Examples:***
1. Basic component extraction:
    ```bash
    ./bin/findcomp -t 150 -w output.pgm input.pgm

2. Extract and filter components
    ```bash
    ./bin/findcomp -m 10 -f 50 500 -t 180 -w output.pgm input.pgm

3. Draw bounding boxes
    ```bash
    ./bin/findcomp -t 170 -b boxes.ppm input.pgm

5. Print component information:
    ```bash
    ./bin/findcomp -t 170 -p -w output.pgm input.pgm

6. Component extraction on PPM files
    ```bash
    ./bin/findcomp -t 170 -w output.ppm input.ppm

## Output

The program will output a PPM/PGM image. You will need to use a tool like pnmtopng to convert it to PNG format

## 📂 File Structure
```
CSC3022F-Assignment3/
├──📂 src
    ├── 💻 main.cpp     # main driver program
    ├── 📄 PGMimageProcessor.h      # Header file
    ├── 📄 PGMimageProcessor.cpp    # Core image processing class
    ├── 📄 ConnectedComponent.h     # Header file
    ├── 📄 ConnectedComponent.cpp   # Component storage class
    ├── 📄 catch.hpp                # Testing framework
    ├── 📄 UnitTests.cpp            # Conducts unit tests
├── 📜 Makefile     # Build configuration
├── 📖 README.md    # This file
├── 🖼️ Shapes.png   # Test images
├── 📄 Shapes.pgm
    .
    .
    .
    (other image files)
```

## 🧪 Testing

Run unit tests with:
```bash
make clean
make test
```

## 📦 Dependencies
- C++17 compatible
- Standard Library only (no external dependencies)



