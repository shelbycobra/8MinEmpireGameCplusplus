#ifndef CARDS_H
#define CARDS_H

#include "Player.h"

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <set>
#include <map>

using namespace std;

class Player;

const string WOOD = "WOOD";
const string IRON = "IRON";
const string CARROT = "CARROT";
const string GEM = "GEM";
const string STONE = "STONE";
const string WILD = "WILD";
const string NONE = "NONE";

class Card {
    int* id;
    string* good;
    string* action;
    int* position;
    int* cost;

public:
    Card();
    Card(const int& theId, const string& theGood, const string& theAction);
    Card(Card* card);
    Card& operator=(Card& card);
    virtual ~Card();

    int getID() { return *id; }
    string getGood() { return *good; }
    string getAction() { return *action; }
    int getPosition() { return *position; }
    int getCost() { return *cost; }
    virtual string getType() { return NONE; }

    void setPosition(int& position) { *this->position = position; }
    void setCost(int& cost) { *this->cost = cost; }
};

class Deck {
    queue<pair<int, Card*>>* cardDeck;
    map<int, Card*>* cardMap;

public:
    Deck();
    Deck(Deck* deck);
    Deck& operator=(Deck& deck);
    ~Deck();

    Card* draw();
    void shuffle();

    queue<pair<int, Card*>>* getDeck() { return cardDeck; }

private:
    int generateRandomInt(set<int>* nums);
};

class Hand {
    vector<Card*>* hand;
    Deck* deck;

public:
    Hand();
    Hand(Hand* hand);
    Hand& operator=(Hand& hand);
    ~Hand();

    void fill();
    Card* exchange(Player* player);
    void drawCardFromDeck();
    void printHand();

    vector<Card*>* getHand() { return hand; }
    Deck* getDeck() { return deck; }

private:
    int selectCardPosition(Player* player);
};

#endif