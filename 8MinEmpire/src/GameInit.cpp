#include <dirent.h>
#include <algorithm>

#include "GameInit.h"
#include "MapLoader.h"
#include "util/MapUtil.h"

InitGameEngine* InitGameEngine::initInstance = 0;

/**
 * Default Constructor
 */
InitGameEngine::InitGameEngine():
    players(new Players()), hand(new Hand()),
    numPlayers(new int()), playerOrder(new vector<string>()),
    isGameTournament(new bool(false)) {
        colours = new list<string>();
        colours->push_front("BLUE");
        colours->push_front("GREEN");
        colours->push_front("YELLOW");
        colours->push_front("RED");
        colours->push_front("WHITE");
    }

/**
 * Destructor
 */
InitGameEngine::~InitGameEngine() {
    for (pair<string, Player*> player: *players) {
        delete player.second;
    }

    delete players;
    delete hand;
    delete numPlayers;
    delete colours;
    delete playerOrder;
    delete isGameTournament;

    initInstance = nullptr;
    players = nullptr;
    hand = nullptr;
    numPlayers = nullptr;
    colours = nullptr;
    playerOrder = nullptr;
    isGameTournament = nullptr;
}

/**
 * Creates the GameMap object and the Players.
 */
void InitGameEngine::initGame() {
    // Only initialize the game once.
    if (players->size() == 0 && GameMap::instance()->getVertices()->size() == 0) {

        cout << "\n---------------------------------------------------------------------------" << endl;
        cout << "---------------------------------------------------------------------------" << endl;
        cout << "                            W E L C O M E  T O " << endl;
        cout << "                      8  M I N U T E  E M P I R E !" << endl;
        cout << "---------------------------------------------------------------------------" << endl;
        cout << "---------------------------------------------------------------------------\n" << endl;

        askGameMode();
        initializeMap();
        selectNumPlayers();
        createPlayers();
        hand->fill();
    }
}

/**
 * Returns a singleton instance of a StartUpGameEngine object.
 */
InitGameEngine* InitGameEngine::instance() {
    if (!initInstance) {
        initInstance = new InitGameEngine();
    }
    return initInstance;
}

void InitGameEngine::setIsTournament(bool isGameTournamentBool) {
    delete isGameTournament;
    isGameTournament = new bool(isGameTournamentBool);
}

//PRIVATE
/**
 * Prompts the user to choose the game mode. Either 'Normal' or 'Tournament'.
 */
void InitGameEngine::askGameMode() {
    while(true) {
        cout << "[ INIT ] Select Mode:" << endl;
        cout << endl;
        cout << "[ INIT ] 1. Normal" << endl;
        cout << "[ INIT ] 2. Tournament" << endl;
        cout << endl;
        cout << "[ INIT ] > ";

        string answer;
        getline(cin, answer);

        try {
            int a = stoi(answer);

            if (a == 1) {
                cout << "[ INIT ] You've chosen Normal Mode." << endl;
                isGameTournament = new bool(false);
                break;
            }
            if (a == 2) {
                cout << "[ INIT ] You've chosen Tournament Mode." << endl;
                delete isGameTournament;
                isGameTournament = new bool(true);
                break;
            }

            cout << "\n[ ERROR! ] Invalid input. Please choose either option 1 or 2.\n" << endl;
        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number.\n" << endl;
        }
    }

}

//PRIVATE
/**
 * Prompts the user to pick among map files found in the maps/ directory and creates
 * a GameMap object from the map file.
 *
 * It validates map files and map objects created from a valid map file.
 *
 */
void InitGameEngine::initializeMap() {

    vector<string>* mapFiles = getMapFiles();

    while(true) {
        string mapName = selectMap(mapFiles);
        MapLoader loader(mapName);

        bool isMapFileValid = loader.generateMap();

        // Map file is invalid and returned no map object.
        if (isMapFileValid == false)
            continue;

        // Map file is valid and created a valid map object.
        if(isConnectedMap() && validateContinents() && validateEdges())
            break;

        // Map file is valid but created a faulty map object.
        delete GameMap::instance();
    }

    delete mapFiles;
    mapFiles = nullptr;
}

//PRIVATE
/**
 * Prompts the user to select the number of players.
 * The user can choose between 2 and 5 players, inclusively.
 */
void InitGameEngine::selectNumPlayers() {
    int maxPlayers = 5;

    if (*isGameTournament)
        maxPlayers = 4;

    const int MAX_NUM_PLAYERS = maxPlayers;
    const int MIN_NUM_PLAYERS = 2;

    while(true) {
        string answer;

        cout << "\n[ INIT ] Please enter the number of players from " << MIN_NUM_PLAYERS << " to " << MAX_NUM_PLAYERS << ":" << endl;
        cout << "[ INIT ] > ";

        getline(cin, answer);

        try {
            int num = stoi(answer);

            if (num >= MIN_NUM_PLAYERS && num <= MAX_NUM_PLAYERS) {
                cout << "[ INIT ] You entered " << num << endl;
                *numPlayers = num;
                break;
            }

            cout << "\n[ ERROR! ] Invalid answer. Please enter a number from " << MIN_NUM_PLAYERS << " to " << MAX_NUM_PLAYERS << "." << endl;

        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number." << endl;
        }
    }
}

//PRIVATE
/**
 * Populates the Players object (typedef unordered_map<string, Player*>) with new Player objects.
 */
