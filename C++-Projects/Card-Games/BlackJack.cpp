/**
 * A program that lets you play the card game BlackJack.
 * Game is played in the terminal.
 * @author Jack Munsell
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <random>

class Card {
    private:
        int value;
        char suit;
    public:
        Card() : value(0), suit(' ') {} // Default Constructor
        Card(int v, char s) : value(v), suit(s) {} // Alternate Constructor given the cards information

        int getValue() {
            return value;
        }
        char getSuit() {
            return suit;
        }
        void setValue(int newValue) {
            value = newValue;
        }
        void setSuit(char newSuit) {
            suit = newSuit;
        }

};

std::string cardToString(Card cardStack) {
    switch(cardStack.getSuit()) {
        case 'D':
            if(cardStack.getValue() > 0 && cardStack.getValue() < 10) {
                return std::to_string(cardStack.getValue() + 1) + " of Diamonds";
                break;
            }
            if(cardStack.getValue() == 10) {
                return "Jack of Diamonds";
                break;
            }
            if(cardStack.getValue() == 11) {
                return "Queen of Diamonds";
                break;
            }
            if(cardStack.getValue() == 12) {
                return "King of Diamonds";
                break;
            }
            if(cardStack.getValue() == 13) {
                return "Ace of Diamonds";
                break;
            }
        case 'H':
            if(cardStack.getValue() > 0 && cardStack.getValue() < 10) {
                return std::to_string(cardStack.getValue() + 1) + " of Hearts";
                break;
            }
            if(cardStack.getValue() == 10) {
                return "Jack of Hearts";
                break;
            }
            if(cardStack.getValue() == 11) {
                return "Queen of Hearts";
                break;
            }
            if(cardStack.getValue() == 12) {
                return "King of Hearts";
                break;
            }
            if(cardStack.getValue() == 13) {
                return "Ace of Hearts";
                break;
            }
        case 'S':
            if(cardStack.getValue() > 0 && cardStack.getValue() < 10) {
                return std::to_string(cardStack.getValue() + 1) + " of Spades";
                break;
            }
            if(cardStack.getValue() == 10) {
                return "Jack of Spades";
                break;
            }
            if(cardStack.getValue() == 11) {
                return "Queen of Spades";
                break;
            }
            if(cardStack.getValue() == 12) {
                return "King of Spades";
                break;
            }
            if(cardStack.getValue() == 13) {
                return "Ace of Spades";
                break;
            }
        case 'C':
            if(cardStack.getValue() > 0 && cardStack.getValue() < 10) {
                return std::to_string(cardStack.getValue() + 1) + " of Clubs";
                break;
            }
            if(cardStack.getValue() == 10) {
                return "Jack of Clubs";
                break;
            }
            if(cardStack.getValue() == 11) {
                return "Queen of Clubs";
                break;
            }
            if(cardStack.getValue() == 12) {
                return "King of Clubs";
                break;
            }
            if(cardStack.getValue() == 13) {
                return "Ace of Clubs";
                break;
            }
        default: 
            return " ";
            break;
    }
    return " ";
}

void createDeck(Card deck[], char suits[]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 13; j++) {
            deck[i * 13  + j] = Card(j + 1, suits[i]);
        }
    }
}

void shuffle(Card deck[]) {
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(deck, deck + 52, g);
}

int main() {
    std::srand(std::time(0));

    Card deck[52];
    char suits[4] = {'D','H','S','C'};
    int values[13];


    createDeck(deck, suits);

}