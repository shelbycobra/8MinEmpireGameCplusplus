#include "GameEngine.h"
#include "GameObservers.h"
#include "PlayerStrategies.h"
#include "GameStartUp.h"
#include <algorithm>

#define NUM_ROUNDS 30

/**
 * Default Constructor
 */
GameEngine::GameEngine() {}

/**
 * Destructor
 */
GameEngine::~GameEngine() {}

/**
 * Default Constructor
 */
MainGameEngine::MainGameEngine(): GameEngine() {}

/**
 * Destructor
 */
MainGameEngine::~MainGameEngine() {
    delete StartUpGameEngine::instance();
    currentPlayer = nullptr;
    currentCard = nullptr;
}

/**
 * Runs the whole game in Normal mode.
 */
void MainGameEngine::runGame() {
    while(continueGame()) {
        getNextPlayer();
        chooseCardFromHand();
        performCardAction();
        addNewCardToBackOfHand();
        askToChangePlayerStrategy();
    }
}

/**
 * Gets the next player in the nextTurn queue.
 */
void MainGameEngine::getNextPlayer() {
    queue<Player*>* nextTurn = StartUpGameEngine::instance()->getNextTurnQueue();

    currentPlayer = nextTurn->front();
    nextTurn->pop();
    nextTurn->push(currentPlayer);

    cout << "\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;
}

/**
 * Prompts the current player to select a card from the game hand, which displays 6 card.
 * The price of each card depends on the cards position. Starting from left, the value of
 * each card is 0, 1, 1, 2, 2, 3. The player pays for the selected card immediately after.
 */
void MainGameEngine::chooseCardFromHand() {

    // Get the number of coins from the current player before and after they pay for a card.
    // Add the difference to the game coin supply.

    int startCoins = currentPlayer->getCoins();
    currentCard = StartUpGameEngine::instance()->getHand()->exchange(currentPlayer);
    int endCoins = currentPlayer->getCoins();

    StartUpGameEngine::instance()->addCoinsToSupply(startCoins - endCoins);
}

/**
 * Executes the action of the current card on the current player.
 *
 */
void MainGameEngine::performCardAction() {
    Players* players = StartUpGameEngine::instance()->getPlayers();

    const string action = currentCard->getAction();

    string answer;
    cout << "[ GAME ] Ignore card action < " << action << " > (y/n)?" << endl;
    cout << "[ GAME ] > ";
    getline(cin, answer);

    if (answer == "y" || answer == "Y") {
        currentPlayer->Ignore();
    } else {
        if(action.find("OR") != size_t(-1) || action.find("AND") != size_t(-1))
            currentPlayer->AndOrAction(action, players);
        else if (action.find("Add") != size_t(-1))
            currentPlayer->PlaceNewArmies(action, players);
        else if (action.find("Destroy") != size_t(-1))
            currentPlayer->DestroyArmy(players);
        else if (action.find("Build") != size_t(-1))
            currentPlayer->BuildCity();
        else if (action.find("Move") != size_t(-1)) {
            if (action.find("water") != size_t(-1))
                currentPlayer->MoveOverWater(action, players);
            else
                currentPlayer->MoveOverLand(action, players);
        }
        else
            cout << "[ ERROR! ] Invalid action." << endl;
    }

    // Notify observers of changes
    Notify();
}

/**
 * Draws a card from the deck and adds it to the 6th position of the game hand,
 * effectively shifting each card left one slot up until the empty slot is filled.
 */
void MainGameEngine::addNewCardToBackOfHand() {
    StartUpGameEngine::instance()->getHand()->drawCardFromDeck();
}

/**
 * Checks whether the game should continue based on the number of cards
 * in each player's hand.
 *
 * @return A boolean indicating whether the game should continue. Returns
 * false only when all players have reached the maxNumCards.
 */
bool MainGameEngine::continueGame() {
    int maxNumCards = getMaxNumberOfCards();

    Players::iterator it;
    Players* players = StartUpGameEngine::instance()->getPlayers();

    for(it = players->begin(); it != players->end(); ++it) {
        if (it->first != ANON && it->second->getHand()->size() < size_t(maxNumCards))
            return true;
    }

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "                      E N D  O F  T H E  G A M E " << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

    return false;
}

