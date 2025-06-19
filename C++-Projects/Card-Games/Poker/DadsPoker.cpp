// Author: Rob Munsell
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

int p1score=0, p2score=0, p3score=0, dscore=0;
int next_card=0; //Next card available on top of deck

struct Handscore
{
    int score;
    int high;
    int second;
    int third;
};

struct Places
        {
        int who;
        int value;
        int high;
        int second;
        int third;
        };


Handscore p1, p2, p3, dealer;

struct Card
{
int absolute_value;  //card value in 52 card deck - used to be deck value.
string suit;
int value;
};

Card deck[52];			//raw array for cards.  Use for shuffling and master value for card
Card player1_cards[5], player2_cards[5], player3_cards[5], dealer_cards[5];


bool sort_value(const Card i, const Card j)
{
		return i.value < j.value;
} //Used for sorting the cards



int Check_Dupe(int card_value, int counter)  //random number generator to shuffle grabbed a lot of dupes.  This detects dupes
{
	int verify = 0;

	while (verify < counter)
	{
		if (card_value == deck[verify].absolute_value)
		{
			return 1;
		}
		verify++;
	}
	return 0;
}

void Init_Deck()  //fill decks and hands with zeros
{
        for (int n=0; n>52; n++)
            {
                deck[n].absolute_value = 0;
                deck[n].value = 0;
            }
        for (int n=0; n>5; n++)
            {
                player1_cards[n].absolute_value = 0;
                player1_cards[n].value = 0;
                player2_cards[n].absolute_value = 0;
                player2_cards[n].value = 0;
                player3_cards[n].absolute_value = 0;
                player3_cards[n].value = 0;
                            }
}

void Create_Deck()  //build the deck
{
	int counter = 0;

	srand(time(NULL));

	while (counter < 52)
	{
		deck[counter].absolute_value = rand() % 52;
/*		cout << counter << " " << deck[counter].absolute_value << " \n";  */

		if (Check_Dupe(deck[counter].absolute_value, counter) == 0)
			counter++;
	}
	cout << "DONE \n \n";
}

string Assign_Suit(int value)    //translates the card value to a suit
{
	if (value < 13)
		return "Hearts";
	else if (value < 26)
		return "Diamonds";
	else if (value < 39)
		return "Spades";
	else
		return "Clubs";
}

int Assign_Value(int value)   //translates the card value to a card face numeric value - need to mapp display value to card face value
{
	if (value < 13)
		return value;
	else if (value < 26)
		return (value - 13);
	else if (value < 39)
		return (value - 26);
	else
		return value - 39;
}


void Map_Deck_To_Cards()   //maps original deck to mapped deck.
{
	int counter = 0;

	while (counter < 52)
	{
		deck[counter].suit = Assign_Suit(deck[counter].absolute_value);
		deck[counter].value = Assign_Value(deck[counter].absolute_value);
		counter++;
	}

}

void Show_Deck()  //displays deck
{
	int counter = 0;
	while (counter < 20)  /* should be 52 */
	{
		cout << "Card Number " << counter << " Card Value " << deck[counter].absolute_value << " Suit " << deck[counter].suit << " Card Value " << deck[counter].value << "\n";
		counter++;
	}

}

void Show_Player_Cards(int player_num)  //shows player cards after deal
{
	int counter = 0;
	while (counter < 5)
	{
		if (player_num == 0)
			cout << "D" << counter + 1 << "---" << dealer_cards[counter].value << "-" << dealer_cards[counter].suit << "\n";
		else if (player_num == 1)
			cout << "P1-" << counter + 1 << "---" << player1_cards[counter].value << "-" << player1_cards[counter].suit << "\n";
		else if (player_num == 2)
			cout << "P2-" << counter + 1 << "---" << player2_cards[counter].value << "-" << player2_cards[counter].suit << "\n";
		else if (player_num == 3)
			cout << "P3-" << counter + 1 << "---" << player3_cards[counter].value << "-" << player3_cards[counter].suit << "\n";
		counter++;
	}
}

void Show_Hands(int players)  //shows a specifc players cards
{
	int counter = 0;

	while (counter < (players + 1))
	{
		Show_Player_Cards(counter);
		cout << "\n";
		counter++;
	}
}


