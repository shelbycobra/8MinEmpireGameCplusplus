#include "../Cards.h"
#include "../util/MapUtil.h"
#include "../util/TestUtil.h"
#include "../Map.h"
#include "../MapLoader.h"

int main() {
    // GameMap* map = generateValidMap();

    GameMap* map = loadMap("../maps/gotmap/got.map");

    string startName = "CL";
    map->setStartVertex(startName);
    Vertex* startVertex = map->getVertices()->find(startName)->second;

    Hand* gameHand = new Hand();

    Players* players = createDummyPlayers(3);
    queue<Player*>* nextTurn = new queue<Player*>();

    cout << "\n[ GAME ] Each player adds 3 armies to the start vertex.\n" << endl;

    for (pair<string, Player*> player: *players) {
        player.second->placeNewArmies(3, startVertex, startName);
        nextTurn->push(player.second);
    }

    while(true) {
        Player* currentPlayer = nextTurn->front();
        nextTurn->pop();
        nextTurn->push(currentPlayer);

        cout << "\n\n\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;

        Card* card = gameHand->exchange(currentPlayer);
        performCardAction(currentPlayer, card->getAction(), map, players);
    }
}
