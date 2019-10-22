#include "../Player.h"
#include "../MapLoader.h"
#include "../util/TestUtil.h"
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
    test_PlayerHasValidBidding();
    test_PlayerOwnsValidSetOfRegions();
    test_PayCoins();
    test_PlaceNewArmies();
    test_MoveArmies();
    test_MoveOverLand();
    test_BuildCity();
    test_DestroyArmy();
    test_PlayerOwnsValidHand();

	return 0;
}

void test_PlayerOwnsValidSetOfRegions() {
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_PlayerOwnsValidSetOfRegions" << endl;
    cout << "=======================================================" << endl;

    string name = "player1";
    Player* player = new Player(name, 9);

    GameMap* map = generateValidMap();
    Vertex* england = new Vertex("England", "ENG", "Europe");

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player occupies a country that doesn't exist on the map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    player->addCountry(england);

    bool result = playerOccupiedCountriesAreFoundOnMap(player, map);

    assert( result == 0 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player occupies two countries that exist on the map." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    player->removeCountry(england);
    player->addCountry(map->getVertices()->find("Z")->second);
    player->addCountry(map->getVertices()->find("X")->second);

    result = playerOccupiedCountriesAreFoundOnMap(player, map);

    assert( result == 1 );

    delete player;
    delete map;
    delete england;

    player = nullptr;
    map = nullptr;
    england = nullptr;
}

void test_PlayerOwnsValidHand(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_PlayerOwnsValidHand" << endl;
    cout << "=======================================================" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player adds a card to its hand." << endl;
    cout << "--------------------------------------------------------\n" << endl;
    string name = "player 1";
    Player player(name, 9);

    Card card(1, "CARROT", "Move 4 armies");
    player.addCardToHand(&card);

    vector<Card*>* hand = player.getHand();
    cout << "Hand size is " << hand->size() << endl;

    assert(hand->size() == size_t(1));
    cout << "The player sucessfully added a card to their hand." << endl;

}

void test_PlayerHasValidBidding(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_PlayerHasValidBidding" << endl;
    cout << "=======================================================" << endl;

    string name = "player1";
    Player player(name, 9);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player is initialized with a bidder object." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    Bidder* bidder = player.getBidder();
    assert(bidder->getPlayer() == &player);
    cout << "Player's bidder object is initialized." << endl;
    cout << "Has Player made a bid yet? " << bidder->getMadeBid() << endl;
    assert(bidder->getMadeBid() == 0);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player places a bid." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    bidder->bid();
    cout << "Has Player made a bid yet? " << bidder->getMadeBid() << endl;
    assert(bidder->getMadeBid() == 1);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player fails to place a bid a second time." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    bidder->bid();
    cout << "Has Player made a bid yet? " << bidder->getMadeBid() << endl;
    assert(bidder->getMadeBid() == 1);
}

void test_PayCoins(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_PayCoins" << endl;
    cout << "=======================================================" << endl;

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player pays coins successfully." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    string name = "player1";
    Player* player = new Player(name, 9);
    //9 - 5
    bool result = player->PayCoins(5);
    assert (result == 1);

    //4 - 4
    result = player->PayCoins(4);
    assert (result == 1);

    //0 - 0
    result = player->PayCoins(0);
    assert (result == 1);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player pays coins unsuccessfully." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    //0 - -1
    result = player->PayCoins(-1);
    assert (result == 0);

    //5 - 6
    string name2 = "player2";
    Player* player2 = new Player(name2, 5);
    result = player2->PayCoins(6);
    assert (result == 0);

    player2->PayCoins(5);

    //0 - 7
    result = player2->PayCoins(7);
    assert (result == 0);

    delete player;
    delete player2;

    player = nullptr;
    player2 = nullptr;
}

void test_PlaceNewArmies(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_PlaceNewArmies" << endl;
    cout << "=======================================================" << endl;

    GameMap* map = generateValidMap();
    string startName = "Z";
    map->setStartVertex(startName);

    string name1 = "Player 1";
    Player* player1 = new Player(name1, 9);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player places new armies on start." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int startArmies;
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    player1->getArmiesOnCountry(startVertex);
    bool result = player1->PlaceNewArmies(3, startVertex, startName);
    startArmies = player1->getArmiesOnCountry(startVertex);

    assert (result == 1 && startArmies == 3);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player places new armies on invalid country \"X\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    string xName = "X";
    int xArmies;
    Vertex* xVertex = map->getVertices()->find(xName)->second;

    player1->getArmiesOnCountry(xVertex);
    result = player1->PlaceNewArmies(2, xVertex, startName);
    xArmies = player1->getArmiesOnCountry(xVertex);

    assert ( result == 0 && xArmies == 0 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player places new armies on adjacent country \"X\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    player1->getArmiesOnCountry(xVertex);
    result = player1->MoveOverLand(1, startVertex, xVertex);
    result = result && player1->BuildCity(xVertex);
    result = result && player1->PlaceNewArmies(2, xVertex, startName);
    xArmies = player1->getArmiesOnCountry(xVertex);

    assert ( result == 1 && xArmies == 3 );

    delete map;
    delete player1;

    map = nullptr;
    player1 = nullptr;
    startVertex = nullptr;
    xVertex = nullptr;
}

void test_MoveArmies(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_MoveArmies" << endl;
    cout << "=======================================================" << endl;

    GameMap* map = generateValidMap();
    string startName = "W";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->PlaceNewArmies(3, startVertex, startName);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies successfully from START to \"X\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    bool result;
    int xArmies;
    int startArmies;
    string xName = "X";

    Vertex* xVertex = map->getVertices()->find(xName)->second;
    result = player->MoveArmies(2, startVertex, xVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    startArmies = player->getArmiesOnCountry(startVertex);

    assert( result == 1 && xArmies == 2 && startArmies == 1 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies over water successfully from \"X\" to \"C\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int cArmies;
    string cName = "C";
    Vertex* cVertex = map->getVertices()->find(cName)->second;

    result = player->MoveArmies(1, xVertex, cVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    cArmies = player->getArmiesOnCountry(cVertex);

    assert( result == 1 && cArmies == 1 && xArmies == 1 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies unsuccessfully from \"X\" to \"B\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int bArmies;
    string bName = "B";
    Vertex* bVertex = map->getVertices()->find(bName)->second;

    result = player->MoveArmies(1, xVertex, bVertex, true);
    xArmies = player->getArmiesOnCountry(xVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == 0 && xArmies == 1 && bArmies == 0 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies unsuccessfully from START to \"B\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    result = player->MoveArmies(1, startVertex, bVertex, true);
    startArmies = player->getArmiesOnCountry(startVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == 0 && startArmies == 1 && bArmies == 0 );

    delete map;
    delete player;

    map = nullptr;
    startVertex = nullptr;
    player = nullptr;
    xVertex = nullptr;
    cVertex = nullptr;
    bVertex = nullptr;
}

void test_MoveOverLand(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_MoveArmies" << endl;
    cout << "=======================================================" << endl;

    GameMap* map = generateValidMap();
    string startName = "A";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->PlaceNewArmies(3, startVertex, startName);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies successfully from START to \"B\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    bool result;
    int startArmies;
    int bArmies;
    string bName = "B";
    Vertex* bVertex = map->getVertices()->find(bName)->second;

    result = player->MoveOverLand(1, startVertex, bVertex);
    startArmies = player->getArmiesOnCountry(startVertex);
    bArmies = player->getArmiesOnCountry(bVertex);

    assert( result == 1 && startArmies == 2 && bArmies == 1 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies successfully from START to \"C\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int cArmies;
    string cName = "C";
    Vertex* cVertex = map->getVertices()->find(cName)->second;

    result = player->MoveOverLand(1, startVertex, cVertex);
    startArmies = player->getArmiesOnCountry(startVertex);
    cArmies = player->getArmiesOnCountry(cVertex);

    assert( result == 1 && startArmies == 1 && cArmies == 1 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player moves armies unsuccessfully over water from \"C\" to \"X\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int xArmies;
    string xName = "X";
    Vertex* xVertex = map->getVertices()->find(xName)->second;

    result = player->MoveOverLand(1, cVertex, xVertex);
    cArmies = player->getArmiesOnCountry(cVertex);
    xArmies = player->getArmiesOnCountry(xVertex);

    assert( result == 0 && cArmies == 1 && xArmies == 0 );
    delete map;
    delete player;

    map = nullptr;
    startVertex = nullptr;
    player = nullptr;
    xVertex = nullptr;
    cVertex = nullptr;
    bVertex = nullptr;
}

void test_BuildCity(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_BuildCity" << endl;
    cout << "=======================================================" << endl;

    GameMap* map = generateValidMap();
    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name = "Player 1";
    Player* player = new Player(name, 9);

    // Start out with 3 armies on the START country
    player->PlaceNewArmies(3, startVertex, startName);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player builds a city on START." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int startCities;
    int result;

    result = player->BuildCity(startVertex);
    startCities = player->getCitiesOnCountry(startVertex);

    assert( result == 1 && startCities == 1 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player builds two cities on valid country \"P\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int pCities;
    string pName = "P";
    Vertex* pVertex = map->getVertices()->find(pName)->second;

    result = player->MoveArmies(1, startVertex, pVertex, true);
    result = result && player->BuildCity(pVertex);
    result = result && player->BuildCity(pVertex);
    pCities = player->getCitiesOnCountry(pVertex);

    assert( result == 1 && pCities == 2 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player fails to build a city on invalid country \"R\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int rCities;
    string rName = "R";
    Vertex* rVertex = map->getVertices()->find(rName)->second;

    result = player->BuildCity(rVertex);
    rCities = player->getCitiesOnCountry(rVertex);

    assert( result == 0 && rCities == 0 );

    delete map;
    delete player;

    map = nullptr;
    startVertex = nullptr;
    player = nullptr;
    pVertex = nullptr;
    rVertex = nullptr;
}

void test_DestroyArmy(){
    cout << "\n=======================================================" << endl;
    cout << "TEST: test_DestroyArmy" << endl;
    cout << "=======================================================" << endl;

    GameMap* map = generateValidMap();
    string startName = "O";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    string name1 = "Player 1", name2 = "Player 2", name3 = "Player 3";

    Player* player1 = new Player(name1, 11);
    Player* player2 = new Player(name2, 11);
    Player* player3 = new Player(name3, 11);

    // Start out with 3 armies on the START country
    player1->PlaceNewArmies(3, startVertex, startName);
    player2->PlaceNewArmies(3, startVertex, startName);
    player3->PlaceNewArmies(3, startVertex, startName);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player 1 destroys an army on START from Player 2." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    bool result;
    int startArmies;

    player2->getArmiesOnCountry(startVertex);
    result = player1->DestroyArmy(startVertex, player2);
    startArmies = player2->getArmiesOnCountry(startVertex);

    assert( result == 1 && startArmies == 2 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player 1 fails destroys own army on START." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    player1->getArmiesOnCountry(startVertex);
    result = player1->DestroyArmy(startVertex, player1);
    startArmies = player1->getArmiesOnCountry(startVertex);

    assert( result == 0 && startArmies == 3 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player 1 destroys an army on \"P\" from Player 3." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    int pArmies;
    string pName = "P";
    Vertex* pVertex = map->getVertices()->find(pName)->second;

    player3->getArmiesOnCountry(pVertex);
    result = player3->MoveOverLand(1, startVertex, pVertex);
    result = result && player1->DestroyArmy(pVertex, player3);
    pArmies = player3->getArmiesOnCountry(pVertex);

    assert( result == 1 && pArmies == 0 );

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player 1 fails to destroy an army on \"P\" from Player 2." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    player2->getArmiesOnCountry(pVertex);
    result = player1->DestroyArmy(pVertex, player2);
    pArmies = player2->getArmiesOnCountry(pVertex);

    assert( result == 0 && pArmies == 0);

    cout << "\n--------------------------------------------------------" << endl;
    cout << "TEST: Player 1 fails to destroy own army on \"P\"." << endl;
    cout << "--------------------------------------------------------\n" << endl;

    result = player1->MoveOverLand(1, startVertex, pVertex);
    player1->getArmiesOnCountry(pVertex);
    result = result && player1->DestroyArmy(pVertex, player1);
    pArmies = player1->getArmiesOnCountry(pVertex);

    assert( result == 0 && pArmies == 1 );

    delete map;
    delete player1;
    delete player2;
    delete player3;

    map = nullptr;
    startVertex = nullptr;
    player1 = nullptr;
    player2 = nullptr;
    player3 = nullptr;
    pVertex = nullptr;
}