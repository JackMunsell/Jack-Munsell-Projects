/**
 * A program that lets you play the card game War against the computer.
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
    public:
    int value;
    char suit;
    
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

std::stack<Card> playerCards;
std::stack<Card> computerCards;

std::string cardToString(Card cardStack) {
    switch(cardStack.suit) {
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
    std::shuffle(deck, deck + 52, g);
}

void dealCards(Card deck[]) {
    for(int i = 1; i < 53; i++) {
        if(i % 2 == 0) // If the index is even card goes to player, if index is odd card goes to computer
            playerCards.push(deck[i - 1]);
        else
            computerCards.push(deck[i - 1]);
    }
}
/**
 * @param card1 the winners top card
 * @param card2 the losers top card
*/
void appendToStackBottom(Card card1, Card card2, bool playerWon) {
    std::stack<Card> tempCards;

    if(playerWon) {

        // transfers playercards stack to tempcards stack, empties playercards
        while (!playerCards.empty()) {
            tempCards.push(playerCards.top());
            playerCards.pop();
        }
        // adds the cards to the bottom of playercards stack
        playerCards.push(card1); 
        playerCards.push(card2);
        // moves tempcards stack to playercards stack, empties tempcards
        while (!tempCards.empty()) {
            playerCards.push(tempCards.top());
            tempCards.pop();
        }
    } else {

        // transfers computercards stack to tempcards stack, empties computercards
        while (!computerCards.empty()) {
            tempCards.push(computerCards.top());
            computerCards.pop();
        }
        // adds the cards to the bottom of computercards stack
        computerCards.push(card2); 
        computerCards.push(card1);
        // moves tempcards stack to computercards stack, empties tempcards
        while (!tempCards.empty()) {
            computerCards.push(tempCards.top());
            tempCards.pop();
        }
    }
    
}

void appendToStackBottomTie(Card playerCard, Card computerCard) {
    std::cout << "main start" << std::endl;
    std::vector<Card> wonCards;
    std::cout << "main start 2" << std::endl;
    wonCards.push_back(playerCard);
    std::cout << "main start 3" << std::endl;
    wonCards.push_back(computerCard);
    std::cout << "main start 4" << std::endl;


    for(int i = 2; i < 5; i++) {
        std::cout << "main for loop" << std::endl;
        wonCards.push_back(playerCards.top());
        playerCards.pop();
        wonCards.push_back(computerCards.top());
        computerCards.pop();

    }
    // if computer card is higher value then they get all cards
    if(computerCards.top().getValue() > playerCards.top().getValue()) {
        std::cout << "Computer's war card is " << cardToString(computerCards.top()) << std::endl;
        std::cout << "Player's war card is " << cardToString(playerCards.top()) << std::endl;
        std::cout << "Computer's card is higher they win the war" << std::endl;

        std::cout << "computer wins main" << std::endl;
        wonCards.push_back(computerCards.top());
        computerCards.pop();
        wonCards.push_back(playerCards.top());
        playerCards.pop();
        std::cout << "computer wins before for loop" << std::endl;

        for (int i = 0; i < wonCards.size() / 2; i++) {
            std::cout << "computer wins in for loop" << std::endl;

            appendToStackBottom(wonCards[i],wonCards[i+5],false);
        }
    } else if(playerCards.top().getValue() > computerCards.top().getValue()) {
        std::cout << "Player's war card is " << cardToString(playerCards.top()) << std::endl;
        std::cout << "Computer's war card is " << cardToString(computerCards.top()) << std::endl;
        std::cout << "Player's card is higher they win the war" << std::endl;

        std::cout << "player wins main" << std::endl;
        wonCards.push_back(playerCards.top());
        playerCards.pop();
        wonCards.push_back(computerCards.top());
        computerCards.pop();
        std::cout << "player wins before for loop" << std::endl;

        for (int i = 0; i < wonCards.size() / 2; i++) {
            std::cout << "player wins in for loop" << std::endl;

            appendToStackBottom(wonCards[i],wonCards[i+5],true);
        }
    } else {
        std::cout << "why" <<std::endl;
    }
}





int main() {
    std::srand(std::time(0));

    Card deck[52];
    char suits[4] = {'D','H','S','C'};
    int cardValues[10];
    int values[13];


    createDeck(deck, suits);
    shuffle(deck);
    dealCards(deck);
    bool play = true;
    while(play) {
        if(playerCards.empty()) {
            std::cout << "Player out of cards, Computer wins the game! " <<std::endl;
            std::cout << "Computer final deck size: " << computerCards.size() << std::endl;
            std::cout << "Players final deck size: " << playerCards.size() << std::endl;
            break;
        }
        if(computerCards.empty()) {
            std::cout << "Computer out of cards, Player wins the game! " <<std::endl;
            std::cout << "Players final deck size: " << playerCards.size() << std::endl;
            std::cout << "Computer final deck size: " << computerCards.size() << std::endl;
            break;
        }

        Card playerCard = playerCards.top();
        Card computerCard = computerCards.top();
        playerCards.pop();
        computerCards.pop();

        std::cout << "Players card is " << cardToString(playerCard) << std::endl;
        std::cout << "Players deck size: " << playerCards.size() + 1 << std::endl;
        std::cout << "Computer card is " << cardToString(computerCard) << std::endl;
        std::cout << "Computer deck size: " << computerCards.size() + 1 << std::endl;



        if (playerCard.getValue() > computerCard.getValue()) {
            std::cout << "Players card is higher, Player wins!" << std::endl;
            appendToStackBottom(playerCard, computerCard, true);
        } else if (playerCard.getValue() < computerCard.getValue()) {
            std::cout << "Computers card is higher, Computer wins!" << std::endl;
            appendToStackBottom(computerCard, playerCard, false);
        } else {
            if(playerCards.size() < 6) {
                return 0;
            } else if(computerCards.size() < 6) {
                return 0;
            } else {
                std::cout << "Player and Computer cards are equal, going into war!" << std::endl;
                appendToStackBottomTie(playerCard, computerCard);
            }
        }

        if((playerCards.size() + computerCards.size()) != 52) {
            std::cout << "total cards less than 52" << std::endl;
            exit(0);
        }
        std::cout << std::endl;
        // sleep(1);
    }
}