void Deal_Cards(int players)  //used to deal cards based on number of players
{
	int counter = 0;
	int pcount = 0;

	if (players == 1)
		while (counter < 10)
		{
			player1_cards[pcount] = deck[counter];
			counter++;
			dealer_cards[pcount] = deck[counter];
			counter++;
			pcount++;
			next_card = 10;
		}
	else if (players == 2)
		while (counter < 15)
		{
			player1_cards[pcount] = deck[counter];
			counter++;
			player2_cards[pcount] = deck[counter];
			counter++;
			dealer_cards[pcount] = deck[counter];
			counter++;
			pcount++;
			next_card = 15;
		}
	else if (players == 3)
		while (counter < 20)
		{
			player1_cards[pcount] = deck[counter];
			counter++;
			player2_cards[pcount] = deck[counter];
			counter++;
			player3_cards[pcount] = deck[counter];
			counter++;
			dealer_cards[pcount] = deck[counter];
			counter++;
			pcount++;
			next_card = 20;
			}
}

void Sort_Hands() //Need Better way to do sort - not brute force
{
    int counter, pcounter=0;

    //Dealer

    while (pcounter < 4 )
        {
        counter = pcounter +1;
        while (counter < 5)
            {
            if (dealer_cards[pcounter].value > dealer_cards[counter].value)   //if card value 1 is greater than card value 2, swap them.
                {
                swap (dealer_cards[pcounter], dealer_cards[counter]);
                counter++;
                }
            else
                counter++;
            }
        pcounter++;
        }

    //Player 1

    pcounter=0;
    while (pcounter < 4 )
        {
        counter = pcounter +1;
        while (counter < 5)
            {
            if (player1_cards[pcounter].value > player1_cards[counter].value)   //if card value 1 is greater than card value 2, swap them.
                {
                swap (player1_cards[pcounter], player1_cards[counter]);
                counter++;
                }
            else
                counter++;
            }
        pcounter++;
        }

    //Player 2

    pcounter=0;
    while (pcounter < 4 )
        {
        counter = pcounter +1;
        while (counter < 5)
            {
            if (player2_cards[pcounter].value > player2_cards[counter].value)   //if card value 1 is greater than card value 2, swap them.
                {
                swap (player2_cards[pcounter], player2_cards[counter]);
                counter++;
                }
            else
                counter++;
            }
        pcounter++;
        }

    //Player 3

    pcounter=0;
    while (pcounter < 4 )
        {
        counter = pcounter +1;
        while (counter < 5)
            {
            if (player3_cards[pcounter].value > player3_cards[counter].value)   //if card value 1 is greater than card value 2, swap them.
                {
                swap (player3_cards[pcounter], player3_cards[counter]);
                counter++;
                }
            else
                counter++;
            }
        pcounter++;
        }
}



bool Is_Flush(Card hand[5])
{
    if ( (hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit) && (hand[3].suit == hand[4].suit) )
        return true;
    else
        return false;
}

bool Is_Straight(Card hand[5])
{
     if ( (hand[0].value == hand[1].value -1) && (hand[2].value == hand[0].value -2) && (hand[3].value == hand[0].value-3) && (hand[4].value == hand[0].value-4) )
        return true;
    else
        return false;
}

bool Is_Straight_Flush(Card hand[5])
{
    if ( (Is_Flush(hand)) && (Is_Straight(hand)) && (hand[4].value!=12) )
            return true;
        else
            return false;
}

bool Is_Royal_Flush(Card hand[5])
{
    if (Is_Flush(hand) && Is_Straight(hand) && hand[4].value==12)
        return true;
    else
        return false;
}

bool Is_FourofKind(Card hand[5])
{
 if ( (hand[0].value == hand[3].value) || (hand[1].value == hand[4].value) )
        return true;
    else
        return false;
}

bool Is_ThreeofKind(Card hand[5])  //if first and third cards match, but it isn't 4 of a kind
{
if ( ( (hand[0].value == hand[2].value) && !Is_FourofKind(hand)) || ( (hand[1].value == hand[3].value) && !Is_FourofKind(hand)) || ( (hand[2].value == hand[4].value) && !Is_FourofKind(hand)))
        return true;
    else
        return false;
}

