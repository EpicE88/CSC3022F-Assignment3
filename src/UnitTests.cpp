/**
 * UnitTests class
 * Does unit testing for required methods of assingment
 * 
 * Author: Ethan Ngwetjana
 * Student Number: NGWETH001
 * Date: 05/04/2025
 */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ConnectedComponent.h"
#include "PGMimageProcessor.h"


TEST_CASE("Default Constructor"){
    PGMimageProcessor p;
    std::vector<unsigned char> inputBuffer = p.getInputBuffer();
    int width = p.getWidth();
    int height = p.getHeight();
    int numComponents = p.getComponentCount();
    int nextComponentID = p.getNextComponentID();

    REQUIRE(inputBuffer.empty());
    REQUIRE(width == 0);
    REQUIRE(height == 0);
    REQUIRE(numComponents == 0);
    REQUIRE(nextComponentID == 0);

}



TEST_CASE("Copy Constructor"){
    PGMimageProcessor original;
    
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0, 
        0, 0, 255
    };
    original.setImageData(data, 3, 3);

    //Populate components vector and increment nextComponentID
    int numComponents = original.extractComponents(128, 1);
    REQUIRE(numComponents > 0);

    //Ensure original has components before copying
    REQUIRE(original.getComponentCount() > 0);
    REQUIRE(original.getNextComponentID() > 0);

    PGMimageProcessor copy(original);

    REQUIRE(original.getInputBuffer() == copy.getInputBuffer());
    REQUIRE(original.getWidth() == copy.getWidth());
    REQUIRE(original.getHeight() == copy.getHeight());
    REQUIRE(original.getComponentCount() == copy.getComponentCount());
    REQUIRE(original.getNextComponentID() == copy.getNextComponentID());

    //Ensure components were deeply copied
    const std::vector<std::unique_ptr<ConnectedComponent>> & originalComponents = original.getComponents();
    const std::vector<std::unique_ptr<ConnectedComponent>> & copiedComponents = copy.getComponents();

    //check each component
    for (int i = 0; i < originalComponents.size(); ++i){
        REQUIRE(originalComponents[i]->getID() == copiedComponents[i]->getID());
        REQUIRE(originalComponents[i]->getNumPixels() == copiedComponents[i]->getNumPixels());
    }

    //check independency

    //remove all components in orginal
    original.filterComponentsBySize(100, 100);
    REQUIRE(original.getComponentCount() == 0);

    //check if copy is unchanged
    REQUIRE(copy.getComponentCount() == numComponents);


}

TEST_CASE("Move Constructor"){
    PGMimageProcessor original;
    
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0, 
        0, 0, 255
    };
    original.setImageData(data, 3, 3);

    //Populate components vector and increment nextComponentID
    int numComponents = original.extractComponents(128, 1);
    REQUIRE(numComponents > 0);

    PGMimageProcessor moved(std::move(original));

    SECTION("Check if contents moved"){
        REQUIRE(original.getInputBuffer() == moved.getInputBuffer());
        REQUIRE(original.getWidth() == moved.getWidth());
        REQUIRE(original.getHeight() == moved.getHeight());
        REQUIRE(original.getComponentCount() == moved.getComponentCount());
        REQUIRE(original.getNextComponentID() == moved.getNextComponentID());

        const std::vector<std::unique_ptr<ConnectedComponent>> & originalComponents = original.getComponents();
        const std::vector<std::unique_ptr<ConnectedComponent>> & movedComponents = moved.getComponents();

        //check each component
        for (int i = 0; i < originalComponents.size(); ++i){
            REQUIRE(originalComponents[i]->getID() == movedComponents[i]->getID());
            REQUIRE(originalComponents[i]->getNumPixels() == movedComponents[i]->getNumPixels());
        }

    }

    SECTION("Check if original is empty"){
        REQUIRE(original.getInputBuffer().empty());
        REQUIRE(original.getWidth() == 0);
        REQUIRE(original.getHeight() == 0);
        REQUIRE(original.getComponents().empty());
        REQUIRE(original.getNextComponentID() == 0);
    }

}

