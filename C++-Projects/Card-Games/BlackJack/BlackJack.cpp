/**
 * A program that lets you play the card game BlackJack.
 * Game is played in the terminal.
 * @author Jack Munsell
*/
#include <iostream>
#include <cstdlib>
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
        int getCardValue() {
            if(value >= 10 && value < 13) {
                return 10; // Face cards are worth 10
            } else if(value == 13) {
                return 11; // Ace is worth 11
            } else {
                return value + 1; // Number cards are worth their face value
            }
        }

};

std::stack<Card> cardStack;
std::vector<Card> playerHand;
std::vector<Card> dealerHand;
bool playerLost = false;
bool isGameOver = false; // If the game was ended before the endGame function was called
bool isPlayerTurn = true;
int playerCardValue = 0;
int dealerCardValue = 0;
int playerScore = 0;
int dealerScore = 0;
int gamesPlayed = 0;

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
    std::cout<<"----------=Starting BlackJack=---------\n";
    std::cout<<"Building the deck.\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 13; j++) {
            deck[i * 13  + j] = Card(j + 1, suits[i]);
        }
    }
}

void dealCards(Card deck[]) {
    std::cout<<"Dealing cards.\n";
    for(int i = 0; i < 2; i++) {
        playerHand.push_back(deck[i]);
        dealerHand.push_back(deck[i + 2]);
        cardStack.pop();
        cardStack.pop();
    }
    
}

void shuffle(Card deck[]) {
    std::cout<<"Shuffling deck.\n";
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(deck, deck + 52, g);
    for(int i = 0; i < 52; i++) {
        cardStack.push(deck[i]);
    }
}

int getPlayerCardValue() {
    playerCardValue = 0;
    for(int i = 0; i < playerHand.size(); i++) {
        playerCardValue += playerHand[i].getCardValue();
    }
    return playerCardValue;
}

int getDealerCardValue() {
    dealerCardValue = 0;
    for(int i = 0; i < dealerHand.size(); i++) {
        dealerCardValue += dealerHand[i].getCardValue();
    }
    return dealerCardValue;
}

void printCards(std::vector<Card> hand) {
    std::cout<<"Cards in hand: \n";
    for(int i = 0; i < hand.size(); i++) {
        std::cout<<cardToString(hand[i])<<"\n";
    }
    std::cout<<"\n";
}

void printPlayerHand(std::vector<Card> hand) {
    std::cout<<"Total cards in hand: "<<hand.size()<<"\n";
    std::cout<<"Total value of hand: "<<getPlayerCardValue()<<"\n";
    if(getPlayerCardValue() > 21) {
        std::cout<<"\nYou busted! Your hand value is over 21.\n";
        isPlayerTurn = false;
        isGameOver = false;
        dealerScore++;
    }
    std::cout<<"\n";
    printCards(hand);
}

void printDealerHand(std::vector<Card> hand) {
    std::cout<<"Total cards in hand: "<<hand.size()<<"\n";
    std::cout<<"Total value of hand: "<<getDealerCardValue()<<"\n";
    std::cout<<"\n";
}

void hit(bool isPlayer) {
    if(isPlayer) {
        if(!cardStack.empty()) {
            Card hitCard = cardStack.top();
            cardStack.pop();
            playerHand.push_back(hitCard);
            std::cout<<"You drew a: "<<cardToString(hitCard)<<"\n";
            std::cout<<"\nYour hand is now: \n";
            printPlayerHand(playerHand);
        } else {
            std::cout<<"No more cards in the deck. Exiting game.\n";
        }
    } else {
        if(!cardStack.empty()) {
            Card hitCard = cardStack.top();
            cardStack.pop();
            dealerHand.push_back(hitCard);
            std::cout<<"Dealer drew a: "<<cardToString(hitCard)<<"\n";
            std::cout<<"\nDealers hand is now: \n";
            printDealerHand(dealerHand);
            printCards(dealerHand);
        } else {
            std::cout<<"No more cards in the deck. Exiting game.\n";
        }
    }   
}