/**
 * Declares the winner of the game.
 *
 * Calls on each player to calculate their total score and uses that information to determine
 * the winner.
 *
 * From the Game Rules:
 *  "The player who has the most victory points from regions, continents,
 * and goods has the most powerful empire and is the winner! If
 * players are tied, the player with the most coins wins. If still tied, the player
 * with the most armies on the board wins. If still tied, the player with the
 * most controlled regions wins.""
 */
void MainGameEngine::declareWinner() {

    cout << "\n[ GAME ] Finding the winner.\n" << endl;

    Player* winner = new Player();
    int highestScore = 0;

    Players* players = StartUpGameEngine::instance()->getPlayers();

    map<Player*, int> scores;

    for(Players::iterator it = players->begin(); it != players->end(); it++) {
        int playerScore = it->second->ComputeScore();

        scores.insert(pair<Player*, int>(it->second, playerScore));

        if(playerScore > highestScore) {
            highestScore = playerScore;
            winner = it->second;
        }
    }

    if (highestScore > 0) {
        for(map<Player*, int>::iterator it = scores.begin(); it != scores.end(); it++) {
            int winnerArmies = 14 - winner->getArmies();
            int otherArmies = 14 - it->first->getArmies();

            if (it->second == highestScore && it->first != winner) {
                cout << "[ GAME ] " << it->first->getName() << " has the same score as " << winner->getName() << "." << endl;
                cout << "\n[ GAME ] Comparing number of coins instead ... " << endl;
                cout << "[ GAME ] " << it->first->getName() << " has " << it->first->getCoins()
                     << " coins and " << winner->getName() << " has " << winner->getCoins()
                     << " coins." << endl;

                if (it->first->getCoins() > winner->getCoins()) {
                    winner = it->first;
                    break;
                }
            } else {
                break;
            }

            if (it->first->getCoins() == winner->getCoins()) {
                cout << "[ GAME ] " << it->first->getName() << " has the same number of coins as " << winner->getName() << "." << endl;
                cout << "\n[ GAME ] Comparing number of armies instead ..." << endl;
                cout << "[ GAME ] " << it->first->getName() << " has " << otherArmies
                     << " armies and " << winner->getName() << " has " << winnerArmies
                     << " armies." << endl;

                if (otherArmies > winnerArmies) {
                    winner = it->first;
                    break;
                }
            } else {
                break;
            }

            if (otherArmies == winnerArmies) {
                cout << "[ GAME ] " << it->first->getName() << " has the same number of armies as " << winner->getName() << "." << endl;
                cout << "\n[ GAME ] Comparing number of controlled regions instead ..." << endl;
                cout << "[ GAME ] " << it->first->getName() << " has " << it->first->getControlledRegions()
                     << " controlled regions and " << winner->getName() << " has " << winner->getControlledRegions()
                     << " controlled regions." << endl;

                if (it->first->getControlledRegions() > winner->getControlledRegions()) {
                    winner = it->first;
                    break;
                } else { // No one won.
                    winner = new Player();
                    break;
                }
            } else {
                break;
            }
        }
    }

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "                     W I N N E R  I S  " << winner->getName() << endl;
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;
}

/**
 * Gets the maxumim number of cards the players can have. This is the number of cards
 * each player must have in order to end the game and calculate the winner.
 *
 * The max number of cards depends on the number of players.
 * 2 Players -> 13 Cards
 * 3 Players -> 10 Cards
 * 4 Players -> 8 Cards
 * 5 Players -> 7 Cards
 *
 * @return The number of cards in each player's hand required to end the game.
 */
int MainGameEngine::getMaxNumberOfCards() {
    int numPlayers = StartUpGameEngine::instance()->getNumPlayers();

    if (numPlayers == 2)
        return 13;
    else if (numPlayers == 3)
        return 10;
    else if (numPlayers == 4)
        return 8;
    else if (numPlayers == 5)
        return 7;
    else {
        cout << "[ ERROR! ] Invalid number of players." << endl;
        return 0;
    }
}

