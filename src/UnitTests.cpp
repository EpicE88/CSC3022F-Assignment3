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


TEST_CASE("PGMimageProcessor - Default Constructor"){
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



TEST_CASE("PGMimageProcessor - Copy Constructor"){
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

TEST_CASE("PGMimageProcessor - Move Constructor"){
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

    // Store original values for verification
    const int originalID = original.getNextComponentID();
    const int originalCount = original.getComponentCount();
    const int originalWidth = original.getWidth();
    const int originalHeight = original.getHeight();
    const std::vector<unsigned char> originalBuffer = original.getInputBuffer();

    const ConnectedComponent* firstComponentPtr = original.getComponents()[0].get();

    PGMimageProcessor moved(std::move(original));

    SECTION("Check if contents moved"){
        REQUIRE(originalBuffer == moved.getInputBuffer());
        REQUIRE(originalWidth == moved.getWidth());
        REQUIRE(originalHeight == moved.getHeight());
        REQUIRE(originalCount == moved.getComponentCount());
        REQUIRE(originalID == moved.getNextComponentID());

        //Check if components were moved
        REQUIRE(moved.getComponents()[0].get() == firstComponentPtr);
    }

    SECTION("Check if original is empty"){
        REQUIRE(original.getInputBuffer().empty());
        REQUIRE(original.getWidth() == 0);
        REQUIRE(original.getHeight() == 0);
        REQUIRE(original.getComponents().empty());
        REQUIRE(original.getNextComponentID() == 0);
    }

}

TEST_CASE("PGMimageProcessor - Copy Assignment Operator"){
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


TEST_CASE("PGMimageProcessor - Move Assignment Operator"){
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

    // Store original values for verification
    const int originalID = original.getNextComponentID();
    const int originalCount = original.getComponentCount();
    const int originalWidth = original.getWidth();
    const int originalHeight = original.getHeight();
    const std::vector<unsigned char> originalBuffer = original.getInputBuffer();

    const ConnectedComponent* firstComponentPtr = original.getComponents()[0].get();

    PGMimageProcessor moved(std::move(original));

    SECTION("Check if contents moved"){
        REQUIRE(originalBuffer == moved.getInputBuffer());
        REQUIRE(originalWidth == moved.getWidth());
        REQUIRE(originalHeight == moved.getHeight());
        REQUIRE(originalCount == moved.getComponentCount());
        REQUIRE(originalID == moved.getNextComponentID());

        //Check if components were moved
        REQUIRE(moved.getComponents()[0].get() == firstComponentPtr);
    }

    SECTION("Check if original is empty"){
        REQUIRE(original.getInputBuffer().empty());
        REQUIRE(original.getWidth() == 0);
        REQUIRE(original.getHeight() == 0);
        REQUIRE(original.getComponents().empty());
        REQUIRE(original.getNextComponentID() == 0);
    }
}

/*
TEST_CASE("PGMimageProcessor - extractComponents method"){
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

TEST_CASE("PGMimageProcessor - filterComponentBySize method"){
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

TEST_CASE("PGMimageProcessor - writeComponents method"){
    PGMimageProcessor p;
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0,
        0, 0, 255
    };
    p.setImageData(data, 3, 3);
    p.extractComponents(128, 1);
    
    const std::string testFile = "test_output.pgm";

    SECTION("Write components to output file"){
        bool wrote = p.writeComponents(testFile);
        REQUIRE(wrote);

        //chekc if file exists
        std::ifstream ifs(testFile);
        REQUIRE(ifs.good());
        ifs.close();
        
        std::remove(testFile.c_str());
    }

    SECTION("Write with no components"){
        PGMimageProcessor emptyP;
        bool wrote = emptyP.writeComponents(testFile);
        REQUIRE_FALSE(wrote);
    }
}

TEST_CASE("PGMimageProcessor - bfs method"){
    PGMimageProcessor p;
    unsigned char data[] = {
        255, 255, 0,
        255, 255, 0,
        0, 0, 255
    };
    p.setImageData(data, 3, 3);

    ConnectedComponent component;

    SECTION("4x4 block in top-left"){
        p.bfs(0, 0, component);
        REQUIRE(component.getNumPixels() == 4);
    }

    SECTION("Single pixel at the bottom"){
        p.bfs(2, 2, component);
        REQUIRE(component.getNumPixels() == 1);
    }
    
}

TEST_CASE("PGMimageProcessor - Get methods"){
    PGMimageProcessor p;
    unsigned char data[] = {
        255, 0, 
        0, 255
    };
    p.setImageData(data, 2, 2);
    p.extractComponents(128, 1);

    SECTION("separated pixels"){
        unsigned char data[] = {
            255, 0, 
            0, 255
        };
        p.setImageData(data, 2, 2);
        p.extractComponents(128, 1);

        REQUIRE(p.getComponentCount() == 2);

        REQUIRE(p.getLargestSize() == 1);
        REQUIRE(p.getSmallestSize() == 1);

    }

    SECTION("one large component"){
        unsigned char data[] = {
            255, 255, 
            255, 255
        };
        p.setImageData(data, 2, 2);
        p.extractComponents(128, 1);

        REQUIRE(p.getComponentCount() == 1);

        REQUIRE(p.getLargestSize() == 4);
        REQUIRE(p.getSmallestSize() == 4);
    }

    SECTION("two components of different sizes"){
        unsigned char data[] = {
            255, 0, 255,
            255, 255, 0
        };
        p.setImageData(data, 3, 3);
        p.extractComponents(128, 1);

        REQUIRE(p.getComponentCount() == 2);

        REQUIRE(p.getLargestSize() == 3);
        REQUIRE(p.getSmallestSize() == 1);
    }

}

TEST_CASE("ConnectedComponent - Default Constructor"){
    ConnectedComponent c;

    REQUIRE(c.getNumPixels() == 0);
    REQUIRE(c.getID() == 0);
    REQUIRE(c.getPixels().empty());
}

TEST_CASE("Connected Component - Copy Constructor"){
    ConnectedComponent original;
    original.setID(1);
    original.addPixel(10, 20);

    ConnectedComponent copy(original);

    REQUIRE(copy.getID() == original.getID());
    REQUIRE(copy.getNumPixels() == original.getNumPixels());
    REQUIRE(copy.getPixels() == original.getPixels());
}

TEST_CASE("Connected Component - Move Constructor"){
    ConnectedComponent original;
    original.setID(3);
    original.addPixel(2, 2);

    const std::pair<int, int> * pixelsPtr = original.getPixels().data();

    ConnectedComponent moved(std::move(original));

    REQUIRE(moved.getID() == 3);
    REQUIRE(moved.getNumPixels() == 1);
    REQUIRE(moved.getPixels().size() == 1);
    REQUIRE(moved.getPixels().data() == pixelsPtr);

    //Check if original is empty
    REQUIRE(original.getPixels().empty());
    REQUIRE(original.getNumPixels() == 0);
}

TEST_CASE("ConnectedComponent - Copy Assignment"){
    ConnectedComponent original;
    original.setID(1);
    original.addPixel(10, 20);

    ConnectedComponent copy;
    copy = original;

    REQUIRE(copy.getID() == original.getID());
    REQUIRE(copy.getNumPixels() == original.getNumPixels());
    REQUIRE(copy.getPixels() == original.getPixels());
}

TEST_CASE("ConnectedComponent - Move Assignment Operator"){
    ConnectedComponent original;
    original.setID(3);
    original.addPixel(2, 2);

    ConnectedComponent moved;
    moved = std::move(original);

    REQUIRE(moved.getID() == 3);
    REQUIRE(moved.getNumPixels() == 1);
    REQUIRE(moved.getPixels().size() == 1);
    REQUIRE(moved.getPixels()[0] == std::make_pair(2, 2));

    //Check if original is empty
    REQUIRE(original.getPixels().empty());
    REQUIRE(original.getNumPixels() == 0);
}
*/








