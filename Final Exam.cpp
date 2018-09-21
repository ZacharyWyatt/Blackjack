#include <iostream>								//Zachary Wyatt
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

class deck
{
	public:
		string suit;	
		string face;	
		int value;
		int location; //0 for in the deck, 1 for in the dealer's hand, 2 for in the player's hand, 3 for discard
};
void Testing(deck array[], int&);					//For debug purposes
void CreateDeck(deck array[]);						//Creates the deck
void Shuffle(deck array [], int);					//Shuffles the deck
void Gameplay(deck array[], int, bool);				//Flow of gameplay is controled here
void DealCard(deck array[], string, int&);			//Deals a card to the player who's passed to it
int TotalHand(deck array[], string, int);			//Returns the total value of the player's hand who's passed
void OutputInfo(deck array[], string, int, bool);	//Uses GetLocation() to display the cards of whichever player is passed, controls facedown cards	
void GetLocation(deck array[], int, int);			//Displays all cards of whichever location is passed to it
int ScoreCheck(deck array[], string, int);			//Controls aspects related to scores
int GameOverCheck(deck array[], int, int);			//Checks for game-over scenarios
void GameOver(deck array [], int, int, int, int&, int&);//Displays game-over scenarios
void Discard(deck array[], int deckCtr);			//Discards the current hands

int main ()	
{	
	bool debug = 0;
	int deckCtr = 0;
	deck card[52];
	
	CreateDeck(card);
	
	cout<<"Enable debug mode? Type 1 for YES; type 0 for NO.\n";
	cin>>debug;
	if (debug == 1) Testing(card, deckCtr);
	else system("CLS");
	
	Shuffle(card, deckCtr);
	Gameplay(card, deckCtr, debug);
return 0;
}

void Discard(deck card[], int deckCtr)
{
	for (int ctr = 0; ctr<52; ctr++) 
	{
		if ((card[ctr].location == 1) || (card[ctr].location == 2)) card[ctr].location = 3;
	}
}

int GameOverCheck(deck card[], int playerScore, int dealerScore)
{
	/*If the player busts, or nobody busts and the dealer's score is higher, return 1*/
	if  ((playerScore>21) || ((dealerScore>playerScore) && (dealerScore<=21))) return 1;

	/*If the dealer busts, or nobody busts and the player's score is higher, return 0*/
	else if ((dealerScore>21) || ((playerScore>dealerScore) && (dealerScore<=21))) return 0;
	
	/*If a tie, return 2*/
	else if (dealerScore == playerScore) return 2;

	else return 3;	/*This means nobody has won yet*/
}

void GameOver(deck card[], int scenario, int playerScore, int dealerScore, int& playerWins, int& gamesPlayed)
{
	system("cls");
	cout<<"Game over!\n";
	
	/*Using the return values from GameOverCheck()*/
	if (scenario == 0) 
	{
		playerWins++;
		cout<<"The Player wins";
		if(playerScore == 21) cout<<" with a Blackjack";
		cout<<"!\n\n";
	}
	else if (scenario == 1)
	{
		cout<<"The Dealer wins";
		if(dealerScore == 21) cout<<" with a Blackjack";
		cout<<".\n\n";
	}
	else if (scenario == 2) cout<<"There was a tie, so the Dealer wins.\n\n";
	
	gamesPlayed++;
	
	if(dealerScore>21) cout<<"The Dealer busted!\n\n";
	else if(playerScore>21) cout<<"The Player busted.\n\n";
	
	OutputInfo(card, "DealerFinal",0 ,0); 
	cout<<"Score: "<<dealerScore<<"\n";
	OutputInfo(card, "Player",0 , 0); 
	cout<<"Score: "<<playerScore<<"\n\n"
		<<"You have won "<<playerWins<<" games and lost "<<(gamesPlayed - playerWins)<<" games, out of a total of "<<gamesPlayed<<" games.\n";
}

int ScoreCheck(deck card[], string whichPlayer, int deckCtr)
{
	int playerScore = 0, dealerScore = 0;
	 
	for (int ctr = 0; ctr<52; ctr++) 
	{
		if ((card[ctr].location == 2) && (whichPlayer == "Player"))
		{
			playerScore = playerScore + card[ctr].value;
			
		/*If you have an Ace, and its value of 11 would have caused a game over, playerScore treats the Ace as worth 1*/
			if ((playerScore>21) && (card[ctr].face == "Ace")) playerScore = playerScore - 10;						
		}
		else if ((card[ctr].location == 1) && (whichPlayer == "Dealer"))
		{
			dealerScore = dealerScore + card[ctr].value;
			
			if ((dealerScore>10) && (card[ctr].face == "Ace")) dealerScore = dealerScore - 10;
		}
	}
		if (whichPlayer == "Player") return playerScore;
		else if (whichPlayer == "Dealer") return dealerScore;
}