/**
 * Asks the user if they want to change the strategy of the current player.
 */
void MainGameEngine::askToChangePlayerStrategy() {
    string answer;
    cout << "[ GAME ] Would you like to change the playing strategy of { " << currentPlayer->getName() << " }?" << endl;
    cout << "[ GAME ] (y/*) > ";
    getline(cin, answer);

    Strategy* newStrategy;

    if (answer == "y" || answer == "Y") {
        while (true) {
            string strategyNum;

            cout << "\n[ GAME ] Which playing strategy would you like to change to?" << endl;
            cout << "[ GAME ] Current playing strategy is " << currentPlayer->getStrategy()->getType() << "." << endl;
            cout << "[ GAME ] Options:" << endl;
            cout << "\n1. HUMAN\n2. GREEDY\n3. MODERATE\n" << endl;
            cout << "[ GAME ] Please enter a number between 1 and 3." << endl;
            cout << "[ GAME ] > ";

            getline(cin, strategyNum);

            try{
                int choice = stoi(strategyNum);

                if (choice == 1) {
                    newStrategy = new HumanStrategy();
                    break;
                } else if (choice == 2) {
                    newStrategy = new GreedyStrategy();
                    break;
                } else if (choice == 3) {
                    newStrategy = new ModerateStrategy();
                    break;
                } else {
                    cout << "[ ERROR! ] Invalid choice." << endl;
                }
            } catch (invalid_argument &e) {
                cout << "[ ERROR! ] Please enter a number." << endl;
            }
        }

        cout << "[ GAME ] Setting strategy to " << newStrategy->getType() << "." << endl;
        currentPlayer->setStrategy(newStrategy);
    }
}

/**
 * Default Constructor
 */
TournamentGameEngine::TournamentGameEngine(): GameEngine() {}

/**
 * Destructor
 */
TournamentGameEngine::~TournamentGameEngine() {
    delete StartUpGameEngine::instance();
    currentPlayer = nullptr;
    currentCard = nullptr;
}

/**
 * Runs game in tournament mode for NUM_ROUNDS times.
 */
void TournamentGameEngine::runGame() {

    for (int i = 0; i < NUM_ROUNDS; i++) {
        getNextPlayer();
        chooseCardFromHand();
        performCardAction();
        addNewCardToBackOfHand();
        Notify();
    }

    declareWinner();
}

/**
 * Gets the next player in the nextTurn queue.
 */
void TournamentGameEngine::getNextPlayer() {
    queue<Player*>* nextTurn = StartUpGameEngine::instance()->getNextTurnQueue();

    currentPlayer = nextTurn->front();
    nextTurn->pop();
    nextTurn->push(currentPlayer);

    cout << "\n\n[ PLAYER TURN ] " << currentPlayer->getName() << ".\n" << endl;

}

/**
 * Prompts the current player to select a card from the game hand, which displays 6 card.
 * The price of each card depends on the cards position. Starting from left, the value of
 * each card is 0, 1, 1, 2, 2, 3. The player pays for the selected card immediately after.
 */
void TournamentGameEngine::chooseCardFromHand() {

    // Get the number of coins from the current player before and after they pay for a card.
    // Add the difference to the game coin supply.

    int startCoins = currentPlayer->getCoins();
    currentCard = StartUpGameEngine::instance()->getHand()->exchange(currentPlayer);
    int endCoins = currentPlayer->getCoins();

    StartUpGameEngine::instance()->addCoinsToSupply(startCoins - endCoins);
}

/**
 * Executes the action of the current card on the current player.
 *
 */
void TournamentGameEngine::performCardAction() {
    Players* players = StartUpGameEngine::instance()->getPlayers();

    const string action = currentCard->getAction();

    if(action.find("OR") != size_t(-1) || action.find("AND") != size_t(-1))
        currentPlayer->AndOrAction(action, players);
    else if (action.find("Add") != size_t(-1))
        currentPlayer->PlaceNewArmies(action, players);
    else if (action.find("Destroy") != size_t(-1))
        currentPlayer->DestroyArmy(players);
    else if (action.find("Build") != size_t(-1))
        currentPlayer->BuildCity();
    else if (action.find("Move") != size_t(-1)) {
        if (action.find("water") != size_t(-1))
            currentPlayer->MoveOverWater(action, players);
        else
            currentPlayer->MoveOverLand(action, players);
    }
    else
        cout << "[ ERROR! ] Invalid action." << endl;
}

