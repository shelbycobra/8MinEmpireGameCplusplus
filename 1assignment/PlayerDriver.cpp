#include "Player.h"
#include "MapLoader.h"
#include "MapUtil.h"
#include <cassert>

void test_PlayerOwnsValidSetOfRegions();
void test_PlayerOwnsValidHand();
void test_PlayerHasValidBidding();
void test_PayCoins();
void test_PlaceNewArmies();
void test_MoveArmies();
void test_MoveOverLand();
void test_BuildCity();
void test_DestroyArmy();

int main() {
    test_PlayerOwnsValidSetOfRegions();
    // test_PlayerOwnsValidHand();
    // test_PlayerHasValidBidding();
    test_PayCoins();
    test_PlaceNewArmies();
    test_MoveArmies();
    test_MoveOverLand();
    test_BuildCity();
    test_DestroyArmy();
}

void test_PlayerOwnsValidSetOfRegions() {
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlayerOwnsValidSetOfRegions" << endl;
    cout << "---------------------------------------" << endl;

    string name = "player1";
    Player* player = new Player(name, 9);

    GameMap* map = generateValidMap();
    Vertices* vertices = map->getVertices();
    Vertex* england = new Vertex("England", "Europe");

    cout << "\nTEST: Player occupies a country that doesn't exist on the map.\n" << endl;

    player->addCountry(england);

    bool result = playerOccupiedCountriesAreFoundOnMap(player->getCountries(), vertices);
    assert( result == false );

    cout << "\nTEST: Player occupies two countries that exist on the map.\n" << endl;

    player->removeCountry(england);
    player->addCountry(map->getVertices()->find("Z")->second);
    player->addCountry(map->getVertices()->find("X")->second);

    result = playerOccupiedCountriesAreFoundOnMap(player->getCountries(), vertices);
    assert( result == true );
}

void test_PlayerOwnsValidHand(){}
void test_PlayerHasValidBidding(){}

void test_PayCoins(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PayCoins" << endl;
    cout << "---------------------------------------" << endl;

    cout << "\nTEST: Player pays coins successfully.\n" << endl;

    string name = "player1";
    Player* player = new Player(name, 9);
    //9 - 5
    bool result = player->payCoins(5);
    assert (result == true);

    //4 - 4
    result = player->payCoins(4);
    assert (result == true);

    //0 - 0
    result = player->payCoins(0);
    assert (result == true);

    cout << "\nTEST: Player pays coins unsuccessfully.\n" << endl;

    //0 - -1
    result = player->payCoins(-1);
    assert (result == false);

    //5 - 6
    string name2 = "player2";
    Player* player2 = new Player(name2, 5);
    result = player2->payCoins(6);
    assert (result == false);

    player2->payCoins(5);

    //0 - 7
    result = player2->payCoins(7);
    assert (result == false);
}

void test_PlaceNewArmies(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_PlaceNewArmies" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    string startName = "Z";
    map->setStartVertex(startName);

    string name1 = "Player 1";
    Player* player1 = new Player(name1, 9);

    cout << "\nTEST: Player places new armies on start.\n" << endl;

    int startArmies;
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    player1->getArmiesOnCountry(startVertex);
    bool result = player1->placeNewArmies(3, startVertex, startName);
    startArmies = player1->getArmiesOnCountry(startVertex);

    assert (result == true && startArmies == 3);

    cout << "\nTEST: Player places new armies on invalid country \"X\".\n" << endl;

    string xName = "X";
    int xArmies;
    Vertex* xVertex = map->getVertices()->find(xName)->second;

    player1->getArmiesOnCountry(xVertex);
    result = player1->placeNewArmies(2, xVertex, startName);
    xArmies = player1->getArmiesOnCountry(xVertex);

    assert ( result == false && xArmies == 0 );

    cout << "\nTEST: Player places new armies on adjacent country \"X\".\n" << endl;

    player1->getArmiesOnCountry(xVertex);
    result = player1->moveOverLand(1, startVertex, xVertex);
    result = result && player1->buildCity(xVertex);
    result = result && player1->placeNewArmies(2, xVertex, startName);
    xArmies = player1->getArmiesOnCountry(xVertex);

    assert ( result == true && xArmies == 3 );
}

