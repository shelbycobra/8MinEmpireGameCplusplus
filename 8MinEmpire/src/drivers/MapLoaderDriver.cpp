#include "../MapLoader.h"
#include "../util/MapUtil.h"
#include "../util/TestUtil.h"
#include <cassert>

void test_ReadValidMapFilesAndGenerateValidMaps();
void test_ReadInvalidMapFiles();
void test_CreateInvalidGraphFromMap();

int main() {
    test_ReadValidMapFilesAndGenerateValidMaps();
    test_ReadInvalidMapFiles();
    test_CreateInvalidGraphFromMap();

    return 0;
}

void test_ReadValidMapFilesAndGenerateValidMaps() {

    cout << "\n\n\n======================================================================" << endl;
    cout << "TEST: test_ReadValidMapFilesAndGenerateValidMaps" << endl;
    cout << "======================================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - large map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    MapLoader loader("largeValid.map");
    loader.generateMap();

    assert(1 == isConnectedMap());
    cout << "Large map is connected!" << endl;

    assert(1 == validateContinents());
    cout << "Large map has valid continents!" << endl;

    assert(1 == validateEdges());
    cout << "Large map has valid edges!" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - med map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete GameMap::instance();
    loader.setMapFilePath("medValid.map");
    loader.generateMap();

    assert(1 == isConnectedMap());
    cout << "Medium map is connected!" << endl;

    assert(1 == validateContinents());
    cout << "Medium map has valid continents!" << endl;

    assert(1 == validateEdges());
    cout << "Medium map has valid edges!" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a valid map file - small map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete GameMap::instance();
    loader.setMapFilePath("smallValid.map");
    loader.generateMap();

    assert(1 == isConnectedMap());
    cout << "Small map is connected!" << endl;

    assert(1 == validateContinents());
    cout << "Small map has valid continents!" << endl;

    assert(1 == validateEdges());
    cout << "Small map has valid edges!" << endl;

    delete GameMap::instance();
    
}

void test_ReadInvalidMapFiles(){
    cout << "\n\n\n========================================================" << endl;
    cout << "TEST: test_ReadInvalidMapFiles" << endl;
    cout << "======================================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a garbage file." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    MapLoader loader("garbage.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing an image." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("missingImage.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing edges section." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("missingEdges.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file with invalid edge syntax." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("invalidEdgeSyntax.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file missing top level continent name." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("missingTopContinent.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file with invalid region name syntax." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("invalidRegionSyntax.map");
    loader.generateMap();

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a non-existant map file." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    loader.setMapFilePath("fileDoesntExist.map");
    loader.generateMap();
}

void test_CreateInvalidGraphFromMap(){
    cout << "\n\n\n========================================================" << endl;
    cout << "TEST: test_CreateInvalidGraphFromMap" << endl;
    cout << "======================================================================\n\n" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a disconnected map." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    MapLoader loader("disconnected.map");
    loader.generateMap();

    assert(0 == isConnectedMap());
    cout << "Map is disconnected!" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with an internal water edge." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete GameMap::instance();
    loader.setMapFilePath("internalWaterEdge.map");
    loader.generateMap();

    assert(1 == isConnectedMap());
    cout << "Map is connected!" << endl;

    assert(0 == validateContinents());
    cout << "Map has invalid continents!" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with invalid edges." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete GameMap::instance();
    loader.setMapFilePath("invalidEdges.map");
    loader.generateMap();

    assert(0 == validateEdges());
    cout << "Map has invalid edges!" << endl;

    cout << "\n--------------------------------------------------------------------" << endl;
    cout << "TEST: Read a map file and create a map with invalid continents." << endl;
    cout << "--------------------------------------------------------------------\n" << endl;

    delete GameMap::instance();
    loader.setMapFilePath("invalidContinent.map");
    loader.generateMap();

    assert(1 == isConnectedMap());
    cout << "Map is connected!" << endl;

    assert(0 == validateContinents());
    cout << "Map has invalid continents!" << endl;

    delete GameMap::instance();
    
}