/**
 * Draws a card from the deck and adds it to the 6th position of the game hand,
 * effectively shifting each card left one slot up until the empty slot is filled.
 */
void TournamentGameEngine::addNewCardToBackOfHand() {
    StartUpGameEngine::instance()->getHand()->drawCardFromDeck();
}

/**
 * Declares the winner of the game.
 *
 * Calls on each player to calculate their total score and uses that information to determine
 * the winner.
 *
 * From the Game Rules:
 *  "The player who has the most victory points from regions, continents,
 * and goods has the most powerful empire and is the winner! If
 * players are tied, the player with the most coins wins. If still tied, the player
 * with the most armies on the board wins. If still tied, the player with the
 * most controlled regions wins.""
 */
void TournamentGameEngine::declareWinner() {

    cout << "\n[ GAME ] Finding the winner.\n" << endl;

    Player* winner = new Player();
    int highestScore = 0;

    Players* players = StartUpGameEngine::instance()->getPlayers();

    map<Player*, int> scores;

    for(Players::iterator it = players->begin(); it != players->end(); it++) {
        int playerScore = it->second->ComputeScore();

        scores.insert(pair<Player*, int>(it->second, playerScore));

        if(playerScore > highestScore) {
            highestScore = playerScore;
            winner = it->second;
        }
    }

    bool isDraw = false;
    string playersInDraw = "";
    string winnerName = winner->getName();

    transform(winnerName.begin(), winnerName.end(), winnerName.begin(), ::toupper);

    if (highestScore > 0) {

        for(map<Player*, int>::iterator it = scores.begin(); it != scores.end(); it++) {

            if (it->second == highestScore && it->first != winner) {
                isDraw = true;

                // Add player's name to list of players in the draw.
                string playerName = it->first->getName();
                transform(playerName.begin(), playerName.end(), playerName.begin(), ::toupper);
                playersInDraw.append("  &  ").append(playerName);
            }
        }
    }

    vector<Player*> playerPtrs;

    unordered_map<string, Player*>::iterator it;

    for(it = players->begin(); it != players->end(); it++) {
        playerPtrs.push_back(it->second);
    }

    cout << endl;
    cout << " -------------------------------------------------------------- " << endl;
    cout << "| Player Number | Player Name | Cards | Victory Points | Coins |" << endl;
    cout << "|---------------|-------------|-------|----------------|-------|" << endl;

    int playerNum = 1;
    vector<Player*>::reverse_iterator r_it;

    for(r_it = playerPtrs.rbegin(); r_it != playerPtrs.rend(); r_it++) {
        string name = (*r_it)->getName();
        int cards = (*r_it)->getHand()->size();
        int points = scores.at(*r_it);
        int coins = (*r_it)->getCoins();

        printf( "|       %d       | %-11s |   %-3d |        %-7d |   %-3d |\n",
            playerNum, name.c_str(), cards, points, coins);

        playerNum++;
    }

    cout << " -------------------------------------------------------------- " << endl;

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------" << endl;

    if (isDraw)
        cout << "          DRAW  BETWEEN  " << winnerName << playersInDraw << endl;
    else
        cout << "                     W I N N E R  I S  " << winnerName << endl;

    cout << "---------------------------------------------------------------------------" << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

}

/**
 * Creates a GameEngine object.
 *
 * @return A TournamentGameEngine (tournament mode) if InitGameEngine isTournament boolean is true,
 * else return a MainGameEngine (normal mode)
 */
GameEngine* GameEngineFactory::create() {
    if (InitGameEngine::instance()->isTournament()) {
        return new TournamentGameEngine();
    } else {
        return new MainGameEngine();
    }
}