#include "Cards.h"
#include "util/MapUtil.h"

#include <stdio.h>
#include <set>
#include <map>
#include <stdlib.h>
#include <time.h>

using namespace std;

Card::Card():
    id(new int(0)), 
    good(new string("No good")), 
    action(new string("No action")) {}

Card::Card(int theId, string theGood, string theAction): 
    id(new int(theId)), 
    good(new string(theGood)), 
    action(new string(theAction)) {}

Card::Card(Card* card) {
    id = new int(card->getID());
    good = new string(card->getGood());
    action = new string(card->getAction());
}

Card& Card::operator =(Card& card){
    id = new int(card.getID());
    good = new string(card.getGood());
    action = new string(card.getAction());
    return *this;
}

Card::~Card() {
    delete id;
    delete good;
    delete action;

    id = 0;
    good = 0;
    action = 0;
}

int Card::getID(){return *id;}
string Card::getGood(){return *good;}
string Card::getAction(){return *action;}

Deck::Deck(){
    string resources[] = {"GEM", "IRON", "STONE", "WOOD", "CARROT"};
    string actions[] = {"Add 3 armies", "Add 2 armies", "Add 4 armies", "Move 3 armies", "Move 5 armies",
    "Move 4 armies", "Move 5 armies over water", "Move 4 armies over water", "Build city", "Destroy army"};

    int numResources = 5;
    int numActions = 10;

    int resourceIndex = 0;
    int actionIndex = 0;
    int count = 0;
    int joinedCard = 0;

    map<int, Card*>* cards = new map<int,Card*>();

    for(int i = 0; i < 42; i++) {

        string good = resources[resourceIndex];
        string action = actions[actionIndex];

        // if count divisible by 14, make OR with actionIndex and 8 or 9
        if (count % 7 == 0) {
            if (actionIndex == 7 + joinedCard) { // prevents the same action being or'd
                action = actions[actionIndex-2] + " OR " + actions[8 + joinedCard];
            } else
                action = actions[actionIndex] + " OR " + actions[8 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        // if count divisible by 10, make AND actionIndex and 7 or 8
        if (count % 10 == 0) {
            if (actionIndex == 7 + joinedCard) { // prevents the same action being and'd
                action = actions[actionIndex-2] + " AND " + actions[8 + joinedCard];
            } else
                action = actions[actionIndex] + " AND " + actions[8 + joinedCard];
            joinedCard = joinedCard == 0 ? 1 : 0;
        }

        Card* newCard = new Card(count, good, action);
        cards->insert(pair<int, Card*> (count, newCard));

        actionIndex = (actionIndex + 1) % numActions;
        resourceIndex = (resourceIndex + 1) % numResources;
        count++;
    }

    set<int>* nums = new set<int>();
    cardDeck = new queue<Card*>();

    while(nums->size() < 42) {
        int rand = generateRandomInt(nums);
        Card* card = cards->find(rand)->second;
        cardDeck->push(card);
    }

}

Deck::Deck(Deck* deck) {
    cardDeck = new queue<Card*>(*deck->getDeck());
}

Deck& Deck::operator =(Deck& deck) {
    cardDeck = new queue<Card*>(*deck.getDeck());
    return *this;
}

Deck::~Deck(){
    delete cardDeck;
    cardDeck = 0;
}

Card* Deck::draw(){
    Card* card = cardDeck->front();
    cardDeck->pop();
    cout << "[ DECK ] Drew card { " << card->getGood() << " : \"" << card->getAction() << "\" } from the deck." << endl;
    return card;
}

queue<Card*>* Deck::getDeck(){
    return cardDeck;
}

int Deck::generateRandomInt(set<int>* nums){
    srand (time(0));

    while (true){
        int num = rand() % 42;
        if(nums->find(num) == nums->end()) {
            nums->insert(num);
            return num;
        }
    }
}

Hand::Hand(): hand(new vector<Card*>()), deck(new Deck()) {
    //Populate game hand
    for(int i = 0; i < 6; i++) {
        hand->push_back(deck->draw());
    }
}

Hand::Hand(Hand* otherGameHand) {
    hand = new vector<Card*>(*otherGameHand->getHand());
    deck = new Deck(otherGameHand->getDeck());
}

Hand& Hand::operator=(Hand& otherGameHand) {
    hand = new vector<Card*>(*otherGameHand.getHand());
    deck = new Deck(otherGameHand.getDeck());
}

Hand::~Hand(){
    delete hand;
    hand = 0;
}

Card Hand::exchange(Player* player){
    int position;
    int values[] = {0, 1, 1, 2, 2, 3};

    while(true) {
        position = selectPositionOfCardFromGameHand();
        if (player->payCoins(values[position])) {

            Card* card = hand->at(position);

            vector<Card*>::iterator it;
            for(it = hand->begin(); it != hand->end(); ++it) {
                if (*it == card) {
                    cout << "[ GAME HAND ] Removed card { " << (*it)->getGood() << " : \"" << (*it)->getAction() << "\" } from game hand."<< endl;
                    hand->erase(it);
                    break;
                }
            }

            hand->push_back(deck->draw());
            player->addCardToHand(card);

            return *card;
        }
    }
}

int Hand::selectPositionOfCardFromGameHand(){
    string pos;
    int position;

    printHand();

    while (true) {
        cout << "[ GAME HAND ] Please choose a card from the game hand." << endl;
        cout << "[ GAME HAND ] > ";
        getline(cin, pos);

        stringstream toInt(pos);
        toInt >> position;

        if (position < 7 && position > 0)
            return position - 1;
        cout << "[ ERROR! ] You can only choose numbers between [1, 6]. Please try again." << endl;
    }
}

void Hand::printHand() {
    cout << "\n[ GAME HAND ] C U R R E N T   H A N D" << endl;
    int values[] = {0, 1, 1, 2, 2, 3};
    int count= 0;
    for(Card* c : *hand) {
        printf("%d [ %d ] Card ID: %-5d Good: %-10s Action: %s\n", count+1, values[count], c->getID(), c->getGood().c_str(), c->getAction().c_str());
        count++;
    }
    cout << endl;
}

vector<Card*>* Hand::getHand(){return hand;}
Deck* Hand::getDeck(){return deck;}

int generateRandomInt(set<int>* nums){
    srand (time(0));

    while (true){
        int num = rand() % 42;
        if(nums->find(num) == nums->end()) {
            nums->insert(num);
            return num;
        }
    }
}