bool Is_Pair(Card hand[5])
{
    int counter=0;

    do {
        if ((hand[counter].value == hand[counter +1].value) && !Is_FourofKind(hand) )
            return true;
        else
            counter++;
     } while (counter <5);
     return false;
}


bool Is_Fullhouse(Card hand[5])
{

  if (Is_ThreeofKind(hand) && !Is_FourofKind(hand))
        if ( (hand[0].value == hand[2].value) && (hand[3].value == hand[4].value))
            return true;
        else
            if ( (hand[2].value == hand[4].value) && (hand[0].value == hand[1].value))
                return true;
            else
                return false;
  else
    return false;


}

bool Is_TwoPairs(Card hand[5])
{
    int counter=0, pairs=0;

    do {
        if ((hand[counter].value == hand[counter +1].value) && !Is_FourofKind(hand) && !Is_ThreeofKind(hand) )
            {
            pairs++;
            counter++;
            }
        else
            counter++;
        } while (counter <5);

     if (pairs > 1)
        return true;
     else
        return false;
}


int Score_Me(Card hand[5])
{
    int score =0;

    if (Is_Royal_Flush(hand) == true)
        score = 30;
    else if (Is_Straight_Flush(hand) == true)
        score = 29;
    else if (Is_FourofKind(hand) == true)
        score = 28;
    else if (Is_Fullhouse(hand) == true)
        score = 27;
    else if (Is_Flush(hand) == true)
        score = 26;
    else if (Is_Straight(hand) == true)
        score = 25;
    else if (Is_ThreeofKind(hand) == true)
        score = 24;
    else if (Is_TwoPairs(hand) == true)
        score = 23;
    else if (Is_Pair(hand) == true)
        score = 22;
    else
        score = hand[4].value;

    return score;
}


void Score_Hands(int players)
{
    dealer.score = Score_Me(dealer_cards);
    p1.score = Score_Me(player1_cards);
    p2.score = Score_Me(player2_cards);
    p3.score = Score_Me(player3_cards);

    if (players == 1)
        {
        p2.score = 0;
        p3.score = 0;
        }
    if (players == 2)
        p3.score =0;

    cout <<"\n Dealer score "<< dealer.score;
    cout <<"\n P1 score "<< p1.score;
    cout <<"\n P2 score "<< p2.score;
    cout <<"\n P3 score "<< p3.score;


}

string Winning_Hand(int score)
{
    string hand;

    switch (score) {
    case 30:
        hand = "Royal Flush";
        return hand;
    case 29:
        hand = "Straight Flush";
        return hand;
    case 28:
        hand = "Four of a Kind";
        return hand;
    case 27:
        hand = "Full House";
        return hand;
    case 26:
        hand = "Flush";
        return hand;
    case 25:
        hand = "Straight";
        return hand;
    case 24:
        hand = "Three of a Kind";
        return hand;
    case 23:
        hand = "Two Pairs";
        return hand;
    case 22:
        hand = "One Pair";
        return hand;
    default:
        hand = "High Card";
        return hand;
    }
}

int Number_Of_Ties()
    {
    int ties = 0;

    if (dealer.score == p1.score)
        ties++;
    if (dealer.score == p2.score)
        ties++;
    if (dealer.score == p3.score)
        ties++;
    if (p1.score == p2.score)
        ties++;
    if (p1.score == p3.score)
        ties++;
    if (p2.score == p3.score)
        ties++;
    if (ties == 0)
        cout << "\n ERROR WITH TIEBREAKER - NO TIES \n";
    return ties;
    // 1 tie = equals 1 pair
    // 3 tie = 3 ties = 1+2 = 1+3 = 2+3
    // 6 tie = 4 way tie

}

