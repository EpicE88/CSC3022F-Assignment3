/**
 * ConnectedComponent class
 * A helper class that represents a connected component.
 * 
 * Author: Ethan Ngwetjana
 * Student number: NGWETH001
 * Date: 02/04/2025
 * 
 * 
 */
#include "ConnectedComponent.h"


//Big 6

/**
 * Default Constructor
 */
ConnectedComponent::ConnectedComponent() : numPixels(0), id(0), pixels() {}

/**
 * Destructor
 */
ConnectedComponent::~ConnectedComponent(){}

/**
 * Copy Constructor
 */
ConnectedComponent::ConnectedComponent(const ConnectedComponent & other): numPixels(other.numPixels),
    id(other.id), pixels(other.pixels){}


/**
 * Copy Assignment Operator
 */
ConnectedComponent & ConnectedComponent::operator=(const ConnectedComponent & other){

    // Check for self assignment
    if (this != & other){
        numPixels = other.numPixels;
        id = other.id;
        pixels = other.pixels;
    }

    return *this;
}

/**
 * Move Constructor
 */
ConnectedComponent::ConnectedComponent(ConnectedComponent && rhs): numPixels(rhs.numPixels),
    id(rhs.id), pixels(std::move(rhs.pixels)){rhs.updateNumPixels();}

/**
 * Move Assignment Operator
 */
ConnectedComponent & ConnectedComponent::operator=(ConnectedComponent && rhs){

    //Check for self assignment
    if (this != &rhs){
        numPixels = rhs.numPixels;
        id = rhs.id;
        pixels = std::move(rhs.pixels);
        
        rhs.numPixels = 0;
        rhs.id = 0;
    }

    return *this;
}

/**
 * Get method. Returns numPixels
 */
int ConnectedComponent::getNumPixels() const{
    return numPixels;
}

/**
 * Get Method. returns ID
 */
int ConnectedComponent::getID() const{
    return id;
}

/**
 * Get Method. Returns the Pixel Coordinates in the form of a pair
 */
std::vector<std::pair<int,int>> ConnectedComponent::getPixels() const{
    return pixels;
}

/**
 * Set method. Sets the ID
 */
void ConnectedComponent::setID(int ID){
    id = ID;
}

/**
 * Method that adds a pixels to the pixels vector
 * @param x: x coordinate of pixel being added
 * @param y: y coordinate of pixel being added
 */
void ConnectedComponent::addPixel(int x, int y){
    pixels.push_back(std::make_pair(x, y));
    updateNumPixels();
}

/**
 * Method that updates numPixels when a pixel is added or removed
 */
void ConnectedComponent::updateNumPixels(){
    numPixels = pixels.size();
}
