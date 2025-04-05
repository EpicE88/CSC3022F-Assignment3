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

//Big 6

/**
 * Default constructor
 */
PGMimageProcessor::PGMimageProcessor(): inputBuffer(), width(0), height(0) , nextComponentID(0), components(){}

/**
 * Destructor
 */
PGMimageProcessor::~PGMimageProcessor(){}

/**
 * Copy Constructor
 */
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor & other): inputBuffer(other.inputBuffer), width(other.width), height(other.height), 
    nextComponentID(other.nextComponentID){

        //Can't make a copy of a unique ptr. 
        //Create a new uniqe_ptr that owns a copy of the ConnectedComponent
        for (const std::unique_ptr<ConnectedComponent> & comp: other.components){
            components.push_back(std::make_unique<ConnectedComponent>(*comp));
        }
    }

/**
 * Copy Assignment Operator
 */
PGMimageProcessor & PGMimageProcessor::operator=(const PGMimageProcessor & other){

    //Check for self assignment
    if (this != &other){
        inputBuffer = other.inputBuffer;
        width = other.width;
        height = other.height;
        nextComponentID = other.nextComponentID;

        //Can't make a copy of a unique ptr. 
        //Create a new uniqe_ptr that owns a copy of the ConnectedComponent
        for (const std::unique_ptr<ConnectedComponent> & comp: other.components){
            components.push_back(std::make_unique<ConnectedComponent>(*comp));
        }
    }

    return *this;
}

/**
 * Move Constructor
 */
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor && rhs): inputBuffer(std::move(rhs.inputBuffer)), 
    width(rhs.width), height(rhs.height), components(std::move(rhs.components)), nextComponentID(rhs.nextComponentID){}

/**
 * Move Assignment Operator
 */
PGMimageProcessor & PGMimageProcessor::operator=(PGMimageProcessor && rhs){

    //Check for self assignment
    if (this != &rhs){
        inputBuffer = std::move(rhs.inputBuffer);
        width = rhs.width;
        height = rhs.height;
        components = std::move(rhs.components);
        nextComponentID = rhs.nextComponentID;
    }

    return *this;
}

/**
 * PGM image reading functionality
 * Author: Patrick Marias, University of Cape Town (CSC3022F)
 * Reused from Assignment 2 resources
 * Modified: buffer to a vector.
 */


/**
 * Method that sets image data from existing sized inputBuffer
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


    inputBuffer.assign(data, data + (wd * ht));
    width = wd; height = ht;
    for (size_t i = 0; i < wd * ht; ++i) inputBuffer[i] = data[i];
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

    inputBuffer.resize(width * height);
    ifs.read(reinterpret_cast<char*>(inputBuffer.data()), width * height);

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
        if (inputBuffer[i] >= threshold){
            inputBuffer[i] = 255;
        }
        else{
            inputBuffer[i] = 0;
        }
    }

    //Only do BFS on pixels that are 255
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if (inputBuffer[y * width + x] == 255){
                ConnectedComponent component;

                bfs(x, y, component);

                // Only store components that are less than greater than minValid size
                if (component.getNumPixels() >= minValidSize){
                    //Assign a permanent unique ID to the component
                    component.setID(nextComponentID++);
                    components.push_back(std::make_unique<ConnectedComponent>(component));
                }
            }
        }
    }

    //Delete original image from memory after processing
    inputBuffer.clear();
    inputBuffer.shrink_to_fit();

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

    //Initialise output image inputBuffer filled with 0s
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

/**
 * Method that returns the number of pixels in smallest component
 */
int PGMimageProcessor::getSmallestSize(void) const{

    int minPixelNum = 999;

    for (const unique_ptr<ConnectedComponent> & component: components){
        if (component->getNumPixels() >= minPixelNum){
            minPixelNum = component->getNumPixels();
        }
    }

    return minPixelNum;
}

/**
 * Method that prints the data for a component to std::cout 
 * @param component: the component that will be printed
 */
void PGMimageProcessor::printComponentData(const ConnectedComponent & component) const{
    cout << "Component ID: " << component.getID() << ", Number of pixels: " << component.getNumPixels() << endl;
} 

/**
 * Method that prints component data for all components in a format
 */
void PGMimageProcessor::printComponentData() const{

    if (components.empty()){
        cout << "There are no components to display" << endl;
        return;
    }

    //Heading
    cout << "Component Data:" << endl;
    //Line
    cout << "------------------------" << endl;

    for (const std::unique_ptr<ConnectedComponent> & component: components){
       printComponentData(*component); 
    }

    //Line
    cout << "------------------------" << endl;

    cout << "Total components: " << components.size() << endl;
    cout << "Smallest component size: " << getSmallestSize() << endl;
    cout << "Largest component size: " << getLargestSize() << endl; 


}

/**
 * Method that uses Breadth-First Search to find all connected foreground pixels of a component
 * @param x: x-coordinate of pixel
 * @param x: y-coordinate of pixel
 * @param component: the connected component 
 */
void PGMimageProcessor::bfs(int x, int y, ConnectedComponent & component){
    
    //Initialise the queue
    std::queue<std::pair<int,int>> q;

    //Push starting pixel
    q.push({x, y});
    
    //Mark starting pixel as visited
    inputBuffer[y * width + x] = 0; 

    //Process pixels until there are non in the queue
    while(!q.empty()){
        std::pair<int, int> current = q.front();

        int xcoord = current.first;
        int ycoord = current.second;
        q.pop(); //Remove from the queue

        //Add pixel to component
        component.addPixel(xcoord, ycoord);

        //Check the N,S,E,W neighbours
        int nOffsets[4][2] = {{0,1}, {1,0}, {0, -1}, {-1, 0}};

        for (int i = 0; i < 4; i++){
            int xNeighbour = xcoord + nOffsets[i][0];
            int yNeighbour = ycoord + nOffsets[i][1];

            //Check if neighbour is withn boundaries
            if ((xNeighbour >= 0 && xNeighbour < width) && (yNeighbour >= 0 && yNeighbour < height)){
                //Check if neighbour is unvisited
                if (inputBuffer[yNeighbour * width + xNeighbour] == 255){
                    inputBuffer[yNeighbour * width + xNeighbour] = 0; // mark as visited
                    q.push(std::make_pair(xNeighbour, yNeighbour));
                }
            }
        }

        
    }

}

/**
 * Get Method. Returns the inputBuffer
 */
const std::vector<unsigned char> & PGMimageProcessor::getInputBuffer() const{
    return inputBuffer;
}

/**
 * Get Method. Returns width
 */
int PGMimageProcessor::getWidth() const{
    return width;
}

/**
 * Get Method. Returns height
 */
int PGMimageProcessor::getHeight() const{
    return height;
}

/**
 * Get Method. Returns a const referencvcve to components vector
 */
const std::vector<std::unique_ptr<ConnectedComponent>>& PGMimageProcessor::getComponents() const {
    return components;
}

/**
 * Get Method. returns nextComponentID
 */
int PGMimageProcessor::getNextComponentID() const{
    return nextComponentID;
}