void GetLocation(deck card[], int location, int faceDown)
{	/*faceDown is set up so that a value of 0 will display the Facedown Card, 
	  1 will display the dealer's second card,and 2 will display its final hand*/
	  
	for (int ctr = 0; ctr<52; ctr++)	/*Displays cards that are in whatever location that's passed*/
	{									
		if ((location == 1) && (card[ctr].location == 1) && (faceDown == 0))
		{
			cout<<"  Facedown Card \n";	/*do not re-output these until time to change them, else which card is facedown will swap occasionally*/
			faceDown = 1;
		} 
		else if ((location == 1) && (card[ctr].location == 1) && (faceDown == 1))
		{
			cout<<"  "<< card[ctr].face<<" of "<< card[ctr].suit<< "\n";
			cout<<"Score: "<<card[ctr].value<<"+\n_________________________________________________";
		}
		else if (card[ctr].location == location) cout<<"  "<< card[ctr].face<<" of "<< card[ctr].suit<< "\n";
	}
}

void OutputInfo(deck card[],string whichPlayer, int deckCtr, bool debug)
{
	if (whichPlayer == "Dealer")	/*Display the dealer's hand, with a facedown card*/
	{
		cout<<"\nDealer's hand:";
		if (debug == 1) cout<<"\t deck counter: "<<deckCtr;
		cout<<"\n";
		GetLocation(card, 1, 0);
	}
	else if (whichPlayer =="Player")	/*Display the player's hand*/
	{
		cout<<"\n\nPlayer's Hand:";
		if (debug == 1) cout<<"\t deck counter: "<<deckCtr;
		cout<<"\n";
		GetLocation(card, 2, 0);	
	}
	else if (whichPlayer == "DealerFinal")	/*Display the Dealer's hand, with the facedown card now faceup*/
	{
		cout<<"Dealer's hand:";
		if (debug == 1) cout<<"\t deck counter: "<<deckCtr;
		cout<<"\n";
		GetLocation(card, 1, 2);
	}
} 

void DealCard(deck card[], string whichPlayer, int& deckCtr)
{
	if (whichPlayer == "Dealer") /*Changes the card at the "top of the deck"'s location*/
	{
		card[deckCtr].location = 1;	
		cout<<"The Dealer deals itself a card.\n\n";
	}
	else if (whichPlayer =="Player")
	{
		card[deckCtr].location = 2;	
		cout<<"The Player is dealt a card.\n\n";
	}
	deckCtr++;
	
	if (deckCtr == 52)	/*If we run out of cards, shuffle them*/
	{	
		Shuffle(card, deckCtr);
		deckCtr = 0;
	}
}

int TotalHand(deck card[], string whichPlayer, int deckCtr)
{
	int total = 0;
	
	for(int ctr = 0; ctr<52; ctr++)
	{
		if(whichPlayer == "Dealer")
		{
			if(card[ctr].location = 1)
			{
				total = total + card[ctr].value;
			}
		}
		else if (whichPlayer == "Player")
		{
			if(card[ctr].location = 2)
			{
				total = total + card[ctr].value;
			}
		}
	}
	return total;
}

void Gameplay(deck card[], int deckCtr, bool debug)
{
	int playerScore = 0, dealerScore = 0, playerWins = 0, gamesPlayed = 0;	
	int scenario = 3;		/*Possible game-over situations*/
	int faceDown = 0;		/*controls the dealer's facedown card*/
	char userInput = ' ';
	
	do{
	/*Deals the initial hand*/	
		DealCard(card, "Dealer", deckCtr);
		DealCard(card, "Dealer", deckCtr);
		DealCard(card, "Player", deckCtr);
		DealCard(card, "Player", deckCtr);
	/*Displays the initial hand*/
		OutputInfo(card, "Dealer", deckCtr, debug);
		dealerScore = ScoreCheck(card, "Dealer", deckCtr);
	
		OutputInfo(card, "Player", deckCtr, debug);
		playerScore = ScoreCheck(card, "Player", deckCtr);
		cout<<"Score: "<<playerScore<<"\n";
		
		do	/*Gives the player cards until they say no*/
		{
			cout<<"\nWould you like a hit? (Y/N)\n> ";
			cin>>userInput;
			
			if (userInput == 'y')
			{
				DealCard(card, "Player", deckCtr);
				OutputInfo(card, "Player", deckCtr, debug); 
			}
		  /*Updates Score as cards are added*/
			playerScore = ScoreCheck(card, "Player", deckCtr);
			cout<<"Score: "<<playerScore<<"\n\n";
			
		  /*Checks if the player has busted*/
			if (playerScore > 21)
			{
				userInput ='n';
			}
		} while (userInput == 'y');
		
		if (playerScore<=21)
		{
			do	/*Gives the dealer cards until their score is greater than 16, stopping if it goes above 21*/
			{
				DealCard(card, "Dealer", deckCtr);
				dealerScore = ScoreCheck(card, "Dealer", deckCtr);
			} while ((dealerScore<16) && (dealerScore<playerScore)); 
		}
	
		OutputInfo(card, "DealerFinal", deckCtr, debug);
		dealerScore = ScoreCheck(card, "Dealer", deckCtr);
		cout<<"Score: "<<dealerScore<<"\n";
		
		scenario = GameOverCheck(card, playerScore, dealerScore);
		if (debug ==1)	cout<<"\nScenario: "<<scenario<<"\n";
		
		if (dealerScore>21) cout<<"The Dealer has busted.";
		else cout<<"The Dealer has finished.";
		cout<<" Press any key to continue.\n";
		cin>>userInput;
		
		if (scenario !=3)
		{
			GameOver(card, scenario, playerScore, dealerScore, playerWins, gamesPlayed);	
		}
		Discard(card, deckCtr);
		
		cout<<"\nPlay again? (Y/N)\n> ";
		cin>>userInput;
	} while (userInput == 'y');
}