Handscore Score_High_Card(Card hand[5], Handscore scorecard)
{
    if (Is_Royal_Flush(hand) == true)
        {
            scorecard.score = 30;
            scorecard.high = hand[4].value;
            scorecard.second = hand[3].value;
            scorecard.third = hand[2].value;
            return scorecard;
        }
    else if (Is_Straight_Flush(hand) == true)
        {
            scorecard.score = 29;
            scorecard.high = hand[4].value;
            scorecard.second = hand[3].value;
            scorecard.third = hand[2].value;
            return scorecard;
        }
    else if (Is_FourofKind(hand) == true)
        {
            scorecard.score = 28;
            if (hand[0].value == hand[1].value)
                {
                scorecard.high = hand[3].value;
                scorecard.second = hand[4].value;
                scorecard.third = 0;
                return scorecard;
                }
            else
                {
                scorecard.high = hand[4].value;
                scorecard.second = hand[0].value;
                scorecard.third = 0;
                return scorecard;
                }
        }
    else if (Is_Fullhouse(hand) == true)
        {
            scorecard.score = 27;
            if (hand[0].value == hand[2].value)
                {
                scorecard.high = hand[2].value;
                scorecard.second = hand[4].value;
                scorecard.third = 0;
                return scorecard;
                }
        else
                {
                scorecard.high = hand[4].value;
                scorecard.second = hand[1].value;
                scorecard.third = 0;
                return scorecard;
                }
        }
    else if (Is_Flush(hand) == true)
        {
            scorecard.score = 26;
            scorecard.high = hand[4].value;
            scorecard.second = hand[3].value;
            scorecard.third = hand[2].value;
            return scorecard;
        }

    else if (Is_Straight(hand) == true)
        {
            scorecard.score = 25;
            scorecard.high = hand[4].value;
            scorecard.second = hand[3].value;
            scorecard.third = hand[2].value;
            return scorecard;
        }

    else if (Is_ThreeofKind(hand) == true)
        {
        scorecard.score = 24;
        if (hand[0].value == hand[2].value)
           {
           scorecard.high = hand[2].value;
           scorecard.second = hand[4].value;
           scorecard.third = hand[3].value;
           return scorecard;
           }
        else
           if (hand[1].value == hand[3].value)
              {
              scorecard.high = hand[3].value;
              scorecard.second = hand[4].value;
              scorecard.third = hand[1].value;
              return scorecard;
              }
        else
           if (hand[2].value == hand[4].value)
              {
              scorecard.high = hand[4].value;
              scorecard.second = hand[1].value;
              scorecard.third = hand[0].value;
              return scorecard;
              }
        }

    else if (Is_TwoPairs(hand) == true)
        {
        scorecard.score = 23;
        if (hand[4].value == hand[3].value)
             scorecard.high = hand[4].value;
        else
           if (hand[3].value == hand[2].value)
             scorecard.high = hand[3].value;

        if (hand[0].value == hand[1].value)
             scorecard.second = hand[1].value;
        else
            if (hand[1].value == hand[2].value)
             scorecard.second = hand[2].value;

        if ( (hand[4].value == hand[3].value) && (hand[1].value == hand[2].value))
             scorecard.third = hand[0].value;
        else
           if ( (hand[4].value == hand[3].value) && (hand[0].value == hand[1].value))
             scorecard.third = hand[2].value;
        else
           if ( (hand[2].value == hand[3].value) && (hand[0].value == hand[1].value))
             scorecard.third = hand[4].value;
        return scorecard;
        }

    else if (Is_Pair(hand) == true)
        {
        scorecard.score = 22;
        if (hand[4].value == hand[3].value)
             {
             scorecard.high = hand[4].value;
             scorecard.second = hand[2].value;
             scorecard.third = hand[1].value;
             return scorecard;
             }
        else
           if (hand[3].value == hand[2].value)
             {
             scorecard.high = hand[3].value;
             scorecard.second = hand[4].value;
             scorecard.third = hand[1].value;
             return scorecard;
             }
        else
           if (hand[2].value == hand[1].value)
             {
             scorecard.high = hand[2].value;
             scorecard.second = hand[4].value;
             scorecard.third = hand[3].value;
             return scorecard;
             }
        else
           if (hand[1].value == hand[0].value)
             {
             scorecard.high = hand[1].value;
             scorecard.second = hand[4].value;
             scorecard.third = hand[3].value;
             return scorecard;
             }
        }
    else
        {
        scorecard.high = hand[4].value;
        scorecard.second = hand[3].value;
        scorecard.third = hand[2].value;
        return scorecard;
        }
    return scorecard;
}

int Compare_Hands (Places hand1, Places hand2)
    {
    if ( hand1.high > hand2.high)
       return 0;
    else
       if (hand2.high > hand1.high)
            return 1;
    else
       if (hand1.second > hand2.second)
            return 0;
    else
       if (hand2.second > hand1.second)
            return 1;
    else
       if (hand1.third > hand2.third)
            return 0;
    else
       if (hand2.third > hand1.third)
            return 1;
    else
       return 5;          //if they tie
    }


