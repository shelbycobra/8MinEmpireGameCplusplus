COMP 354 2019 - 8 MINUTE EMPIRE

By Shelby Wams #29489177


============================================ASSIGNMENT 4===================================================

Note: To play the full game, run the FullGame Driver.


Part 1: Cards Factory

    DRIVER: CardsFactoryDriver.cpp

        A driver that demonstrates the CardsFactory object. The factory creates one of several card types:
    WoodCard, StoneCard, WildCard, IronCard, CarrotCard, and GemCard.

Part 2: Tournament Mode

    DRIVER: TournamentDriver.cpp

        A driver that demonstrates the Tournament mode of the game. It prompts the user to first select either
    


============================================ASSIGNMENT 3===================================================

Note: To play the full game, run the FullGame Driver.


Part 1: Player Strategies

    DRIVER: StrategiesDriver.cpp

        A driver that demonstrates the different player strategies during game play. After each turn, the user
    is prompted to change the current player's strategy, if they wish. The three strategies are:

    1. Human: Prompts the user for input during game play.
    2. Greedy: Chooses "Build" or "Destroy" cards first.
    3. Moderate: Chooses "Add" or "Move" cards first. Moves armies strategically to maximize the number of owned regions.

Part 2: Phase Observer

    DRIVER: PhaseObserverDriver.cpp

        A driver that demonstates the PhaseObserver class. Prints out a header at the end of each turn that displays
    the current player's card choice, the position of the card and the cost of the card.

Part 3: Statistic Observer

    DRIVER: StatsObserverDriver.cpp

        A driver that demonstrates the StatsObserver class. Prints out the currently occupied regions and countries
    with their owners, if any. Prints out the current victory points from owned regions, countries and goods, as well as
    the number of goods in each player's hand.

Part 4: Map Singleton

    DRIVER: MapSingletonDriver.cpp

        A driver that demonstrates that the GamMap object is a Singleton.

============================================ASSIGNMENT 2===================================================


Note: To play the full game, run the FullGame Driver.


Part 1: Game Start

    DRIVER: GameInitDriver.cpp

        An interactive driver that tests the InitGameEngine class. It loads a map from one of
    the map files in the maps/ directory and creates the players for the game.


Part 2: Game Play: Startup Phase

    DRIVER: GameStartUpDriver.cpp

        An interactive driver that tests the GameStartEngine class. The class uses a InitGameEngine
    object to initialize the map and the players before setting up the map with the starter armies,
    starting the bidding process and determining which player goes first.


Part 3: Game Play: Main Game Loop

    DRIVER: MainLoopDriver.cpp

        An interactive driver that tests part of the MainGameEngine class. It uses the StartUpGameEngine to start
    the game. It then runs a game loop with each player turn starting with the first and going clockwise order,
    which, in other words means, going in player creation order starting with the first player chosen by the bid winner.
    Each player takes one face-up card and pays the appropriate cost for the card depending on its position in
    the game hand. The card is displayed before adding it to the player's hand.


Part 4: Main Game Loop: The Player Actions

    DRIVER: PlayerActionsDriver.cpp

        An interactive driver that demonstrates the possible actions that a player can take when they
    pay for a card from the gam hand. This driver doesn't run in a loop since it only uses a limited
    set of cards for demo purposes.


Part 5: Main Game Loop: After The Action

    DRIVER: AfterActionDriver.cpp

        An interactive driver that demonstrates part of the MainGameEngine class. It shows the behaviour after
    the player completes the card action. A card gets drawn from the deck and placed at the back of the game
    hand and the play passes to the next clockwise player.


