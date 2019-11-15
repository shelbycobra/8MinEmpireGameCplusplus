#include "../GameMainLoop.h"

int main() {
    MainGameEngine gameEngine;

    gameEngine.startGame();

    int maxNumCards = gameEngine.getMaxNumberOfCards();

    while(true) {

        gameEngine.getNextPlayer();
        gameEngine.chooseCardFromHand();

        gameEngine.performCardAction();
        gameEngine.addNewCardToBackOfHand();

        if (!gameEngine.continueGame(maxNumCards))
            break;
    }

    gameEngine.declareWinner();

    return 0;
}