void InitGameEngine::createPlayers(){
    vector<Player*> playerList;
    cout << "[ INIT ] Creating " << *numPlayers << " players." << endl;

    for (int i = 0; i < *numPlayers; i++) {
        Player* player = createPlayer();
        players->insert(pair<string, Player*>(player->getName(), player));
        playerOrder->push_back(player->getName());
    }
}

//PRIVATE
/**
 * Creates a Player object.
 *
 * Prompts the user to write in a unique name and choose a colour from
 * the five available colours. Once a player selects a colour, it is no
 * longer available for the other players to choose.
 */
Player* InitGameEngine::createPlayer(){
    string name;
    string colour;
    Strategy* strategy;

    cout << "\n[ INIT ] Creating a new player" << endl;

    name = chooseName();
    colour = chooseColour();
    strategy = chooseStrategy();

    return new Player(name, colour, strategy);
}

//PRIVATE
/**
 * Gets a vector pointer of all the map files in the maps/ directory.
 *
 * @return A vector of the names of the files in the maps/ directory.
 */
vector<string>* InitGameEngine::getMapFiles() {
    vector<string>* files = new vector<string>();

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir ("maps")) != NULL) {

        while ((ent = readdir (dir)) != NULL) {

            //Exclude . and .. from map files
            if(string(ent->d_name).compare(".") == string(ent->d_name).compare(".."))
                files->push_back(string(ent->d_name));
        }

        closedir (dir);
    }

    return files;
}

//PRIVATE
/**
 * Prompts the user to select a map file among the files in the maps/ directory.
 */
string InitGameEngine::selectMap(vector<string>* maps) {
    const int NUM_MAPS = maps->size();

    while(true) {
        string answer;

        cout << "\n[ INIT ] Please select a map [ 1 - " << NUM_MAPS << " ]:\n" << endl;
        for (int i = 0; i < NUM_MAPS; i++)
            cout << i+1 << " : " << maps->at(i) << endl;
        cout << "\n[ INIT ] > ";

        getline(cin, answer);

        try{
            int index = stoi(answer);

            if (index > 0 && index <= NUM_MAPS) {
                string chosenMap = maps->at(index-1);
                cout << "[ INIT ] You have chosen " << chosenMap << endl;
                return chosenMap;
            }

            cout << "\n[ ERROR! ] Invalid answer. Please choose among the available maps." << endl;

        } catch (invalid_argument& e) {
            cout << "\n[ ERROR! ] Invalid input. Please enter a number." << endl;
        }
    }
}

//PRIVATE
/**
 * Prints the available colours that a player can choose from.
 */
void InitGameEngine::printColours() {
    cout << "[ INIT ]  AVAILABLE COLOURS\n" << endl;
    int i = 1;
    for(list<string>::iterator it = colours->begin(); it != colours->end(); ++it) {
        cout << i << ". " << (*it) << endl;
        i++;
    }
    cout << endl;
}

//PRIVATE
/**
 * Prompts the user to choose among available colours.
 */
string InitGameEngine::chooseColour() {
    string colourPos;
    string colour;
    int pos;

    while(true) {
        cout << "\n[ INIT ] Choose a colour for the player:" << endl;
        printColours();
        cout << "[ INIT ] > ";

        getline(cin, colourPos);

        try {
            pos = stoi(colourPos);

            list<string>::iterator it = colours->begin();

            for(int i = 1; i <= (int) colours->size(); i++) {
                if (i == pos) {
                    colour = *it;
                    colours->remove(*it);
                    break;
                }
                it++;
            }

            if (colour != "") {
                cout << "[ INIT ] You chose " << colour << "." << endl;
                break;
            }

            cout << "[ ERROR! ] You must enter a number between 1 and " << colours->size() << "." << endl;
        } catch (invalid_argument &e) {
            cout << "[ ERROR! ] Please enter a number." << endl;
        }
    }

    return colour;
}

//PRIVATE
/**
 * Prompts the user to choose a unique player name.
 */
string InitGameEngine::chooseName() {
    string name;

    while(true) {
        cout << "[ INIT ] Enter name of the player:" << endl;
        cout << "[ INIT ] > ";
        getline(cin, name);

        if(name != "") {
            if(players->find(name) != players->end())
                cout << "\n[ ERROR! ] That name is already taken. Please choose another.\n" << endl;
            else
                break;
        } else
            cout << "[ ERROR! ] Name cannot be empty. Please try again." << endl;
    }

    return name;
}

//PRIVATE
/**
 * Prompts the user to choose an initial strategy for the player being created.
 *
 * @return A pointer to the selected strategy.
 */
Strategy* InitGameEngine::chooseStrategy() {
    while (true) {
        string strategyChoice;

        cout << "\n[ INIT ] Which playing strategy would you like to change to?" << endl;
        cout << "[ INIT ] Options:" << endl;
        cout << "\n1. HUMAN\n2. GREEDY\n3. MODERATE\n" << endl;
        cout << "[ INIT ] Please enter a number between 1 and 3." << endl;
        cout << "[ INIT ] > ";

        getline(cin, strategyChoice);

        cout << "[ INIT ] You chose " << strategyChoice << endl;

        try{
            int choice = stoi(strategyChoice);

            if (choice == 1 && !*isGameTournament)
                return new HumanStrategy();
            if (choice == 2)
                return new GreedyStrategy();
            if (choice == 3)
                return new ModerateStrategy();

            cout << "\n[ ERROR! ] Invalid choice.";
            if (*isGameTournament)
                cout << " Please choose either Greedy or Moderate Strategies.";
            cout << endl << endl;
        } catch (invalid_argument &e) {
            cout << "\n[ ERROR! ] Please enter a number.\n" << endl;
        }
    }
}