void Shuffle(deck card[], int deckCtr)
{
	int firstCard = 0;
	int secondCard = 0;
	deck tempCard;
	
	srand (time(NULL));
	for(int ctr = 0; ctr<=(rand() % 700 + 300); ctr++)	/*loops a random number of times*/
	{
		firstCard = rand() % 52; 	/*Selects a card at random*/
		secondCard = rand() % 52;	/*selects another random card*/
		
		if (((card[firstCard].location ==3) && (card[secondCard].location ==3)) || ((card[firstCard].location ==0) && (card[secondCard].location ==0)))
		{
			tempCard = card[firstCard];			/*tempCard is a copy of card 1*/
			card[firstCard] = card[secondCard];	/*card 1 is a copy of card 2*/
			card[secondCard] = tempCard;		/*Second card copies tempCard, becoming the original card*/
		}
	}
	for(int ctr = 0; ctr<52; ctr++)	/*Updates all the locations, in case the random loop missed anything*/
	{
		if(card[ctr].location ==3) card[ctr].location = 0;
	}
	cout<<"The deck has been shuffled.\n\n";
}

void CreateDeck(deck card[])
{
	int ctr = 0;
	for(int ctr2 = 1; ctr2<=13; ctr2++)
	{
		for(int ctr3 = 1; ctr3<=4; ctr3++)		
		{
		/*Suits*/
			if (ctr3==1) card[ctr].suit = "Clubs";
			else if (ctr3==2) card[ctr].suit = "Hearts";	
			else if (ctr3==3) card[ctr].suit = "Diamonds";
			else card[ctr].suit = "Spades";			
		/*Values and faces*/		
			if (ctr2==1)
			{
			card[ctr].value =  11;
			card[ctr].face =  "Ace";				
			}
			else if (ctr2==2)
			{
			card[ctr].value =  2;
			card[ctr].face =  "Two";				
			}
			else if (ctr2==3)
			{
			card[ctr].value =  3;
			card[ctr].face =  "Three";				
			}
			else if (ctr2==4)
			{
			card[ctr].value =  4;
			card[ctr].face =  "Four";				
			}
			else if (ctr2==5)
			{
			card[ctr].value =  5;
			card[ctr].face =  "Five";				
			}
			else if (ctr2==6)
			{
			card[ctr].value =  6;
			card[ctr].face =  "Six";				
			}
			else if (ctr2==7)
			{
			card[ctr].value =  7;
			card[ctr].face =  "Seven";				
			}
			else if (ctr2==8)
			{
			card[ctr].value =  8;
			card[ctr].face =  "Eight";				
			}
			else if (ctr2==9)
			{
			card[ctr].value =  9;
			card[ctr].face =  "Nine";				
			}
			else if (ctr2==10)
			{
			card[ctr].value =  10;
			card[ctr].face =  "Ten";				
			}
			else if (ctr2==11)
			{
			card[ctr].value =  10;
			card[ctr].face =  "Jack";				
			}
			else if (ctr2==12)
			{
			card[ctr].value =  10;
			card[ctr].face =  "Queen";				
			}
			else
			{
			card[ctr].value =  10;
			card[ctr].face =  "King";				
			}
		/*Location*/
			card[ctr].location = 0;
			ctr++;
		}
	}
}

