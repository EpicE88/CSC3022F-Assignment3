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

int main(char argc, char * argv[]){

    //Default command-line arguments
    int minComponentSize = 1;
    int minSize = 0;
    int maxSize = 0;
    int threshold = 128;
    bool print = false;
    std::string inFileName;
    std::string outFileName;

    //Handle command-line arguments
    for (int i = 0; i < argc; ++i){
        std::string arg = argv[i];

        if (arg == "-m" && i + 1 < argc)
            minComponentSize = std::stoi(argv[++i]);

        if (arg == "-f" && i + 2 < argc){
            minSize == std::stoi(argv[++i]);
            maxSize == std::stoi(argv[++i]);
        }
        if (arg == "-t" && i + 1 < argc)
            threshold = std::stoi(argv[++i]);

        if (arg == "-p")
            print = true;

        if (arg == "-w" && i + 1 < argc)
            outFileName = argv[++i];
        
        else
            inFileName = argv[i];

        //Handle incorrect input
        if (minSize == 0 || maxSize == 0 || inFileName.empty() || outFileName.empty()){
            std::cerr << "Incorrect input." << std::endl;
            std::cerr << "Format: ./bin/findcomp -m <min_valid_comp_size> -f <min_comp_size> <max_comp_size> -t <threshold> -p -w <out_file_name> <in_file_name>" << std::endl;
            return 1;
        }
        
    }


    PGMimageProcessor imageProcessor;

    imageProcessor.read(inFileName);
    imageProcessor.extractComponents(threshold, minComponentSize);
    imageProcessor.filterComponentsBySize(minSize, maxSize);
    imageProcessor.writeComponents(outFileName);
    
    if (print){
        imageProcessor.printComponentData();
    }

    
}