Part 6: Main Game Loop: Compute The Game Score

    DRIVER: GameScoreDriver.cpp

        An interactive driver that demonstrates part of the MainGameEngine class and the Player ComputeScore methods.
    This driver uses a separate class called ScoreTest that is a friend of the Player class for easier testing.
    Each test in the driver requires specific input:

    1. test_onePlayerWinsGame()                 2. test_twoPlayersTieButHaveDifferentNumCoins
        INPUT:                                      INPUT:
            got.map                                     got.map
            3                                           3
            <name 1>                                    <name 1>
            <colour 1>                                  <colour 1>
            <name 2>                                    <name 2>
            <colour 2>                                  <colour 2>
            <name 3>                                    <name 3>
            <colour 3>                                  <colour 3>
            any vertex                                  any vertex
            any bid                                     3
            any bid                                     2
            any bid                                     1


    3. test_twoPlayersTieAndHaveSameNumCoins     4. test_twoPlayersTieAndHaveSameNumCoinsAndSameNumArmies
        INPUT:                                      INPUT:
            got.map                                     got.map
            3                                           3
            <name 1>                                    <name 1>
            <colour 1>                                  <colour 1>
            <name 2>                                    <name 2>
            <colour 2>                                  <colour 2>
            <name 3>                                    <name 3>
            <colour 3>                                  <colour 3>
            CL                                          CL
            0                                           0
            0                                           0
            0                                           0

    5. test_playerHasWildCard                    6. test_gameEndsWhenPlayersHaveMaxNumCards
        INPUT:                                      INPUT:
            got.map                                     got.map
            3                                           2, 3, 4, 5 (13, 10, 8, 7 cards)
            <name 1>
            <colour 1>
            <name 2>
            <colour 2>
            <name 3>
            <colour 3>
            CL
            3
            2
            1



============================================ASSIGNMENT 1===================================================


0. DESCRIPTION

8 Minute Empire is a area control game via the console. The current implementation creates some key classes
and tests them, but it does not include a full game loop yet. However, if you run the Cards driver,
you will be able to interact with the game in a way that closely mimics what the game loop might be like.

1. SRC DIRECTORY STRUCTURE

-- src
    |-- Bidder.cpp
    |-- Bidder.h
    |-- Cards.cpp
    |-- Cards.h
    |-- Map.cpp
    |-- Map.h
    |-- MapLoader.cpp
    |-- MapLoader.h
    |-- Player.cpp
    |-- Player.h
    |
    |-- drivers
    |   |-- BidderDriver.cpp
    |   |-- CardsDriver.cpp
    |   |-- MapDriver.cpp
    |   |-- MapLoaderDriver.cpp
    |   `-- PlayerDriver.cpp
    |
    `-- util
        |-- MapUtil.cpp
        |-- MapUtil.h
        |-- TestUtil.cpp
        `-- TestUtil.h

2. RUNNING THE DRIVERS

    2.a. If on Mac or Linux:
        Open a terminal window and change directory into the root directory of the project.
        Execute `./run_driver <driver_name>`, where 'driver_name' is either Player, Map, MapLoader, Bidder or Cards.

    2.b If on Windows:
        Open up Visual Studio.
        Create a New Project from Existing Source Code.
        Check project configuration in Properties->Linker->System->SubSystem. It should be CONSOLE.
        Run the project in debug. Make sure to only include the driver class you want to run since each driver has
        its own main().

3. ABOUT THE DRIVERS

    3.a Map
        - Creates several example maps and tests their validity according to the assignment criteria.

    3.b MapLoader
        - Reads from several map files located in the maps/ directory to test the map loader functions.

    3.c Player
        - Demonstrates the manipulation by a Player object on the construction, destruction and movement
        of armies and/or cities on a map.
        - One section of the driver tests the bidder object and requires input from the user.

    3.d Bidder
        - Tests the bidding function between several players.
        - In order to test the different scenarios that may occur during bidding, run the
        Bidder driver several times and play with different inputs.

    3.c Cards
        - Part 1 tests that the Deck object contains 42 cards and part 2 tests the Hand object, 
        the Cards visible from the Hand object, and the different actions the player can take.
        - This driver is very interactive!