void Testing(deck card[], int& deckCtr)
{
	int userInput = 0;
	deckCtr = 0;
	
	cout<<"1) View a list of cards in the deck \n2) Shuffle \n3) Set the deck counter \n4) Continue to game\n";
	do{
		cin>>userInput;
		
		if(userInput == 1)
		{	
			for(int ctr=0; ctr<52; ctr++)
			{
				cout<< (ctr+1) <<")  "<< card[ctr].face<<" of "<< card[ctr].suit<<"\tValue: "<< card[ctr].value << "  \tin ";
				
				if (card[ctr].location ==0) cout<< "the deck\n";
				else if(card[ctr].location ==1) cout<< "dealer's hand'\n";
				else if(card[ctr].location ==2) cout<< "player's hand'\n";
				else cout<< "discard\n";
			}
		}
		else if(userInput == 2) Shuffle(card, deckCtr);
		
		else if (userInput ==3)
		{
			cout<<"Enter the desired number, from 1 to 52.\n> ";
			cin>>deckCtr;
		}
	} while (userInput <4);	
	system("CLS");
}

//----------ASSIGNMENT----------------------------------------------
/*	Although there are numerous ways to write a program like this, several of which are more
	efficient than this, the objective here is for the student to utilize a number of the concepts and
	constructs we have learned in class thus far this semester. 

	Criteria
	
Done	1) Define a CARD class with all the necessary attributes of cards. 
Done	2) Create a deck (array) of cards for all 52 cards. 
Done	3) Populate the deck( ) with all 52 cards (suit and card) using a loop. 
Done	4) No card from the deck should be dealt twice. 
Done	5) When all of the cards from the deck have been dealt your
		   program should shuffle all of the discards and continue dealing	
Done	6) Maintain the current dealer’s and player’s points separately
		   and display the player’s scores after both the dealer and player
		   received their first two cards and after each subsequent “hit”. 
Done	7) At the conclusion of each hand the player and dealer’s total
		   points for that hand should be output for the user to see. 
Done	8) Your program should keep track of and output the number of
		   wins for the player/dealer as well as the total number of games
		   played and output this information at the conclusion of each hand.
Done	9) At the conclusion of each hand the player should be given the
		   option to play another hand.
Done	10) There should be liberal use of FUNCTIONS throughout your
			program in an effort to make your program flow and logic
			easy to follow. 
Done	11) There should be no GLOBAL variables. 
Done	12) Naming, indentation, whitespace, commenting, and output
			conventions learned this semester should be followed. 
Ehhh	13) Your entire program should be fewer than 350 lines of code. 
Done	14) Your program should abide by all the rules outlined in the
			attached Basic Rules of Black Jack. 
	
	Basic Rules of Blackjack
	
		The ultimate goal in Blackjack is to attain a score as close as possible to 21 without going over.
		
		The cards are valued by:
			• The number on cards 2 through 10 are as printed on the face of the card.
			• All face-cards are valued as 10 points.
			• Aces can be either 1 or 11 whichever is best for the player/dealer.
			
		The suits printed on the face of the cards have no value. So a hand containing 2, 6, and 9 would
		equal 17 points. A hand containing an Ace and a six then could be either 17 or seven points.
		To begin play, the dealer and the player are dealt two cards each. The dealer’s first card is dealt
		face-down and all subsequent dealer card(s) are dealt face-up. All player cards should be dealt
		face-up for our purposes (in reality it does not matter, face-up or face-down, as the decisions by
		the player and the dealer are not dependent on one another).
		
		Once the two initial cards have been dealt to both the player and the dealer, the dealer then asks
		the player if (s)he wants another card. The player tallies their score and decides whether accepting
		another card will better their hand. If the player elects to take another card they say "hit me" or
		“hit”. If the player elects to stay at their current score they say "stand". The player can take as
		many cards as (s)he wishes until they are satisfied with their score or their score exceeds 21.
		Should the score in their hand go over 21 the player automatically loses to the dealer.
		
		Once the player chooses to “stand”, (s)he may not take any more cards for the remainder of the
		hand and must be satisfied with his/her current score. Next the dealer shows all of his/her cards.
		If the dealer’s total score is sixteen or less, (s)he must take another card, and continue to do so
		until his/her score exceeds sixteen or surpasses 21. In the case of the dealer an Ace is always
		counted as 11 until it would put the dealers score over 21. This means that an initial 7 and Ace in
		the dealer's hand would equal 18 and the dealer would “stand”. While a 4 and Ace (15) would
		require another card. If the third card in the preceding example was an 8, (4, Ace, 8 = 23 points),
		the Ace would automatically be converted to 1 point 
		giving the dealer a total of 13 points and the
		dealer would be required to take another card.
		A player’s hand with an Ace can have a score that can be called hard or soft. A soft score is one
		in which the Ace can be used as either one or 11. But a hard score would be one in which the Ace
		can only be used as a one - such as a hand with Ace, 5, 8. In this case if the Ace were counted as
		11 the hand's score would exceed 21 points and the player would automatically lose.
		At the point where both the dealer and the player “stand”, the player’s score is compared with the
		dealer's score. Players with scores lower than that of the dealer lose to the dealer. Players with
		scores higher than that of the dealer win. In the case of tie scores the dealer is awarded the win. 
*/
