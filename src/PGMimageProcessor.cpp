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

/**
 * Method that extracts all the connected components, based on the supplied threshold and excluding any components
 * less than minValidSize
 * @param threshold: user-defined threshold
 * @param minValidSize: specified minimum valid size of connected components
 * 
 */
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize){


    //Convert grayscale image pixels to 255 or 0
    for (int i = 0; i < (width * height); i++){
        if (buffer[i] >= threshold){
            buffer[i] = 255;
        }
        else{
            buffer[i] = 0;
        }
    }

    //Only do BFS on pixels that are 255
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if (buffer[y * width + x] == 255){
                ConnectedComponent component;

                //bfs(x, y, component);


                // Only store components that are less than greater than minValid size
                if (component.getNumPixels() >= minValidSize){

                    //Assign a permanent unique ID to the component
                    component.setID(nextComponentID++);
                    components.push_back(std::make_unique<ConnectedComponent>(component));
                }
            }
        }
    }

    return components.size();

}

/**
 * Method that iterates through container of components and filters all the components which do not obey the size 
 * criteria
 * @param minSize: minimum size criteria
 * @param maxSize: maximum size criteria
 */
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){
    
    //Iterated with an iterator through all stored components
    for(vector<std::unique_ptr<ConnectedComponent>>::const_iterator it = components.begin(); it != components.end(); ++it){
        int pixels = (*it)->getNumPixels();
        
        //Remove any component that is not in the size criteria
        if (pixels < minSize || pixels > maxSize){
            components.erase(it);
        }

    }

    return components.size();
}

/**
 * Method that creates a new PGM file which contains all available components
 * @param outFileName: name of the output PGM file
 */
bool PGMimageProcessor::writeComponents(const std::string & outFileName){

    //No components to write
    if (components.empty()){
        return false;
    }

    //Initialise output image buffer filled with 0s
    vector<unsigned char> outputBuffer(width * height, 0);


    //Set pixels of components to 255
    for (const unique_ptr<ConnectedComponent> & component: components){
        for (const pair<int,int> & pixelCoord: component->getPixels()){
            int x = pixelCoord.first;
            int y = pixelCoord.second;
            outputBuffer[y * width + x] = 255;
        }
    }

    //Write output buffer to file
    ofstream ofs(outFileName, ios::binary);

    if (!ofs){
        cerr << "Unable to open PGM output file " << outFileName << endl;
        return false;
    }

    ofs << "P5" << width << " " << height << endl << 255 << endl;
    ofs.write(reinterpret_cast<const char*>(outputBuffer.data()), width * height);

    if (!ofs){
        cerr << "Error writing binary block of PGM.\n";

        return false;
    }

    ofs.close();

    return true;

}

/**
 * Method that return s the number of components
 */
int PGMimageProcessor::getComponentCount(void) const{
    return components.size();
}

/**
 * Method that returns the number of pixels in largest component
 */
int PGMimageProcessor::getLargestSize(void) const{

    int maxPixelNum = -999;

    for (const unique_ptr<ConnectedComponent> & component: components){
        if (component->getNumPixels() >= maxPixelNum){
            maxPixelNum = component->getNumPixels();
        }
    }

    return maxPixelNum;
}






