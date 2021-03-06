#include "Bidder.h"
#include "GameInit.h"
#include <time.h>

/**
 * Default constructor
 */
Bidder::Bidder():
    madeBid(new bool(false)),
    player(new Player()),
    bidAmount(new int(0)) {}

/**
 * The Bidder object is initialized with a pointer to the Player who owns the Bidder object
 * and the boolean madeBid to false.
 *
 * The boolean madeBid is used to prevent additional bids after the first one is made by the Player.
 */
Bidder::Bidder(Player* player):
    madeBid(new bool(false)),
    player(player),
    bidAmount(new int(0)) {srand(time(0));}

/**
 * Copy constructor
 */
Bidder::Bidder(Bidder* bidder) {
    madeBid = new bool(bidder->getMadeBid());
    player = new Player(bidder->getPlayer());
    bidAmount = new int(bidder->getBidAmount());
}

/**
 * Assignment operator
 */
Bidder& Bidder::operator=(Bidder& bidder) {
    if (&bidder != this) {
        delete madeBid;
        delete bidAmount;
        // We don't delete the player object since it has other pointers
        // pointing to it throughout the game.

        madeBid = new bool(bidder.getMadeBid());
        player = new Player(bidder.getPlayer());
        bidAmount = new int(bidder.getBidAmount());
    }
    return *this;
}

/**
 * Destructor
 */
Bidder::~Bidder(){
    delete madeBid;
    delete bidAmount;

    madeBid = nullptr;
    player = nullptr;
    bidAmount = nullptr;
}

/**
 * Prompts the Player to make a bid from their purse.
 * If the Player has enough coins to make the bid, the
 * madeBid boolean is set to true.
 */
int Bidder::bid() {
    int bid = -1;
    string bidStr;

    if (!*madeBid) {

        cout << "[ BIDDER ] " << player->getName() << ", please select your bid."
             << " You have " << player->getCoins() << " coins." << endl;
        cout << "[ BIDDER ] > ";

        if (InitGameEngine::instance()->isTournament())
        {

            int maxBid = player->getCoins();
            bid = rand() % (maxBid+1);
            *bidAmount = bid;

            cout << bid << endl;
        }
        else
        {
            while(true) {
                try {

                    getline(cin, bidStr);

                    bid = stoi(bidStr);

                    if (bid <= player->getCoins()) {
                        *bidAmount = bid;
                        break;
                    }

                    cout << "\n[ ERROR! ] You don't have enough coins to make that bid. Please try again.\n" << endl;

                } catch(invalid_argument &e) {
                    cout << "\n[ ERROR! ] Please enter a number.\n" << endl;
                }
            }
        }

    } else {
        cout << "\n[ ERROR! ] A bid has already been made for " << player->getName() << endl;
    }

    *madeBid = true;

    return bid;
}

/**
 * A class function that starts the bidding process for a group of players.
 *
 * @param players A pointer to a vector of Player pointers representing all the players in the game.
 */
Player* Bidder::startBid(Players* players) {
    cout << "\n[ BIDDER ] STARTING BID!\n" << endl;

    unordered_map<Player*, int>* bids = new unordered_map<Player*, int>();

    Players::iterator it;
    for(it = players->begin(); it != players->end(); ++it){

        if (it->first != ANON) {
            int bid = it->second->getBidder()->bid();

            bids->insert(pair<Player*, int>(it->second, bid));
        }
    }

    Player* winner = Bidder::calculateWinner(bids, players);

    winner->PayCoins(bids->find(winner)->second);

    delete bids;
    bids = nullptr;

    return winner;
 }

/**
 * A class function that calculates the winner from the bids of the players in the game.
 *
 * @param bids An unordered_map pointer that maps Players to their bids.
 * @param players A pointer to a vector of Player pointers representing all the players in the game.
 * @return The player who won the bid.
 */
Player* Bidder::calculateWinner(unordered_map<Player*, int>* bids, Players* players) {

    cout << "\n[ BIDDER ] Finding winning bid ... \n\n";

    int max = -1;
    Player* winner;

    unordered_map<Player*, int>::iterator it;
    for(it = bids->begin(); it != bids->end(); ++it) {
        if (it->second > max) {
            winner = it->first;
            max = it->second;
        }
    }

    //check for duplicate bids
    for(it = bids->begin(); it != bids->end(); ++it) {

        if (it->second == max && it->first != winner) {
            cout << "\n[ BIDDER ] There is a tie between " << winner->getName() << " and "
                    << it->first->getName() << " for the highest bid of " << max << ".\n";

            cout << "[ BIDDER ] Who is the younger player?\n";
            cout << "[ BIDDER ] > ";

            if (InitGameEngine::instance()->isTournament())
            {
                cout << winner->getName() << endl;
            }
            else
            {
                while(true) {

                    string youngerPlayer;
                    getline(cin, youngerPlayer);

                    if (youngerPlayer == winner->getName() || youngerPlayer == it->first->getName()) {
                        winner = players->find(youngerPlayer)->second;
                        break;
                    } else {
                        cout << "[ BIDDER ] That name is invalid. Please choose between " << winner->getName() << " and "
                        << it->first->getName() << " for the highest bid of " << max << ".\n";
                    }
                }
            }
        }
    }

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "[ BIDDER ] " << winner->getName() << " has won the bid!" << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

    return winner;
}

/**
 * Prompts the winner to choose which player starts the game.
 *
 * @param winner The winner of the bid phase.
 * @param players A list of all players in the game.
 * @return The first player.
 */
Player* Bidder::getFirstPlayer(Player* winner, Players* players) {
    string name = "";

    cout << "[ BIDDER ] " << winner->getName() << ", please choose who goes first. " << endl;
    cout << "[ BIDDER ] > ";

    if(InitGameEngine::instance()->isTournament())
    {
        name = winner->getName();
        cout << name << endl;
    }
    else
    {
        while (true) {
            getline(cin, name);

            if (players->find(name) != players->end()) {
                break;
            }

            cout << "\n[ ERROR! ] " << name << " doesn't exist among current players.\n" << endl;
        }
    }

    cout << "\n---------------------------------------------------------------------------" << endl;
    cout << "[ BIDDER ] " << name << " goes first!" << endl;
    cout << "---------------------------------------------------------------------------\n" << endl;

    return players->find(name)->second;
}