/**
 * PGMimageProcessor class
 * This class reads in a PGM image and performs connected component analysis
 * 
 * Author: Ethan Ngwetjana
 * Student Number: NGWETH001
 * Date: 31/03/2025
 */

#include <iostream>
#include <string>
#include <fstream>

#include "PGMimageProcessor.h"

using namespace std;


/**
 * PGM image reading functionality
 * Author: Patrick Marias, University of Cape Town (CSC3022F)
 * Reused from Assignment 2 resources
 */


/**
 * Method that sets image data from existing sized buffer
 * @param data: image data
 * @param wd: width
 * @param ht: height
 */
void PGMimageProcessor::setImageData(unsigned char* data, int wd, int ht){
    if (data == nullptr || wd < 1 || ht < 1)
    {
        cerr << "setImageData() invalid data specified - aborted.\n";
        return;
    }
    if (buffer) delete[] buffer;
    buffer = new unsigned char[wd * ht];
    width = wd; height = ht;
    for (size_t i = 0; i < wd * ht; ++i) buffer[i] = data[i];
}


/**
 * Method that reads PGM image
 * @param fileName: Name of the PGM image being read
 */
void PGMimageProcessor::read(const string& fileName){

    ifstream ifs(fileName, ios::binary);
    if (!ifs)
    {
        cerr << "Failed top open file for read: " << fileName << endl;
        return;
    }
    string line;
    ifs >> line >> ws;
    if (line != "P5")
    {
        cerr << "Malformed PGM file - magic is: " << line << endl;
        return;
    }
    while (getline(ifs, line))
    {
        //cout << line << endl;
        if (line[0] != '#') break;
    }
    istringstream iss(line);
    iss >> width >> height;
    //cout << "width, height = (" << width << "," << height << ")\n";

    if (!iss)
    {
        cerr << "Header not correct - unexpected image sizes found: " << line << endl;
        return;
    }
    int maxChan = 0;
    ifs >> maxChan >> ws;
    if (maxChan != 255)
    {
        cerr << "Max grey level incorect - found: " << maxChan << endl;
    }
    // start of binary block

    buffer = new unsigned char[width * height];
    ifs.read(reinterpret_cast<char*>(buffer), width * height);

    if (!ifs)
    {
        cerr << "Failed to read binary block - read\n";
    }

    ifs.close();
}