int Tie_Winner(int ties, Places thands[4])  //this function receives the sorted by score hands

    {
        int winner = 0;
        int winner2 = 0;
        int winner3 = 0;

        if (ties == 1 || ties == 2)  //Single tie
            {
                winner = Compare_Hands(thands[0], thands[1]);
                if (winner == 5)
                    return winner;
                 else
                    return thands[winner].who;
            }

        if (ties == 3) // 3 hands are the same
            {
            winner = Compare_Hands(thands[0], thands[1]);
            winner2 = Compare_Hands(thands[1], thands[2]);

            if (winner == 5 && winner2 == 5)
                return 5;
             else
                if (winner == 5)
                    {
                    winner3 = Compare_Hands(thands[0], thands[winner2]);
                    if (winner3 == 5)
                        return winner3;
                    else
                        if (winner == 0)
                            return thands[0].who;
                        else
                            return thands[winner2].who;
                    }
             else
                if (winner2 == 5)
                    {
                    winner3 = Compare_Hands(thands[winner], thands[2]);
                    if (winner3 == 5)
                        return winner3;
                    else
                        if (winner == 0)
                            return thands[winner].who;
                        else
                            return thands[2].who;
                    }
             else
                {
                winner2 = Compare_Hands(thands[0], thands[2]);
                winner3 = Compare_Hands(thands[1], thands[2]);
                if (winner2 == 5)
                    return winner3;
                 else
                   if (winner == 0 && winner2 == 0)
                        return thands[0].who;
                 else
                   if (winner2 == 1 && winner3 == 1)
                        return thands[2].who;
                 else
                   if (winner == 1 && winner3 ==0)
                        return thands[1].who;
                 }
            }

        if (ties == 6) // All 4 hands are the same  ---- Document me
            {
            winner = Compare_Hands(thands[0], thands[1]);
            winner2 = Compare_Hands(thands[2], thands[3]);

            if (winner == 5 && winner2 == 5)
                {
                winner3 = Compare_Hands(thands[0], thands[3]);
                if (winner3 == 0)
                    return 0;
                 else
                    if (winner3 == 1)
                        return thands[3].who;
                 else
                    return winner3;
                }

            if (winner == 5 && winner2 != 5)  // comparing 2 and 3 to identical pair of 0 and 1
                {
                if (winner2 == 0)  // comparing 3 to 1 and 2
                    {
                    winner3 = Compare_Hands(thands[0], thands[2]);
                    if (winner3 == 0)
                        return thands[0].who;
                     else
                        if (winner3 == 1)
                            return thands[2].who;
                     else        // looking where 1 and 2 could be same as 3
                        return 5;
                    }
                 else
                    if (winner2 == 1) // comparing 4 to 1 and 2
                    {
                    winner3 = Compare_Hands(thands[0], thands[3]);
                    if (winner3 == 0)
                        return thands[0].who;
                     else
                        if (winner3 == 1)
                            return thands[3].who;
                     else   // looking where 1 and 2 could be same as 4
                        return 5;
                    }
                }

            if (winner != 5 && winner2 == 5)  // comparing 0 and 1 to identical pair of 2+3
                 {
                if (winner == 0)  // comparing 0 to 2 and 3
                    {
                    winner3 = Compare_Hands(thands[0], thands[2]);
                    if (winner3 == 0)
                        return thands[0].who;
                     else
                        if (winner3 == 1)
                            return thands[2].who;
                     else        // looking where 0 could be same as 2 and 3
                        return 5;
                    }
                 else
                    if (winner == 1) // comparing 1 to 2 and 3
                    {
                    winner3 = Compare_Hands(thands[0], thands[3]);
                    if (winner3 == 0)
                        return thands[0].who;
                     else
                        if (winner3 == 1)
                            return thands[3].who;
                     else   // looking where 1 could be same as 2 and 3
                        return 5;
                    }
                }

             if (winner != 5 && winner2 != 5)
                {
                winner3 = Compare_Hands(thands[winner], thands[winner2]);
                if (winner3 == 5) // if tie
                        return winner3;
                    else
                        if (winner3 == 0)  //front pair is bigger
                            {
                            if (winner == 0)  //if first one of front pair is biggest
                               return thands[0].who;
                             else
                                return thands[1].who;
                            }
                    else
                        if (winner3 == 1) //if back pair is bigger
                            {
                            if (winner2 == 0) //if first of back pair is biggest
                               return thands[0].who;
                             else
                                return thands[1].who;
                            }
                }
            }
        return 5;
    }

