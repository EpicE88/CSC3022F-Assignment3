/**
 * ConnectedComponent Header File
 * 
 * Author: Ethan Ngwetjana
 * Student Number: NGWETH001
 * Date: 31/03/2025
 * 
 * 
 */

#include <vector>

#ifndef _CC_H
#define _CC_H

class ConnectedComponent{
private:
    int numPixels;
    int id;
    std::vector<std::pair<int, int>> pixels;

public:
    //Big 6

    /**
     * Default Constructor
     */
    ConnectedComponent();

    /**
     * Destructor
     */
    ~ConnectedComponent();

    /**
     * Copy Constructor
     */
    ConnectedComponent(const ConnectedComponent & other);

    /**
     * Copy Assignment Operator
     */
    ConnectedComponent & operator=(const ConnectedComponent & other);

    /**
     * Move Constructor
     */
    ConnectedComponent(ConnectedComponent && rhs);

    /**
     * Move Assignment Operator
     */
    ConnectedComponent & operator=(ConnectedComponent && rhs);


    /**
     * Get method. Returns numPixels
     */
    int getNumPixels() const;

    /**
     * Get Method. returns ID
     */
    int getID()const;

    /**
     * Get Method. Returns the Pixel Coordinates in the form of a pair
     */
    std::vector<std::pair<int,int>> getPixels() const;

    
    /**
     * Set method. Sets the ID
     */
    void setID(int id);


};

#endif