void printRules() {
    std::cout<<"----------=BlackJack Rules=----------\n";
    std::cout << "1. The goal is to beat the dealer; if your hand exceeds 21, you bust and lose.\n";
    std::cout << "2. During your turn, the dealer only reveals their top card.\n";
    std::cout << "3. If your final hand equals the dealer’s, you lose — the dealer always wins ties.\n";
    std::cout << "4. The dealer must hit on hands totaling less than 17.\n";
    std::cout << "5. Aces are worth 11; face cards (K, Q, J) are worth 10.\n";
    std::cout << "6. If your initial hand is an Ace and a 10-point card, you win automatically with Blackjack.\n";
    std::cout << "7. If the dealer's top card is an Ace, they must reveal their second card.\n";
    std::cout << "8. On your turn, you can hit (draw a card) or stand (end your turn).\n";
    std::cout << "9. You can hit until you bust (go over 21) or choose to stand with your hand.\n";
    std::cout << "10. If the dealer’s face-up card is an Ace or 10-point card, they peek for Blackjack.\n";
    std::cout << "11. Player Blackjack beats a dealer 21, but not a dealer Blackjack.\n";
    std::cout << "12. Cards are drawn from a shuffled deck without repeats.\n";
    std::cout << "13. Splitting, doubling down, and insurance are not supported in this version YET.\n";
    std::cout << "14. Dealer reveals their full hand after the player stands.\n";
    std::cout << "15. The game continues until the player chooses to stop.\n";
    std::cout << "16. After 16 games a new deck will be created and shuffled.\n";
}

void playerTurn() {
    isPlayerTurn = true;
    std::cout<<"\n----------=Players turn=----------\n";
    std::cout<<"Players hand: \n";
    printPlayerHand(playerHand);
    while(isPlayerTurn) {
        if(getPlayerCardValue() > 21) {
            std::cout<<"\nYou busted! Your hand value is over 21.\n";
            isPlayerTurn = false;
            playerLost = true;
            isGameOver = false;
            dealerScore++;
            return;
        }
        int choice = 0;
        if(getPlayerCardValue() == 21) {
            std::cout<<"Your hand value is 21, automatically standing.\n";
            isPlayerTurn = false;
        } else if(getPlayerCardValue() < 21) {
            std::cout<<"Hit or Stand? (1 for Hit, 0 for Stand)\n";
            std::cin>>choice;
        }

        switch(choice) {
            case 0:
                std::cout<<"You chose to stand.\n";
                isPlayerTurn = false;
                playerLost = false;
                break;
            case 1:
                std::cout<<"You chose to hit.\n";
                hit(isPlayerTurn);
                break;
            default:
                std::cout<<"Choice invalid, try again.\n";
                continue;
        }
    }
}

void dealerTurn() {
    isPlayerTurn = false;
    std::cout<<"\n----------=Dealers turn=----------\n";
    if(dealerHand[0].getCardValue() >= 10) {
        std::cout<<"Dealers top card is the " << cardToString(dealerHand[0]) << ". Dealer must reveal their second card.\n";
        std::cout<<"Dealers hand is now: \n";
        printDealerHand(dealerHand);
        printCards(dealerHand);
        while(getDealerCardValue() < 17) {
            hit(isPlayerTurn);
        }
        if(getDealerCardValue() == 21) {
            std::cout<<"Dealer has a hand value of 21, automatically standing.\n";
            isPlayerTurn = false;
        }
        if(getDealerCardValue() > 21) {
            std::cout<<"Dealer busted! Dealer's hand value is over 21.\n";
            std::cout<<"Your hand: \n";
            printPlayerHand(playerHand);
            std::cout<<"\nDealers hand: \n";
            printDealerHand(dealerHand);
            printCards(dealerHand);
            std::cout<<"You win!\n";
            playerScore++;
            isGameOver = true;
            return;
        }
        if(getDealerCardValue() >= 17) {
            std::cout<<"Dealers final hand: \n";
            printDealerHand(dealerHand);
        }
        std::cout<<"Dealers final hand: \n";
        printDealerHand(dealerHand);
        printCards(dealerHand);
    } else {
        std::cout<<"Dealers top card: "<<cardToString(dealerHand[0])<<"\n";
        while(getDealerCardValue() < 17) {
            hit(isPlayerTurn);
        }
        if(getDealerCardValue() > 21) {
            std::cout<<"Dealer busted! Dealer's hand value is over 21.\n";
            std::cout<<"Your hand: \n";
            printPlayerHand(playerHand);
            std::cout<<"Dealers hand: \n";
            printDealerHand(dealerHand);
            printCards(dealerHand);
            std::cout<<"You win!\n";
            playerScore++;
            isGameOver = true;
            return;
        }
        if(getDealerCardValue() >= 17) {
            std::cout<<"Dealers final hand: \n";
            printDealerHand(dealerHand);
            printCards(dealerHand);
        }
    }
}