int Tie_Breaker()
{
    string descrip;

    int ties = 0;
    int winner = 0;
    Places who_tied[4];

    //populating array to see who tied
    who_tied[0].value=dealer.score;
    who_tied[1].value=p1.score;
    who_tied[2].value=p2.score;
    who_tied[3].value=p3.score;
    who_tied[0].who=0;
    who_tied[1].who=1;
    who_tied[2].who=2;
    who_tied[3].who=3;
    who_tied[0].high=dealer.high;
    who_tied[1].high=p1.high;
    who_tied[2].high=p2.high;
    who_tied[3].high=p3.high;
    who_tied[0].second=dealer.second;
    who_tied[1].second=p1.second;
    who_tied[2].second=p2.second;
    who_tied[3].second=p3.second;
    who_tied[0].third=dealer.third;
    who_tied[1].third=p1.third;
    who_tied[2].third=p2.third;
    who_tied[3].third=p3.third;



    int counter=0;
    int pcounter=1;
    while (counter < 3) //sorting hands by score
    {
        while (pcounter < 4)
        {

        if (who_tied[counter].value < who_tied[pcounter].value)
           {
            swap(who_tied[counter].value,who_tied[pcounter].value);
            swap(who_tied[counter].who,who_tied[pcounter].who);
            swap(who_tied[counter].high,who_tied[pcounter].high);
            swap(who_tied[counter].second,who_tied[pcounter].second);
            swap(who_tied[counter].third,who_tied[pcounter].third);

           }
        else
            pcounter++;
        }
        counter++;
        pcounter=counter+1;

    }

    ties = Number_Of_Ties();

    winner = Tie_Winner(ties, who_tied);
    return winner;

}

void Find_Winner()
{
  string descrip;
  int winner = 0;

  dealer = Score_High_Card (dealer_cards, dealer);
  p1 = Score_High_Card(player1_cards, p1);
  p2 = Score_High_Card(player2_cards, p2);
  p3 = Score_High_Card(player3_cards, p3);


  if ( (dealer.score > p1.score) && (dealer.score > p2.score) && (dealer.score > p3.score))
        cout << "\n \n Dealer Wins with " << (descrip = Winning_Hand(dealer.score));
    else
        if ( (p1.score > dealer.score) && (p1.score > p2.score) && (p1.score > p3.score))
        cout << "\n \n Player 1 Wins with " << (descrip = Winning_Hand(p1.score));
    else
        if ( (p2.score > dealer.score) && (p2.score > p1.score) && (p2.score > p3.score))
        cout << "\n \n Player 2 Wins with " << (descrip = Winning_Hand(p2.score));
    else
        if ( (p3.score > dealer.score) && (p3.score > p1.score) && (p3.score > p1.score))
        cout << "\n \n Player 3 Wins with " << (descrip = Winning_Hand(p3.score));
    else
        {
        winner = Tie_Breaker();
        switch (winner)
            {
            case 0: cout << "\n \n Dealer Wins with " << (descrip = Winning_Hand(dealer.score)); break;
            case 1: cout << "\n \n Player 1 Wins with " << (descrip = Winning_Hand(p1.score)); break;
            case 2: cout << "\n \n Player 2 Wins with " << (descrip = Winning_Hand(p2.score)); break;
            case 3: cout << "\n \n Player 3 Wins with " << (descrip = Winning_Hand(p3.score)); break;
            case 5: cout << "\n \n Tie " << (descrip = Winning_Hand(p3.score));
            }
        }
}

int main()
{
	int players = 3;

	Create_Deck();
	Map_Deck_To_Cards();

/*
	Show_Deck();
*/

	Deal_Cards(players);
	Sort_Hands();
	Show_Hands(players);

    Score_Hands(players);
    Find_Winner();


	/* to do

	Swap cards
		Select cards to swap
		Deal new cards
	GUI

	*/

	return 0;

}


