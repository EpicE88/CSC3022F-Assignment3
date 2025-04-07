/**
 * Main method
 * 
 * Author: Ethan Ngwetjana
 * Student number: NGWETH001
 * Date: 03/04/2025
 */
#include <iostream>
#include "ConnectedComponent.h"
#include "PGMimageProcessor.h"
#include <climits>

int main(int argc, char * argv[]){

// Default command-line arguments
int minComponentSize = 1;  
int minSize = 1;  
int maxSize = INT_MAX;  
int threshold = 128;   
bool print = false;
bool filterFlag = false;        
std::string inFileName;
std::string outFileName;

// Handle command-line arguments
for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-m" && i + 1 < argc) {
        minComponentSize = std::stoi(argv[++i]);
    }
    else if (arg == "-f" && i + 2 < argc) {
        minSize = std::stoi(argv[++i]);
        maxSize = std::stoi(argv[++i]);
        filterFlag = true;
    }
    else if (arg == "-t" && i + 1 < argc) {
        threshold = std::stoi(argv[++i]);
    }
    else if (arg == "-p") {
        print = true;
    }
    else if (arg == "-w" && i + 1 < argc) {
        outFileName = argv[++i];
    }
    else {
        if (arg[0] != '-') {
            inFileName = arg;
        }
    }
}

// Validate required arguments
if (inFileName.empty()) {
    std::cerr << "Error: No input file specified." << std::endl;
    return 1;
}

if (!outFileName.empty() && outFileName[0] == '-') {
    std::cerr << "Error: Invalid output filename." << std::endl;
    return 1;
}

    PGMimageProcessor<unsigned char> imageProcessor;

    imageProcessor.read(inFileName);
    imageProcessor.extractComponents(threshold, minComponentSize);
    if (filterFlag)
        imageProcessor.filterComponentsBySize(minSize, maxSize);

    imageProcessor.writeComponents(outFileName);
    
    if (print){
        imageProcessor.printComponentData();
    }

    
}