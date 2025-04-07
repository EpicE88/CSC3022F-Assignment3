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
template <typename T>
PGMimageProcessor<T>::PGMimageProcessor(): inputBuffer(), width(0), height(0) , nextComponentID(0), components(){}

/**
 * Destructor
 */
template <typename T>
PGMimageProcessor<T>::~PGMimageProcessor(){}

/**
 * Copy Constructor
 */
template <typename T>
PGMimageProcessor<T>::PGMimageProcessor(const PGMimageProcessor & other): inputBuffer(other.inputBuffer), width(other.width), height(other.height), 
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
template <typename T>
PGMimageProcessor<T> & PGMimageProcessor<T>::operator=(const PGMimageProcessor & other){

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
template <typename T>
PGMimageProcessor<T>::PGMimageProcessor(PGMimageProcessor && rhs): inputBuffer(std::move(rhs.inputBuffer)), 
    width(rhs.width), height(rhs.height), components(std::move(rhs.components)), 
    nextComponentID(rhs.nextComponentID){
        rhs.width = 0;
        rhs.height = 0;
        rhs.nextComponentID = 0;
    }

/**
 * Move Assignment Operator
 */
template <typename T>
PGMimageProcessor<T> & PGMimageProcessor<T>::operator=(PGMimageProcessor && rhs){

    //Check for self assignment
    if (this != &rhs){
        inputBuffer = std::move(rhs.inputBuffer);
        width = rhs.width;
        height = rhs.height;
        components = std::move(rhs.components);
        nextComponentID = rhs.nextComponentID;

        rhs.width = 0;
        rhs.height = 0;
        rhs.nextComponentID = 0;
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
template <typename T>
void PGMimageProcessor<T>::setImageData(T* data, int wd, int ht){
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
 * Method that reads PGM image (Specialised for PGM)
 * @param fileName: Name of the PGM image being read
 */
template<>
void PGMimageProcessor<unsigned char>::read(const string& fileName){

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
 * Method that reads PGM image (Specialised for PPM images)
 * @param fileName: Name of the PGM image being read
 */
template<>
void PGMimageProcessor<std::array<unsigned char, 3>>::read(const string& fileName){

    ifstream ifs(fileName, ios::binary);
    if (!ifs)
    {
        cerr << "Failed top open file for read: " << fileName << endl;
        return;
    }
    string line;
    ifs >> line >> ws;
    if (line != "P6")
    {
        cerr << "Malformed PPM file - magic is: " << line << endl;
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
    ifs.read(reinterpret_cast<char*>(inputBuffer.data()), width * height * 3);

    if (!ifs)
    {
        cerr << "Failed to read binary block - read\n";
    }

    ifs.close();
}

/**
 * Method that extracts all the connected components, based on the supplied threshold and excluding any components
 * less than minValidSize (Specialised for PGM images)
 * @param threshold: user-defined threshold
 * @param minValidSize: specified minimum valid size of connected components
 * 
 */
template <typename T>
int PGMimageProcessor<T>::extractComponents(unsigned char threshold, int minValidSize){

    std::vector<unsigned char> grey(width * height);

    grey.resize(width * height);
    if constexpr (std::is_same_v<T, std::array<unsigned char, 3>>){
        
        for (int i = 0; i < width * height; i++){
            grey[i] = 0.299f * inputBuffer[i][0] + 0.587f * inputBuffer[i][1] + 0.114f * inputBuffer[i][2];
        }
        
    }
    else{
        std::copy(inputBuffer.begin(), inputBuffer.end(), grey.begin());
    }

    //Convert grayscale image pixels to 255 or 0
    for (int i = 0; i < (width * height); i++){
        if (grey[i] >= threshold){
            grey[i] = 255;
        }
        else{
            grey[i] = 0;
        }
    }

    //Only do BFS on pixels that are 255
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if (grey[y * width + x] == 255){
                ConnectedComponent component;

                bfs(x, y, component, grey);

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

    grey.clear();
    grey.shrink_to_fit();

    return components.size();

}


/**
 * Method that iterates through container of components and filters all the components which do not obey the size 
 * criteria
 * @param minSize: minimum size criteria
 * @param maxSize: maximum size criteria
 */
template <typename T>
int PGMimageProcessor<T>::filterComponentsBySize(int minSize, int maxSize){

    //Bounds checking
    if (minSize < 0 || maxSize < 0 || minSize > maxSize){
        return components.size();
    }


    //Iterated with an iterator through all stored components
    std::vector<std::unique_ptr<ConnectedComponent>>::iterator it = components.begin();
    while (it != components.end()) {
        int pixels = (*it)->getNumPixels();
        if (pixels < minSize || pixels > maxSize) {
            it = components.erase(it); 
        } else {
            ++it;
        }
    }
    return components.size();
}

/**
 * Method that creates a new PGM file which contains all available components
 * @param outFileName: name of the output PGM file (Specialised for PGM images)
 */
template<>
bool PGMimageProcessor<unsigned char>::writeComponents(const std::string & outFileName){

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

    ofs << "P5\n" << width << " " << height << endl << 255 << endl;
    ofs.write(reinterpret_cast<const char*>(outputBuffer.data()), width * height);

    if (!ofs){
        cerr << "Error writing binary block of PGM.\n";

        return false;
    }

    ofs.close();

    return true;

}

/**
 * Method that creates a new PGM file which contains all available components
 * @param outFileName: name of the output PGM file. (Specialise for PPM images)
 */
template<>
bool PGMimageProcessor<std::array<unsigned char, 3>>::writeComponents(const std::string & outFileName){

    //No components to write
    if (components.empty()){
        return false;
    }

    //Initialise output image inputBuffer filled with 0s
    std::vector<std::array<unsigned char, 3>> outputBuffer(width * height, {0, 0, 0});

    //Set pixels of components to 255
    for (const unique_ptr<ConnectedComponent> & component: components){
        for (const pair<int,int> & pixelCoord: component->getPixels()){
            int x = pixelCoord.first;
            int y = pixelCoord.second;
            outputBuffer[y * width + x] = {255, 255, 255};
        }
    }

    //Write output buffer to file
    ofstream ofs(outFileName, ios::binary);

    if (!ofs){
        cerr << "Unable to open PPM output file " << outFileName << endl;
        return false;
    }

    ofs << "P6\n" << width << " " << height << endl << 255 << endl;
    ofs.write(reinterpret_cast<const char*>(outputBuffer.data()), width * height * 3);

    if (!ofs){
        cerr << "Error writing binary block of PPM.\n";

        return false;
    }

    ofs.close();

    return true;

}

/**
 * Method that return s the number of components
 */
template <typename T>
int PGMimageProcessor<T>::getComponentCount(void) const{
    return components.size();
}

/**
 * Method that returns the number of pixels in largest component
 */
template <typename T>
int PGMimageProcessor<T>::getLargestSize(void) const{
    if(components.empty())
        return 0;

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
template <typename T>
int PGMimageProcessor<T>::getSmallestSize(void) const{
    if (components.empty())
        return 0;

    int minPixelNum = 999;

    for (const unique_ptr<ConnectedComponent> & component: components){
        if (component->getNumPixels() <= minPixelNum){
            minPixelNum = component->getNumPixels();
        }
    }

    return minPixelNum;
}

/**
 * Method that prints the data for a component to std::cout 
 * @param component: the component that will be printed
 */
template <typename T>
void PGMimageProcessor<T>::printComponentData(const ConnectedComponent & component) const{
    cout << "Component ID: " << component.getID() << ", Number of pixels: " << component.getNumPixels() << endl;
} 

/**
 * Method that prints component data for all components in a format
 */
template <typename T>
void PGMimageProcessor<T>::printComponentData() const{

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
template <typename T>
void PGMimageProcessor<T>::bfs(int x, int y, ConnectedComponent & component, std::vector<unsigned char> & grey){
    
    //Initialise the queue
    std::queue<std::pair<int,int>> q;

    //Push starting pixel
    q.push({x, y});
    
    //Mark starting pixel as visited
    grey[y * width + x] = 0; 

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
                if (grey[yNeighbour * width + xNeighbour] == 255){
                    grey[yNeighbour * width + xNeighbour] = 0; // mark as visited
                    q.push(std::make_pair(xNeighbour, yNeighbour));
                }
            }
        }

        
    }

}

/**
 * Get Method. Returns the inputBuffer
 */
template <typename T>
const std::vector<T> & PGMimageProcessor<T>::getInputBuffer() const{
    return inputBuffer;
}

/**
 * Get Method. Returns width
 */
template <typename T>
int PGMimageProcessor<T>::getWidth() const{
    return width;
}

/**
 * Get Method. Returns height
 */
template <typename T>
int PGMimageProcessor<T>::getHeight() const{
    return height;
}

/**
 * Get Method. Returns a const referencvcve to components vector
 */
template <typename T>
const std::vector<std::unique_ptr<ConnectedComponent>>& PGMimageProcessor<T>::getComponents() const {
    return components;
}

/**
 * Get Method. returns nextComponentID
 */
template <typename T>
int PGMimageProcessor<T>::getNextComponentID() const{
    return nextComponentID;
}

template class PGMimageProcessor<unsigned char>;
template class PGMimageProcessor<std::array<unsigned char, 3>>;