void test_MoveArmies(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_MoveArmies" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    string startName = "W";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->placeNewArmies(3, startVertex, startName);

    cout << "\nTEST: Player moves armies successfully from START to \"X\".\n" << endl;

    bool result;
    int xArmies;
    int startArmies;
    string xName = "X";

    Vertex* xVertex = map->getVertices()->find(xName)->second;
    result = player->moveArmies(2, startVertex, xVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    startArmies = player->getArmiesOnCountry(startVertex);

    assert( result == true && xArmies == 2 && startArmies == 1 );

    cout << "\nTEST: Player moves armies over water successfully from \"X\" to \"C\".\n" << endl;

    int cArmies;
    string cName = "C";
    Vertex* cVertex = map->getVertices()->find(cName)->second;

    result = player->moveArmies(1, xVertex, cVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    cArmies = player->getArmiesOnCountry(cVertex);

    assert( result == true && cArmies == 1 && xArmies == 1 );

    cout << "\nTEST: Player moves armies unsuccessfully from \"X\" to \"B\".\n" << endl;

    int bArmies;
    string bName = "B";
    Vertex* bVertex = map->getVertices()->find(bName)->second;

    result = player->moveArmies(1, xVertex, bVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == false && xArmies == 1 && bArmies == 0 );

    cout << "\nTEST: Player moves armies unsuccessfully from START to \"B\".\n" << endl;

    result = player->moveArmies(1, startVertex, bVertex, true);
    startArmies = player->getArmiesOnCountry(startVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == false && startArmies == 1 && bArmies == 0 );

}

void test_MoveOverLand(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_MoveArmies" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    string startName = "A";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->placeNewArmies(3, startVertex, startName);

    cout << "\nTEST: Player moves armies successfully from START to \"B\".\n" << endl;

    bool result;
    int startArmies;
    int bArmies;
    string bName = "B";
    Vertex* bVertex = map->getVertices()->find(bName)->second;

    result = player->moveOverLand(1, startVertex, bVertex);
    startArmies = player->getArmiesOnCountry(startVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == true && startArmies == 2 && bArmies == 1 );

    cout << "\nTEST: Player moves armies successfully from START to \"C\".\n" << endl;

    int cArmies;
    string cName = "C";
    Vertex* cVertex = map->getVertices()->find(cName)->second;

    result = player->moveOverLand(1, startVertex, cVertex);
    startArmies = player->getArmiesOnCountry(startVertex);
    cArmies = player->getArmiesOnCountry(cVertex);

    assert( result == true && startArmies == 1 && cArmies == 1 );

    cout << "\nTEST: Player moves armies unsuccessfully over water from \"C\" to \"X\".\n" << endl;

    int xArmies;
    string xName = "X";
    Vertex* xVertex = map->getVertices()->find(xName)->second;

    result = player->moveOverLand(1, cVertex, xVertex);
    cArmies = player->getArmiesOnCountry(cVertex);
    xArmies = player->getArmiesOnCountry(xVertex);

    assert( result == false && cArmies == 1 && xArmies == 0 );

}
void test_BuildCity(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_BuildCity" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->placeNewArmies(3, startVertex, startName);

    cout << "\nTEST: Player builds a city on START.\n" << endl;

    int startCities;
    int result;

    result = player->buildCity(startVertex);
    startCities = player->getCitiesOnCountry(startVertex);

    assert( result == true && startCities == 1 );

    cout << "\nTEST: Player builds two cities on valid country \"P\".\n" << endl;

    int pCities;
    string pName = "P";
    Vertex* pVertex = map->getVertices()->find(pName)->second;

    result = player->moveArmies(1, startVertex, pVertex, true);
    result = result && player->buildCity(pVertex);
    result = result && player->buildCity(pVertex);
    pCities = player->getCitiesOnCountry(pVertex);

    assert( result == true && pCities == 2 );

    cout << "\nTEST: Player fails to build a city on invalid country \"R\".\n" << endl;

    int rCities;
    string rName = "R";
    Vertex* rVertex = map->getVertices()->find(rName)->second;

    result = player->buildCity(rVertex);
    rCities = player->getCitiesOnCountry(rVertex);

    assert( result == false && rCities == 0 );
}
void test_DestroyArmy(){
    cout << "\n---------------------------------------" << endl;
    cout << "TEST: test_DestroyArmy" << endl;
    cout << "---------------------------------------" << endl;

    GameMap* map = generateValidMap();
    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name1 = "Player 1", name2 = "Player 2", name3 = "Player 3";

    Player* player1 = new Player(name1, 11);
    Player* player2 = new Player(name2, 11);
    Player* player3 = new Player(name3, 11);

    // Start out with 3 armies on the START country
    player1->placeNewArmies(3, startVertex, startName);
    player2->placeNewArmies(3, startVertex, startName);
    player3->placeNewArmies(3, startVertex, startName);

    cout << "\nTEST: Player 1 destroys an army on START from Player 2.\n" << endl;

    bool result;
    int startArmies;

    player2->getArmiesOnCountry(startVertex);
    result = player1->destroyArmies(startVertex, player2);
    startArmies = player2->getArmiesOnCountry(startVertex);

    assert( result == true && startArmies == 2 );

    cout << "\nTEST: Player 1 fails destroys own army on START.\n" << endl;

    player1->getArmiesOnCountry(startVertex);
    result = player1->destroyArmies(startVertex, player1);
    startArmies = player1->getArmiesOnCountry(startVertex);

    assert( result == false && startArmies == 3 );

    cout << "\nTEST: Player 1 destroys an army on \"P\" from Player 3.\n" << endl;

    int pArmies;
    string pName = "P";
    Vertex* pVertex = map->getVertices()->find(pName)->second;

    player3->getArmiesOnCountry(pVertex);
    result = player3->moveOverLand(1, startVertex, pVertex);
    result = result && player1->destroyArmies(pVertex, player3);
    pArmies = player3->getArmiesOnCountry(pVertex);

    assert( result == true && pArmies == 0 );

    cout << "\nTEST: Player 1 fails to destroy an army on \"P\" from Player 2.\n" << endl;

    player2->getArmiesOnCountry(pVertex);
    result = player1->destroyArmies(pVertex, player2);
    pArmies = player2->getArmiesOnCountry(pVertex);

    assert( result == false && pArmies == 0);

    cout << "\nTEST: Player 1 fails to destroy own army on \"P\".\n" << endl;

    result = player1->moveOverLand(1, startVertex, pVertex);
    player1->getArmiesOnCountry(pVertex);
    result = result && player1->destroyArmies(pVertex, player1);
    pArmies = player1->getArmiesOnCountry(pVertex);

    assert( result == false && pArmies == 1 );
}