void gameOver() {
    std::cout<<"\n----------=Game Over=----------\n";
            std::cout<<"Your hand: \n";
            printPlayerHand(playerHand);
            std::cout<<"\nDealers hand: \n";
            printDealerHand(dealerHand);
            std::cout<<"----------=+=-=+=----------\n";
            if(dealerHand[0].getCardValue() == 11 && dealerHand[1].getCardValue() == 10 && playerHand[0].getCardValue() == 11 && playerHand[1].getCardValue() == 10){
                std::cout<<"Both you and the dealer got a Blackjack! Dealer wins!\n";
                dealerScore++;
            } else if(playerHand[0].getCardValue() == 11 && playerHand[1].getCardValue() == 10) {
                std::cout<<"You got Blackjack! You win!\n";
                playerScore++;
            } else if(dealerHand[0].getCardValue() == 11 && dealerHand[1].getCardValue() == 10) {
                std::cout<<"Dealer got a Blackjack! Dealer wins.\n";
                dealerScore++;
            } else if(getPlayerCardValue() > getDealerCardValue()) {
                std::cout<<"You win! Your hand value is greater than the dealers.\n";
                playerScore++;
            } else if(getPlayerCardValue() < getDealerCardValue()) {
                std::cout<<"Dealer wins! Their hand value is greater than yours.\n";
                dealerScore++;
            } else {
                std::cout<<"Both players cards are equal! Dealer wins by default.\n";
                dealerScore++;
            }
            std::cout<<"----------=+=-=+=----------\n";
}

void emptyCardStack() {
    while(!cardStack.empty()) {
        cardStack.pop();
    }
}

void playGame() {
    bool playerChoice = true;
    Card deck[52];
    char suits[4] = {'D','H','S','C'};

    createDeck(deck, suits);
    shuffle(deck);
    while(playerChoice) {
        if(gamesPlayed % 16 == 0 && gamesPlayed != 0) {
            std::cout<<"16 games have been played creating a new deck and shuffling.\n";
            emptyCardStack();
            createDeck(deck, suits);
            shuffle(deck);
        }
        dealCards(deck);
        printRules();
        playerTurn();
        if(playerLost == false) {
            dealerTurn();
        }
        if(isGameOver == false) {
            gameOver();
        }

        // Reset game state for next round
        playerHand.clear();
        dealerHand.clear();
        playerCardValue = 0;
        dealerCardValue = 0;
        std::cout<<"cardStack size: "<<cardStack.size()<<"\n";
        std::cout<<"\n\tPlayer Score: "<<playerScore<<"\n";
        std::cout<<"\tDealer Score: "<<dealerScore<<"\n";
        std::cout<<"\nPlay another game? (1 for Yes, 0 for No)\n";
        std::cin>>playerChoice;
        // Clears terminal
        // std::system("clear");
        gamesPlayed++;
    }
    std::cout<<"\n\tFinal Scores:\n";
    std::cout<<"\tPlayer Score: "<<playerScore<<"\n";
    std::cout<<"\tDealer Score: "<<dealerScore<<"\n";
    std::cout<<"\n\tThanks for playing!\n\n";
    playerScore = 0;
    dealerScore = 0;
}


int main() {

    playGame();

}