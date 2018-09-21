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