/**
 * PGMimageProcessor Header File.
 * 
 * Author: Ethan Ngwetjana
 * Student Number: NGWETH001
 * 
 * Date: 31/03/2025
 * 
 *
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "ConnectedComponent.h"

#ifndef _PGMIP_H
#define _PGMIP_H

class PGMimageProcessor{
private:

    std::vector<unsigned char> inputBuffer;
    int width, height;
    std::vector<std::unique_ptr<ConnectedComponent>> components;
    int nextComponentID;


public: 

    //Big 6

    /**
     * Default constructor
     */
    PGMimageProcessor();

    /**
     * Destructor
     */
    ~PGMimageProcessor();

    /**
     * Copy Constructor
     */
    PGMimageProcessor(const PGMimageProcessor & other);

    /**
     * Copy Assignment Operator
     */
    PGMimageProcessor & operator=(const PGMimageProcessor & other);

    /**
     * Move Constructor
     */
    PGMimageProcessor(PGMimageProcessor && rhs);

    /**
     * Move Assignment Operator
     */
    PGMimageProcessor & operator=(PGMimageProcessor && rhs);



    /**
     * PGM image reading functionality
     * Author: Patrick Marias, University of Cape Town (CSC3022F)
     * Reused from Assignment 2 resources
     */


    /**
     * Method that reads PGM image
     * @param fileName: Name of the PGM image being read
     */
    void read(const std::string & fileName);


    /**
     * Method that sets image data from existing sized inputBuffer
     * @param data: image data
     * @param wd: width
     * @param ht: height
     */
    void setImageData(unsigned char* data, int wd, int ht);

    // Core methods

    /**
     * Method that extracts all the connected components, based on the supplied threshold and excluding any components
     * less than minValidSize
     * @param threshold: user-defined threshold
     * @param minValidSize: specified minimum valid size of connected components
     * 
     */
    int extractComponents(unsigned char threshold, int minValidSize);


    /**
     * Method that iterates through container of components and filters all the components which do not obey the size 
     * criteria
     * @param minSize: minimum size criteria
     * @param maxSize: maximum size criteria
     */
    int filterComponentsBySize(int minSize, int maxSize);

    /**
     * Method that creates a new PGM file which contains all available components
     * @param outFileName: name of the output PGM file
     */
    bool writeComponents(const std::string & outFileName);

    /**
     * Method that return s the number of components
     */
    int getComponentCount(void) const;

    /**
     * Method that returns the number of pixels in largest component
     */
    int getLargestSize(void) const;

    /**
     * Method that returns the number of pixels in smallest component
     *
     */
    int getSmallestSize(void) const;

    /**
     * Method that prints the data for a component to std::cout 
     * @param component: the component that will be printed
     */

    void printComponentData(const ConnectedComponent & component) const; 
    
    /**
     * Method that prints component data for all components in a format
     */
    void printComponentData() const;

    /**
     * Method that uses Breadth-First Search to find all connected foreground pixels of a component
     * @param x: x-coordinate of pixel
     * @param y: y-coordinate of pixel 
     * @param component: the connected component
     */
    void bfs(int x, int y, ConnectedComponent & component);

};

#endif