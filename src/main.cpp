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

/**
 * Helper method to get to see if input file is ppm/pgm
 * @param filename: name of the input file
 */
std::string getFileExtension(const std::string & filename){
    int dotPosition = filename.find_last_of(".");
    if (dotPosition == std::string::npos)
        //no extension
        return "";

    std::string extension = filename.substr(dotPosition + 1);

    //Convert to lower case
    for (char& c: extension){
        c = tolower(c);
    }
    return extension;

}

int main(int argc, char * argv[]){

    // Default command-line arguments
    int minComponentSize = 1;  
    int minSize = 1;  
    int maxSize = INT_MAX;  
    int threshold = 128;   
    bool print = false;
    bool filterFlag = false;        
    bool boxFlag = false;
    bool writeFlag = false;
    std::string inFileName;
    std::string outFileName;
    std::string boxFileName;

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
            if (boxFlag){
                std::cerr << "Cannot use both -w and -b flags together" << std::endl;
                return 1;
            }
            writeFlag = true;
            outFileName = argv[++i];
        }
        else if (arg == "-b") {
            if (writeFlag){
                std::cerr << "Cannot use both -w and -b flags together" << std::endl;
                return 1;
            }

            boxFlag = true;
            boxFileName = argv[++i];
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

    if (writeFlag && outFileName[0] == '-') {
        std::cerr << "Error: Invalid output filename." << std::endl;
        return 1;
    }

    if (boxFlag && outFileName[0] == '-') {
        std::cerr << "Error: Invalid output filename." << std::endl;
        return 1;
    }

        
    std::string extension = getFileExtension(inFileName);


    if (extension == "ppm"){
        PGMimageProcessor<std::array<unsigned char, 3>> imageProcessor;

        imageProcessor.read(inFileName);
        imageProcessor.extractComponents(threshold, minComponentSize);
        if (filterFlag)
            imageProcessor.filterComponentsBySize(minSize, maxSize);

        if (writeFlag)
            imageProcessor.writeComponents(outFileName);

        if (boxFlag)
            imageProcessor.drawBoundingBoxes(boxFileName);
        
        if (print){
            imageProcessor.printComponentData();
        }

    }
    else if (extension == "pgm"){
        PGMimageProcessor<unsigned char> imageProcessor;

        imageProcessor.read(inFileName);
        imageProcessor.extractComponents(threshold, minComponentSize);
        if (filterFlag)
            imageProcessor.filterComponentsBySize(minSize, maxSize);

        if (writeFlag)
        imageProcessor.writeComponents(outFileName);

        if (boxFlag)
            imageProcessor.drawBoundingBoxes(boxFileName);
        
        
        if (print){
            imageProcessor.printComponentData();
        }
    }
    else {
        std::cerr << "Unsupported file format. Use .pgm or ppm" << std::endl;
        return 1;
    }
        
    return 0;
}