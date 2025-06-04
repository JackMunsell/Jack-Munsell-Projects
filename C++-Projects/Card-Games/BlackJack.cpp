/**
 * A program that lets you play the card game BlackJack.
 * Game is played in the terminal.
 * @author Jack Munsell
*/
#include<iostream>
#include <cstdlib>
#include <ctime>

class Card {
    public:
    int value;
    char suit;

    Card() : value(0), suit(' ') {} // Default Constructor
    Card(int v, char s) : value(v), suit(s) {} // Alternate Constructor given the cards information
};

void createDeck(Card deck[], char suits[]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 13; j++) {
            deck[i * 13  + j] = Card(j + 1, suits[i]);
        }
    }
}

void shuffle(Card deck[]) {
    for (int i = 51; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(deck[i], deck[j]);
    }
}

int main() {
    std::srand(std::time(0));

    Card deck[52];
    char suits[4] = {'D','H','S','C'};
    int values[13];


    createDeck(deck, suits);

}