TEST_CASE("Copy Assignment Operator"){
    PGMimageProcessor original;
    
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0, 
        0, 0, 255
    };
    original.setImageData(data, 3, 3);

    //Populate components vector and increment nextComponentID
    int numComponents = original.extractComponents(128, 1);
    REQUIRE(numComponents > 0);

    //Ensure original has components before copying
    REQUIRE(original.getComponentCount() > 0);
    REQUIRE(original.getNextComponentID() > 0);

    PGMimageProcessor copy;
    copy = original;

    REQUIRE(original.getInputBuffer() == copy.getInputBuffer());
    REQUIRE(original.getWidth() == copy.getWidth());
    REQUIRE(original.getHeight() == copy.getHeight());
    REQUIRE(original.getComponentCount() == copy.getComponentCount());
    REQUIRE(original.getNextComponentID() == copy.getNextComponentID());

    //Ensure components were deeply copied
    const std::vector<std::unique_ptr<ConnectedComponent>> & originalComponents = original.getComponents();
    const std::vector<std::unique_ptr<ConnectedComponent>> & copiedComponents = copy.getComponents();

    //check each component
    for (int i = 0; i < originalComponents.size(); ++i){
        REQUIRE(originalComponents[i]->getID() == copiedComponents[i]->getID());
        REQUIRE(originalComponents[i]->getNumPixels() == copiedComponents[i]->getNumPixels());
    }

    //check independency

    //remove all components in orginal
    original.filterComponentsBySize(100, 100);
    REQUIRE(original.getComponentCount() == 0);

    //check if copy is unchanged
    REQUIRE(copy.getComponentCount() == numComponents);
}

TEST_CASE("Move Assignment Operator"){
    PGMimageProcessor original;
    
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0, 
        0, 0, 255
    };
    original.setImageData(data, 3, 3);

    //Populate components vector and increment nextComponentID
    int numComponents = original.extractComponents(128, 1);
    REQUIRE(numComponents > 0);

    PGMimageProcessor moved(std::move(original));

    SECTION("Check if contents moved"){
        REQUIRE(original.getInputBuffer() == moved.getInputBuffer());
        REQUIRE(original.getWidth() == moved.getWidth());
        REQUIRE(original.getHeight() == moved.getHeight());
        REQUIRE(original.getComponentCount() == moved.getComponentCount());
        REQUIRE(original.getNextComponentID() == moved.getNextComponentID());

        const std::vector<std::unique_ptr<ConnectedComponent>> & originalComponents = original.getComponents();
        const std::vector<std::unique_ptr<ConnectedComponent>> & movedComponents = moved.getComponents();

        //check each component
        for (int i = 0; i < originalComponents.size(); ++i){
            REQUIRE(originalComponents[i]->getID() == movedComponents[i]->getID());
            REQUIRE(originalComponents[i]->getNumPixels() == movedComponents[i]->getNumPixels());
        }

    }

    SECTION("Check if original is empty"){
        REQUIRE(original.getInputBuffer().empty());
        REQUIRE(original.getWidth() == 0);
        REQUIRE(original.getHeight() == 0);
        REQUIRE(original.getComponents().empty());
        REQUIRE(original.getNextComponentID() == 0);
    }
}

TEST_CASE("extractComponents method"){
    PGMimageProcessor p;
    unsigned char data[] = {
        128, 0, 128, 0, 255,
        0, 255, 0, 128, 0,
        255, 0, 255, 0, 255,
        0, 128, 0, 255, 0,
        128, 0, 255, 0, 128
    };
    p.setImageData(data, 5, 5);

    SECTION("Threshold 128, min size 1"){
        int componentCount = p.extractComponents(128, 1);
        REQUIRE(componentCount == 13);
        REQUIRE(p.getComponentCount() == 13);
    }

    SECTION("Threshold 255, min size 1"){
        int componentCount = p.extractComponents(255, 1);
        REQUIRE(componentCount < 13);
        REQUIRE(p.getComponentCount() < 13);
    }

    SECTION("Threshold 500, min size 1"){
        int componentCount = p.extractComponents(500, 2);
        REQUIRE(componentCount == 0);
        REQUIRE(p.getComponentCount() == 0);
    }

    SECTION("Threshold 128, min size 2"){
        int componentCount = p.extractComponents(128, 2);
        REQUIRE(componentCount == 0);
        REQUIRE(p.getComponentCount() == 0);
    }

}

TEST_CASE("filterComponentBySize method"){
    PGMimageProcessor p;
    unsigned char data[] = {
        255, 255, 0, 0, 0,
        255, 255, 0, 0, 0,
        0, 0, 255, 0, 0,
        0, 0, 0, 255, 255,
        0, 0, 0, 255, 255
    };
    p.setImageData(data, 5, 5);
    p.extractComponents(128, 1);

    SECTION("Filter components between 1 and 5"){
        int componentCount = p.filterComponentsBySize(1, 5);
        REQUIRE(componentCount == 2); 
    }

    SECTION("Filter components between 5 and 5"){
        int componentCount = p.filterComponentsBySize(5, 5);
        REQUIRE(componentCount == 1); 
    }

    SECTION("Filter components between 1 and 4"){
        int componentCount = p.filterComponentsBySize(1, 4);
        REQUIRE(componentCount == 1); 
    }
}









