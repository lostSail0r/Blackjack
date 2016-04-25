//*******************************
//*******************************
// Chris Grady                  *
// Casino Blackjack             *
// CIST 2361 Final Project      *
// Spring 2016                  *
//*******************************
//*******************************

#include "User.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// Function Prototypes
void viewDeck(int deck[]);
void initDeck(int deck[]);
void shuffle(int deck[], int size);
void loadAccounts(class User(&data)[100], int&);
void showRules();
string gameMode(int&);
void userSignIn(int, class User(&data)[100], int&, int&, int&, int&, int&, int&, int&, int&);
void chooseDifficulty(int&);
void takeBets(int, double winnings[], double bets[]);
void insuranceBet(int, double winnings[], double sideBets[], int dHand[]);
bool doubleDown(class User(&data)[100], int players, double winnings[], double bets[], int, const int pHand[]);
void playFirstHand(int, int&, double(&winnings)[7], double(&bets)[7], class User(&data)[100], double(&sideBets)[7]);
void addToHand(int hand[], int cardToAdd);
bool playAnotherHand(char&, int);
bool hitOrStand(char&, bool, int, double winnings[], double bets[]);
void checkSoftOrHard(int p1Hand[], int p2Hand[], int p3Hand[], int p4Hand[], int p5Hand[], int p6Hand[], int p7Hand[], int);
void checkIfSoftOrHard(int pHand[], int players);
void checkIfBust(const int p1Hand[], const int p2Hand[], const int p3Hand[], const int p4Hand[], const int p5Hand[],
	const int p6Hand[], const int p7Hand[], const int dHand[], int);
void checkForBust(const int pHand[], const int dHand[], int);
void checkBlackJack(const int p1Hand[], const int p2Hand[], const int p3Hand[], const int p4Hand[], const int p5Hand[],
	const int p6Hand[], const int p7Hand[], const int dHand[], int, bool[]);
void determineWinner(const int p1Hand[], const int dHand[], int, double winnings[], double bets[], bool naturalBJ[]);
void showCard(int card);
void displayAllCards(const int cards[], int numCards, bool hideFirstCard);
void showScore(const int playerHand[], const int dHand[]);
int CardValue(int card);
int getTopCard(int deck[]);
int getHandValue(const int hand[]);
void checkSoftOrHardAI(int dHand[]);
void hitUntilStandAI(int dHand[], int deck[], int&);
void updateEarnings(class User(&data)[100], int, int&, int&, int&, int&, int&, int&, int&, int, double winnings[], double bets[]);
void determineRank(class User(&data)[100], int);
void selectionSort(class User(&data)[100], int);
void updateDB(class User(&data)[100], int);
void displayResults(class User(&data)[100], int);

// Global Constant
const int SIZE = 100;

int main()
{
	// Declaring Variables
	// Array holding winnings for each player this game
	double winnings[7] = { 50.00, 50.00, 50.00, 50.00, 50.00, 50.00, 50.00 };
	// Array holding bets for each player per hand
	double bets[7] = { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00 };
	// Array holding sidebets for players betting on insurance
	double sideBets[7] = { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00 };
	int players = 0;						// Number of players
	string mode;							// Holds game mode (human vs human or human vs computer)
	int numUsers = 0;						// Holds the amount of user accounts on file; comes in handy when used as counter
	int level;								// Holds level of difficulty; selected by user
	User accounts[SIZE];					// Declaring array holding each object of the User class  
	int p1, p2, p3, p4, p5, p6, p7;			// Hold subscript of each player's user account

	// Loads existing accounts from text file
	loadAccounts(accounts, numUsers);
	// Displays introduction/instructions to game
	showRules();
	// Allows user to select how "smart" the dealer will be
	chooseDifficulty(level);
	// Clears screen for UX
	system("cls");
	// Gets number of players; determines game mode
	mode = gameMode(players);
	system("cls");
	// Allows users to sign in/sign up
	userSignIn(players, accounts, numUsers, p1, p2, p3, p4, p5, p6, p7);
	// Main game function
	playFirstHand(players, level, winnings, bets, accounts, sideBets);
	// After user's decide to quit, all changes to player earnings are updated
	updateEarnings(accounts, numUsers, p1, p2, p3, p4, p5, p6, p7, players, winnings, bets);
	// Updates are made to text file - Now earnings and rank carry over to next program call
	updateDB(accounts, numUsers);
	// Leaderboard is displayed to user before exit
	displayResults(accounts, numUsers);
	// Game Over
	return 0;
}

/* ==================================================
Name: viewDeck
Description: Loop through the deck array and print each value.
(This was more used for debugging than functionality)
================================================== */
void viewDeck(int deck[])
{
	// Loop through the deck array and print each value.
	for (int i = 0; i < 52; i++)
	{
		cout << CardValue(deck[i]) << endl;
	}
}

/* ==================================================
Name: loadAccounts
Description: Searches text file for previous account 
entries and loads them into array of objects
================================================== */
void loadAccounts(class User(&accounts)[100], int &numUsers)
{
	// Variables that temporarily hold file contents
	string name, email, username, pw, temp, fname, lname;
	int earnings = 0, rank = 0;

	// Declaring and opening input file for initial read
	ifstream inputFile;
	inputFile.open("userDB.txt", ios::in);

	// Reads numUsers only if file isn't empty
	if (!inputFile.eof())
	{
		// Reading number of existing users
		inputFile >> numUsers;
	}

	// Reading contents of file ("database" of user account info)
	for (int i = 0; i < numUsers; i++)
	{
		inputFile >> fname;
		getline(inputFile, lname);
		name = fname + lname;
		accounts[i].name = name;
		inputFile >> accounts[i].email;
		inputFile >> accounts[i].username;
		inputFile >> accounts[i].pw;
		inputFile >> accounts[i].earnings;
		inputFile >> accounts[i].rank;
	}
	inputFile.close();
}


/* ==================================================
Name: showRules
Description: Displays the rules of the game
================================================== */
void showRules()
{
	// Display Rules to user
	// Page 1
	cout << endl;
	cout << "  ---------------------------------------------  " << endl;
	cout << " |            Welcome To Blackjack!!           | " << endl;
	cout << " |___________________RULES:____________________| " << endl;
	cout << " |     o Aces can either be 1 or 11 points.    | " << endl;
	cout << " |   o Dealer Ties are a push - bets restored  | " << endl;
	cout << " |---------------------------------------------| " << endl;
	cout << " | Earnings -                                  | " << endl;
	cout << " | o Blackjack or Winning Hand:                | " << endl;
	cout << " |   3:2 payout ratio.                         | " << endl;
	cout << " |   (150% increase of your initial bet)       | " << endl;
	cout << " | o If Only Dealer gets Blackjack:            | " << endl;
	cout << " |   - Everyone loses what they bet            | " << endl;
	cout << " | o If Dealer's score beats Player score:     | " << endl;
	cout << " |   - Player loses what they bet              | " << endl;
	cout << " | o Push (Tie Score):                         | " << endl;
	cout << " |   - Regain your initial betting amount      | " << endl;
	cout << " |---------------------------------------------|  " << endl;
	cout << " | Rankings -                                  | " << endl;
	cout << " | o You are ranked based on total earnings.   | " << endl;
	cout << " | o A rank of 0 means that you haven't played.| " << endl;
	cout << " |---------------------------------------------|  " << endl;
	cout << " |         Press any button for page 2         | " << endl;
	cout << " |_____________________________________________| " << endl << endl;
	system("pause");
	system("cls");

	// Page 2
	cout << "  ---------------------------------------------  " << endl;
	cout << " |___________________BETTING:__________________|  " << endl;
	cout << " | o Initial Bet: Entered by player each round | " << endl;
	cout << " | o Double-Down: Bet doubled and 1 card dealt | " << endl;
	cout << " |      o Insurance: Side-Bet for player       | " << endl;
	cout << " |    When Dealer's face up card is an ACE:    | " << endl;
	cout << " |    Player's can bet half of the inital bet  | " << endl;
	cout << " |    That the dealer has a natural Blackjack  | " << endl;
	cout << " |          ** INSURANCE PAYS 2:1 **           | " << endl;
	cout << "  ---------------------------------------------  " << endl;
	cout << " |    FYI: All players start off with $50 !    | " << endl;
	cout << " |    $50 FREE-TELL YOUR FRIENDS ABOUT US !    | " << endl;
	cout << " |---------------------------------------------| " << endl;
	cout << " |             Level of Difficulty:            | " << endl;
	cout << " |---------------------------------------------| " << endl;
	cout << " |   o (Beginner) - Dealer stands at soft 17   | " << endl;
	cout << " |---------------------------------------------| " << endl;
	cout << " |     o (Expert) - Dealer hits at soft 17     | " << endl;
	cout << " |_____________________________________________| " << endl << endl;
}

/* ==================================================
Name: chooseDifficulty
Desc: Prompt Player for desired level of difficulty.
Use Dealer's AI for Expert, don't use AI for
================================================== */
void chooseDifficulty(int &level)
{
	// Declaring local variables
	bool cond;

	do
	{
		//Prompt user and store input
		cout << "\n";
		cout << "Level of Difficulty - Beginner (1) or Expert (2): ";
		cin >> level;
	
		// Input Validation
		cond = cin.fail();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	} while (cond);

	while (level != 1 && level != 2)
	{
		cout << "\nERROR: Invalid Entry - Choose a level of difficulty (1 or 2):\n";
		cin >> level;
	}
}


/* ==================================================
Name: gameMode
Description: Determines whether the user wants to play 
human vs. computer or human vs human
================================================== */
string gameMode(int &players)
{
	// Declaring Local Variables
	string mode;
	bool cond;

	// Gets number of players
	do
	{
		cout << "\nHow many players are involved? (1-7):\n";
		cin >> players;
		players = int(players);

		// Input Validation
		cond = cin.fail();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	} while (cond);


	// Further Input Validation
	while (players <= 0 || players > 7)
	{
		cout << "ERROR: Invalid number of players, try again:\n";
		cin >> players;
		players = int(players);
	}

	// Returns Game Mode
	if (players == 1)
	{
		mode = "solo";
	}
	else
	{
		mode = "multi";
	}
	return mode;
}


/* ==================================================
Name: userSignIn
Desc: Allows users to sign in
================================================== */
void userSignIn(int players, class User(&accounts)[100], int &numUsers, int &p1, int &p2, int &p3, int &p4, int &p5, int &p6, int &p7)
{
	// Declaring Local Variables
	int i = 0;
	char signedUp;
	string username;				// Holds temporary value that user inputs
	string pw;				// Holds temporary value that user inputs
	bool usernameNotFound;			// Returns whether username exists
	bool pwNotFound;				//// Returns whether username exists

	cout << "\nNOTE: If this is your first time playing, you will need to sign up.\n";

	// Human vs. Computer Mode
	if (players == 1)
	{
		cout << "\nDo you already have an account? (Enter Y or N): ";
		cin >> signedUp;

		// Converting to uppercase
		signedUp = toupper(signedUp);

		// If new user
		if (signedUp == 'N')
		{
			// Clear screen
			system("CLS");

			// Getting user's name
			cout << "\nEnter your name:\n";
			cin.ignore();
			getline(cin, accounts[numUsers].name);

			// Name Input Validation
			while (accounts[numUsers].name == "")
			{
				cout << "ERROR: You cannot submit a blank name. Please try again:\n";
				getline(cin, accounts[numUsers].name);
			}

			// Clear screen
			system("CLS");

			// Getting users email address
			cout << "\nEnter your email address:\n";
			cin >> accounts[numUsers].email;

			// E-Address Input Validation
			while (accounts[numUsers].email == "")
			{
				cout << "ERROR: You cannot submit a blank address. Please try again:\n";
				cin >> accounts[numUsers].email;
			}

			// Clear screen
			system("CLS");

			// Getting Username
			cout << "\nEnter your preferred Username (No Spaces):\n";
			cin >> accounts[numUsers].username;

			while (accounts[numUsers].username == "")
			{
				cout << "ERROR: You cannot submit a blank username. Please try again:\n";
				cin >> accounts[numUsers].username;
			}

			// Clear screen
			system("CLS");

			// Getting Password
			cout << "\nEnter your password (No Spaces):\n";
			cin >> accounts[numUsers].pw;

			while (accounts[numUsers].pw == "")
			{
				cout << "ERROR: You cannot submit a blank password. Please try again:\n";
				cin >> accounts[numUsers].pw;
			}

			// Initializing earnings and rank
			accounts[numUsers].earnings = 0.0;
			accounts[numUsers].rank = 0;

			// Making a link that will be referenced later when updating earnings
			p1 = numUsers;

			// Adding to tally of users
			numUsers++;
		}

		// If existing user (1 player)
		else if (toupper(signedUp == 'Y'))
		{
			// Prompting for Username
			system("cls");
			do
			{
				cout << "Enter Username:\n";
				cin >> username;

				// Searching accounts array for username inputted by user; 
				// Returns subscript of target element
				for (i = 0; i < numUsers; i++)
				{
					if (username == accounts[i].username)
					{
						// Holds subscript of username
						p1 = i;
						usernameNotFound = false;
						break;
					}
				}
				if (i == numUsers)
				{
					cout << "\nUsername Not Found!\nTry Again!\n\n";
					usernameNotFound = true;
				}
			} while (usernameNotFound);

			// Prompting for Password
			do
			{
				cout << "\nEnter Password:\n";
				cin >> pw;

				// Searching accounts array for password inputted by user; 
				// Returns subscript of target element
				for (i = 0; i < numUsers; i++)
				{
					if (pw == accounts[i].pw)
					{
						pwNotFound = false;
						break;
					}
				}
				if (i == numUsers)
				{
					cout << "\nIncorrect Password!!\nTry Again!\n\n";
					pwNotFound = true;
				}
			} while (pwNotFound);
		}
	}
	// Multi-Player Mode (Human vs Human)
	else
	{
		system("cls");
		cout << "All " << players << " players will be asked to Login or Signup individually.\n";

		for (int eachPlayer = 0; eachPlayer < players; eachPlayer++)
		{
			if (eachPlayer > 0)
			{
				system("cls");
			}
			cout << "\nDoes Player " << (eachPlayer + 1) << " already have an account? (Enter Y or N): ";
			cin >> signedUp;

			// Converting to uppercase
			signedUp = toupper(signedUp);

			// If new user
			if (signedUp == 'N')
			{
				// Clear screen
				system("CLS");

				// Getting user's name
				cout << "\nEnter your name:\n";
				cin.ignore();
				getline(cin, accounts[numUsers].name);

				// Name Input Validation
				while (accounts[numUsers].name == "")
				{
					cout << "ERROR: You cannot submit a blank name. Please try again:\n";
					getline(cin, accounts[numUsers].name);
				}

				// Clear screen
				system("CLS");

				// Getting users email address
				cout << "\nEnter your email address:\n";
				cin >> accounts[numUsers].email;

				// E-Address Input Validation
				while (accounts[numUsers].email == "")
				{
					cout << "ERROR: You cannot submit a blank address. Please try again:\n";
					cin >> accounts[numUsers].email;
				}

				// Clear screen
				system("CLS");

				// Getting username
				cout << "\nEnter your preferred Username (No Spaces):\n";
				cin >> accounts[numUsers].username;

				while (accounts[numUsers].username == "")
				{
					cout << "ERROR: You cannot submit a blank username. Please try again:\n";
					cin >> accounts[numUsers].username;
				}

				// Clear screen
				system("CLS");


				// Getting Password
				cout << "\nEnter your password (No Spaces):\n";
				cin >> accounts[numUsers].pw;

				while (accounts[numUsers].pw == "")
				{
					cout << "ERROR: You cannot submit a blank password. Please try again:\n";
					cin >> accounts[numUsers].pw;
				}

				// Initializing earnings and rank
				accounts[numUsers].earnings = 0.0;
				accounts[numUsers].rank = 0;

				// Making a link that will be referenced later when updating earnings
				if (eachPlayer == 0)
				{
					p1 = numUsers;
				}
				else if (eachPlayer == 1)
				{
					p2 = numUsers;
				}
				else if (eachPlayer == 2)
				{
					p3 = numUsers;
				}
				else if (eachPlayer == 3)
				{
					p4 = numUsers;
				}
				else if (eachPlayer == 4)
				{
					p5 = numUsers;
				}
				else if (eachPlayer == 5)
				{
					p6 = numUsers;
				}
				else if (eachPlayer == 6)
				{
					p7 = numUsers;
				}

				// Adding to tally of users
				numUsers++;
			}

			// If existing user (Multi-player)
			else if (toupper(signedUp == 'Y'))
			{
				if (eachPlayer == 0)
				{
					system("cls");
				}
				// Prompting for Username
				do
				{
					cout << "Enter Username:\n";
					cin >> username;

					// Searching accounts array for username inputted by user; 
					// Returns subscript of target element
					for (i = 0; i < numUsers; i++)
					{
						if (username == accounts[i].username)
						{
							usernameNotFound = false;
							break;
						}
					}
					if (i == numUsers)
					{
						cout << "\nUsername Not Found!\nTry Again!\n\n";
						usernameNotFound = true;
					}
				} while (usernameNotFound);

				// Holds subscript of user
				if (eachPlayer == 0)
				{
					p1 = i;
				}
				else if (eachPlayer == 1)
				{
					p2 = i;
				}
				else if (eachPlayer == 2)
				{
					p3 = i;
				}
				else if (eachPlayer == 3)
				{
					p4 = i;
				}
				else if (eachPlayer == 4)
				{
					p5 = i;
				}
				else if (eachPlayer == 5)
				{
					p6 = i;
				}
				else if (eachPlayer == 6)
				{
					p7 = i;
				}

				// Prompting for Password
				do
				{
					cout << "\nEnter Password:\n";
					cin >> pw;

					// Searching accounts array for password inputted by user; 
					// Returns subscript of target element
					for (i = 0; i < numUsers; i++)
					{
						if (pw == accounts[i].pw)
						{
							pwNotFound = false;
							break;
						}
					}
					if (i == numUsers)
					{
						cout << "\nIncorrect Password!!\nTry Again!\n\n";
						pwNotFound = true;
					}
				} while (pwNotFound);
			}
			else
			{
				// Misc Input Validation
				system("CLS");
				cout << "Error!: Try again - Do you already have an account? (Enter Y or N): ";
				cin >> signedUp;
			}
		}
	}

}

/* ==================================================
Name: insuranceBet
Description: If dealer's face up card is an ace,
user's can bet on whether or not dealer has blackjack
================================================== */
void insuranceBet(int players, double winnings[], double sideBets[], int dHand[])
{
	// Declaring local variables
	string takeBet;
	int dealerScore = getHandValue(dHand);

	// Inform user about side-bet
	system("cls");
	cout << "The dealer's face-up card is an ACE!\n";
	cout << "There is roughly a 1 in 3 chance that \nthe dealer has BLACKJACK.\n";
	cout << "--------------";
	cout << "\nDealer's hand:" << endl;
	displayAllCards(dHand, 10, true);
	cout << "\n--------------";

	// Human vs Computer Mode
	if (players == 1)
	{
		cout << "\nWould you like to bet on insurance?\n";
		cout << "Enter Y or N: ";
		cin >> takeBet;
		// Input Validation
		while (takeBet != "Y" && takeBet != "y" && takeBet != "N" && takeBet != "n")
		{
			// Clears input error flags and removes everything currently in the input buffer. 
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "ERROR: Would you like to bet on insurance?\n";
			cin >> takeBet;
		}
		// If player wants to bet on insurance against dealer blackjack
		if (takeBet == "Y" || takeBet == "y")
		{
			cout << "\nHow much would you like to bet?: ";
			cin >> sideBets[0];
			// Verifies that player isn't betting too much
			while (sideBets[0] > winnings[0])
			{
				cout << "ERROR: You are trying to bet more than you have. Try again: ";
				cin >> sideBets[0];
			}
			// Deducts bet from winnings
			winnings[0] -= sideBets[0];
			// Checks dealer's face-down card to see if dealer has blackjack
			if (dealerScore == 21)
			{
				// Player gets 2:1 payout
				winnings[0] += (sideBets[0] + (sideBets[0] * 2));
				cout << "\nPlayer 1 won the insurance bet!\n\n";
			}
			else
			{
				cout << "\nPlayer 1 lose the side-bet!\n\n";
			}
		}
		// If player doesn't want to bet
		else if (takeBet == "N" || takeBet == "n")
		{
			cout << "\nPlayer 1 passes on insurance bet.\n\n";
		}
	}

	// Human vs Human Mode
	else if (players > 1)
	{
		for (int i = 0; i < players; i++)
		{
			// Prompting each player
			cout << "\nWould Player " << (i + 1) << " like to bet on insurance?\n";
			cout << "Enter Y or N: ";
			cin >> takeBet;
			// Input Validation
			while (takeBet != "Y" && takeBet != "y" && takeBet != "N" && takeBet != "n")
			{
				// Clears input error flags and removes everything currently in the input buffer. 
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "ERROR: Would you like to bet on insurance?\n";
				cin >> takeBet;
			}
			// If player wants to bet
			if (takeBet == "Y" || takeBet == "y")
			{
				cout << "\nHow much would Player " << (i + 1) << " like to bet?: ";
				cin >> sideBets[i];
				// Verifies that player isn't betting too much
				while (sideBets[i] > winnings[i])
				{
					cout << "ERROR: You are trying to bet more than you have. Try again: ";
					cin >> sideBets[i];
				}
				// Deducts bet from winnings
				winnings[i] -= sideBets[i];
				// Checks bet and updates winnings each iteration
				if (dealerScore == 21)
				{
					// Player gets 2:1 payout
					winnings[i] += (sideBets[i] + (sideBets[i] * 2));
				}

				cout << "Player " << (i + 1) << " bet $" << sideBets[i] << " on side-bet.\n";
			}
			// If player doesn't want to bet
			else if (takeBet == "N" || takeBet == "n")
			{
				cout << "\nPlayer " << (i + 1) << " passes on insurance bet.\n\n";
			}
		}
	}
	// Checks dealer's face-down card to see if dealer has blackjack
	system("cls");
	if (dealerScore == 21)
	{
		cout << "The Dealer has Blackjack. Players that bet won insurance money!!\n";
	}
	else
	{
		cout << "The Dealer didn't have Blackjack. Insurance betters lost side-bet.\n";
	}
	cout << "------------------------------------------------------------------\n\n";
}

/* ==================================================
Name: takeBets
Description: Ask each player to enter their initial bet
================================================== */
void takeBets(int players, double winnings[], double bets[])
{
	system("cls");
	cout << "Each player will now place their bets:\n";
	cout << "-------------------------------------\n";
	// Loops through each player
	for (int i = 0; i < players; i++)
	{
		// Clears screen after first player places bet
		if (i > 0)
		{
			system("cls");
			cout << "Each player will now place their bets:\n";
			cout << "-------------------------------------\n";
		}
		// Get user input
		cout << "Player " << (i + 1) << ": Place your Bet (Any number between 2 and 500):\n";
		cin >> bets[i];
		// Validate Input
		if (bets[i] >= 2.00 && bets[i] <= 500.00)
		{
			// Deducts betting amount from player's winnings
			winnings[i] -= bets[i];
			continue;
		}
		else
		{
			do {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				//Display error message
				cout << "ERROR: Invalid Entry - Try Again (HINT: Don't include dollar sign):\n";
				cin >> bets[i];
			} while (bets[i] < 2.00 || bets[i] > 500.00);
		}
	}
}

/* ==================================================
Name: doubleDown
Description: Asks eligible players if they want to double down
================================================== */
bool doubleDown(class User(&data)[100], int players, double winnings[], double bets[], int playerID, int pHand[])
{
	// Declaring local variable
	char dd;
	int p = (playerID - 1);

	// If initial card values are 9, 10, or 11...
	if (getHandValue(pHand) == 9 || getHandValue(pHand) == 10 || getHandValue(pHand) == 11)
	{
		// Let user know that they can double down; get input
		cout << "\n\nPLAYER " << playerID << " is eligible to double-down.\n";
		cout << "Would PLAYER " << playerID << " like to double-down? (Enter Y or N):\n";
		cin >> dd;
		// Input Validation
		while (dd != 'Y' && dd != 'y' && dd != 'N' && dd != 'n')
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nERROR: Invalid Entry. Would you like to double-down? (Enter Y or N):\n";
			cin >> dd;
		}

		// If user doubles down
		if (dd == 'Y' || dd == 'y')
		{
			// Deducts amount initially bet from player
			winnings[p] -= bets[p];
			// Doubles amount betted (So that proper winnings can be provided)
			bets[p] += bets[p];
			return(true);
		}
		// If user declines
		else if (dd == 'N' || dd == 'n')
		{
			return(false);
		}
		else
		{
			return(false);
		}
	}
	// If not eligible for double down 
	else
	{
		return(false);
	}
}

/* ==================================================
Name: playFirstHand
Description: Main game function
================================================== */
void playFirstHand(int players, int &level, double(&winnings)[7], double(&bets)[7], class User(&accounts)[100], double(&sideBets)[7])
{
	// Local variable
	char keepPlaying = 'N';
	bool dd;
	bool naturalBJ[7] = { false, false, false, false, false, false, false };

	// Plays at least one hand; prompts user after each round to continue
	do
	{
		//Declare local variable
		char Draw = 'H';

		// Re-initialize bets and natural blackjack array
		// This keeps previous round variables from getting in the way
		for (int b = 0; b < 7; b++)
		{
			bets[b] = 0.00;
			naturalBJ[b] = false;
		}

		//Declare, initialize and shuffle the deck.
		int deck[52];
		initDeck(deck);
		shuffle(deck, 51);

		// Declare and initialize Player and Dealer hands
		int p1Hand[10] = { 0 };
		int p2Hand[10] = { 0 };
		int p3Hand[10] = { 0 };
		int p4Hand[10] = { 0 };
		int p5Hand[10] = { 0 };
		int p6Hand[10] = { 0 };
		int p7Hand[10] = { 0 };
		int dHand[10] = { 0 };

		//Deal first round; based on number of players
		if (players == 1)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
		}
		else if (players == 2)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
		}
		else if (players == 3)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
		}
		else if (players == 4)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
		}
		else if (players == 5)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
		}
		else if (players == 6)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
			addToHand(p6Hand, getTopCard(deck));
			addToHand(p6Hand, getTopCard(deck));
		}
		else if (players == 7)
		{
			addToHand(dHand, getTopCard(deck));
			addToHand(dHand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p1Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p2Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p3Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p4Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
			addToHand(p5Hand, getTopCard(deck));
			addToHand(p6Hand, getTopCard(deck));
			addToHand(p6Hand, getTopCard(deck));
			addToHand(p7Hand, getTopCard(deck));
			addToHand(p7Hand, getTopCard(deck));
		}

		// Calls function that takes initial bets every round
		takeBets(players, winnings, bets);

		// Clear screen and update hands
		system("cls");
		cout << endl;
		cout << "The Dealer has shuffled the deck and dealt each player their card(s).\n";

		// Display Player and Dealer hands
		// Human vs Computer mode
		if (players == 1)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);
		}
		// Human vs Human mode
		else if (players == 2)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);
		}
		else if (players == 3)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);

			cout << "\nPlayer 3 hand:" << endl;
			displayAllCards(p3Hand, 10, false);
		}
		else if (players == 4)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);

			cout << "\nPlayer 3 hand:" << endl;
			displayAllCards(p3Hand, 10, false);

			cout << "\nPlayer 4 hand:" << endl;
			displayAllCards(p4Hand, 10, false);
		}
		else if (players == 5)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);

			cout << "\nPlayer 3 hand:" << endl;
			displayAllCards(p3Hand, 10, false);

			cout << "\nPlayer 4 hand:" << endl;
			displayAllCards(p4Hand, 10, false);

			cout << "\nPlayer 5 hand:" << endl;
			displayAllCards(p5Hand, 10, false);
		}
		else if (players == 6)
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);

			cout << "\nPlayer 3 hand:" << endl;
			displayAllCards(p3Hand, 10, false);

			cout << "\nPlayer 4 hand:" << endl;
			displayAllCards(p4Hand, 10, false);

			cout << "\nPlayer 5 hand:" << endl;
			displayAllCards(p5Hand, 10, false);

			cout << "\nPlayer 6 hand:" << endl;
			displayAllCards(p6Hand, 10, false);
		}
		else
		{
			cout << "Player 1 hand:" << endl;
			displayAllCards(p1Hand, 10, false);

			cout << "\nPlayer 2 hand:" << endl;
			displayAllCards(p2Hand, 10, false);

			cout << "\nPlayer 3 hand:" << endl;
			displayAllCards(p3Hand, 10, false);

			cout << "\nPlayer 4 hand:" << endl;
			displayAllCards(p4Hand, 10, false);

			cout << "\nPlayer 5 hand:" << endl;
			displayAllCards(p5Hand, 10, false);

			cout << "\nPlayer 6 hand:" << endl;
			displayAllCards(p6Hand, 10, false);

			cout << "\nPlayer 7 hand:" << endl;
			displayAllCards(p7Hand, 10, false);
		}
		// Displays dealer's hand
		cout << "\nDealer's hand:" << endl;
		displayAllCards(dHand, 10, true);
		cout << endl;

		// INSURANCE
		// If dealer's face up card is an ace - 
		// Calls function to take insurance side-bets 
		if (CardValue(dHand[1]) == 11 || dHand[1] == 1)
		{
			insuranceBet(players, winnings, sideBets, dHand);
			// Display Player and Dealer hands
			if (players == 1)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);
			}
			else if (players == 2)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);
			}
			else if (players == 3)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);

				cout << "\nPlayer 3 hand:" << endl;
				displayAllCards(p3Hand, 10, false);
			}
			else if (players == 4)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);

				cout << "\nPlayer 3 hand:" << endl;
				displayAllCards(p3Hand, 10, false);

				cout << "\nPlayer 4 hand:" << endl;
				displayAllCards(p4Hand, 10, false);
			}
			else if (players == 5)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);

				cout << "\nPlayer 3 hand:" << endl;
				displayAllCards(p3Hand, 10, false);

				cout << "\nPlayer 4 hand:" << endl;
				displayAllCards(p4Hand, 10, false);

				cout << "\nPlayer 5 hand:" << endl;
				displayAllCards(p5Hand, 10, false);
			}
			else if (players == 6)
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);

				cout << "\nPlayer 3 hand:" << endl;
				displayAllCards(p3Hand, 10, false);

				cout << "\nPlayer 4 hand:" << endl;
				displayAllCards(p4Hand, 10, false);

				cout << "\nPlayer 5 hand:" << endl;
				displayAllCards(p5Hand, 10, false);

				cout << "\nPlayer 6 hand:" << endl;
				displayAllCards(p6Hand, 10, false);
			}
			else
			{
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);

				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);

				cout << "\nPlayer 3 hand:" << endl;
				displayAllCards(p3Hand, 10, false);

				cout << "\nPlayer 4 hand:" << endl;
				displayAllCards(p4Hand, 10, false);

				cout << "\nPlayer 5 hand:" << endl;
				displayAllCards(p5Hand, 10, false);

				cout << "\nPlayer 6 hand:" << endl;
				displayAllCards(p6Hand, 10, false);

				cout << "\nPlayer 7 hand:" << endl;
				displayAllCards(p7Hand, 10, false);
			}
			cout << "\nDealer's hand:" << endl;
			displayAllCards(dHand, 10, true);
			cout << endl;
		}

		// Checks players hands before any more cards are dealt
		// If Player has an Ace, see if Player want's to have a soft or hard hand.
		checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
		// Check for Natural Blackjack.
		checkBlackJack(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, dHand, players, naturalBJ);

		// Each iteration is a player's turn
		// After this for loop, the dealer makes
		// His final moves and the round is over
		for (int player = 0; player < players; player++)
		{
			// UX Formatting for Human vs Human Mode
			if (player > 0)
			{
				// Displays for Player 1 & 2
				system("cls");
				if (naturalBJ[player - 1] == true)
				{
					cout << "\nPlayer " << (player) << " got a natural blackjack!\n";
					cout << "---------------------------------\n\n";
				}
				cout << "";
				cout << "Player 1 hand:" << endl;
				displayAllCards(p1Hand, 10, false);
				cout << "\nPlayer 2 hand:" << endl;
				displayAllCards(p2Hand, 10, false);
				if (players < 3)
				{
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
				// If more than 2 players;
				if (players == 3)
				{
					cout << "\nPlayer 3 hand:" << endl;
					displayAllCards(p3Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
				if (players == 4)
				{
					cout << "\nPlayer 4 hand:" << endl;
					displayAllCards(p4Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
				if (players == 5)
				{
					cout << "\nPlayer 5 hand:" << endl;
					displayAllCards(p5Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
				if (players == 6)
				{
					cout << "\nPlayer 6 hand:" << endl;
					displayAllCards(p6Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
				if (players == 7)
				{
					cout << "\nPlayer 7 hand:" << endl;
					displayAllCards(p7Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
				}
			}
			// Calls function regarding doubling down
			// (Only eligible players are prompted - Scores of 9,10, or 11)
			// Returns dd flag; determines whether or not user should be able to hit after
			switch (player)
			{
			case 0:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p1Hand);
				break;
			case 1:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p2Hand);
				break;
			case 2:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p3Hand);
				break;
			case 3:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p4Hand);
				break;
			case 4:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p5Hand);
				break;
			case 5:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p6Hand);
				break;
			case 6:
				dd = doubleDown(accounts, players, winnings, bets, (player + 1), p7Hand);
				break;
			}

			// Adds single card to hand if user doubles down
			// This is basically the dd alternative to hitting or standing
			if (dd)
			{
				system("cls");
				cout << "Player " << (player + 1) << " doubled his bet.\n";
				cout << "-------------------------\n";
				// Updates hand for respective player
				switch (player)
				{
				case 0:
					addToHand(p1Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p1Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p1Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 1:
					addToHand(p2Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p2Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p2Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 2:
					addToHand(p3Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p3Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p3Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 3:
					addToHand(p4Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p4Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p4Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 4:
					addToHand(p5Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p5Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p5Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 5:
					addToHand(p6Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p6Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p6Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				case 6:
					addToHand(p7Hand, getTopCard(deck));
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p7Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					checkIfSoftOrHard(p7Hand, players);
					cout << endl << endl;
					system("pause");
					break;
				}
			}
			else if (naturalBJ[player])
			{
				continue;
			}
			else
			{
				cout << "\n\nPlayer " << (player + 1) << ":\n";
				cout << "----------------------------------------------";
			}

			// Each player decides to hit or stand 
			// (Excluding those who doubled-down; They are dealt separately above this)
			while (hitOrStand(Draw, dd, player, winnings, bets))
			{
				// Player 1
				if (player == 0)
				{
					// If user hits, add card to hand
					addToHand(p1Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p1Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p1Hand, players);
					// Check to see if player lost
					checkForBust(p1Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p1Hand) > 21)
					{
						break;
					}
				}
				// Player 2 (If applicable)
				else if (player == 1)
				{
					// If user hits, add card to hand
					addToHand(p2Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p2Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p2Hand, players);
					// Check to see if player lost
					checkForBust(p2Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p2Hand) > 21)
					{
						break;
					}
				}
				// Player 3 (If applicable)
				else if (player == 2)
				{
					// If user hits, add card to hand
					addToHand(p3Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p3Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p3Hand, players);
					// Check to see if player lost
					checkForBust(p3Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p3Hand) > 21)
					{
						break;
					}
				}
				// Player 4 (If applicable)
				else if (player == 3)
				{
					// If user hits, add card to hand
					addToHand(p4Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p4Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p4Hand, players);
					// Check to see if player lost
					checkForBust(p4Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p4Hand) > 21)
					{
						break;
					}
				}
				// Player 5 (If applicable)
				else if (player == 4)
				{
					// If user hits, add card to hand
					addToHand(p5Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p5Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p5Hand, players);
					// Check to see if player lost
					checkForBust(p5Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p5Hand) > 21)
					{
						break;
					}
				}
				// Player 6 (If applicable)
				else if (player == 5)
				{
					// If user hits, add card to hand
					addToHand(p6Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p6Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p6Hand, players);
					// Check to see if player lost
					checkForBust(p6Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p6Hand) > 21)
					{
						break;
					}
				}
				// Player 7 (If applicable)
				else if (player == 6)
				{
					// If user hits, add card to hand
					addToHand(p7Hand, getTopCard(deck));
					cout << "The Dealer dealt you another card.\n" << endl;
					// Display Player's updated hand
					cout << "Your hand:" << endl;
					displayAllCards(p7Hand, 10, false);
					cout << "\nDealer's hand:" << endl;
					displayAllCards(dHand, 10, true);
					// If Player has an Ace, see if Player want's to have a soft or hard hand.
					checkIfSoftOrHard(p7Hand, players);
					// Check to see if player lost
					checkForBust(p7Hand, dHand, players);
					// Allows user to hit again if possible
					if (getHandValue(p7Hand) > 21)
					{
						break;
					}
				}
			}
		}
		// This just makes it look nicer when the last user doubles down
		if (!dd)
		{
			system("cls");
		}
		else
		{
			cout << endl << endl;
		}
		// Dealer hits or stand depending on AI setting
		hitUntilStandAI(dHand, deck, level);
		cout << endl;

		// Check for other winner(s)
		if (players == 1)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
		}
		else if (players == 2)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
		}
		else if (players == 3)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p3Hand, dHand, players, winnings, bets, naturalBJ);
		}
		else if (players == 4)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p3Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p4Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
		}
		else if (players == 5)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p3Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p4Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p5Hand, dHand, players, winnings, bets, naturalBJ);
		}
		else if (players == 6)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p3Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p4Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p5Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p6Hand, dHand, players, winnings, bets, naturalBJ);
		}
		else if (players == 7)
		{
			determineWinner(p1Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p2Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p3Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p4Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p5Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p6Hand, dHand, players, winnings, bets, naturalBJ);
			cout << endl;
			determineWinner(p7Hand, dHand, players, winnings, bets, naturalBJ);
		}
	} while (playAnotherHand(keepPlaying, players));
}

/* ==================================================
Name: initDeck
Description: Create the cards background values
================================================== */
void initDeck(int deck[])
{
	//Declare local variables.
	int Rank = 101;
	int i = 0;

	//Hearts
	for (i = 0; i <= 13; i++)
	{
		deck[i] = Rank++;
	}

	//Diamonds
	for (i = 13; i <= 26; i++)
	{
		deck[i] = Rank++ + 100 - 14;
	}

	//Clubs
	for (i = 26; i <= 39; i++)
	{
		deck[i] = Rank++ + 200 - 28;
	}

	//Spades
	for (i = 39; i <= 51; i++)
	{
		deck[i] = Rank++ + 300 - 42;
	}
}

/* ==================================================
Name: shuffle
Description: Randomly rearranges deck of cards
================================================== */
void shuffle(int deck[], int size)
{
	// Declaring local variables
	int xRan;
	// Using time for truly random number
	srand(time(0));
	// Randomizing number determining how many times deck is shuffled
	xRan = rand() % 50 + 5;

	random_shuffle(deck, deck + 51);
}

/* ==================================================
Name: showCard
Description: Displays a given card's rank and suit.
================================================== */
void showCard(int card)
{
	//Show nothing for non cards (ie.0)
	if (card == 0)
	{
		cout << "";
	}
	//Define Ranks.
	else
	{
		switch (card % 100)
		{
		case 1:
			cout << "[A";
			break;
		case 11:
			cout << "[J";
			break;
		case 12:
			cout << "[K";
			break;
		case 13:
			cout << "[Q";
			break;
		case 14:
			cout << "[A";
			break;

			//For non-face cards, just use their 10s value as rank.
		default:
			cout << "[" << card % 100;
		}
	}

	//Show nothing for non cards (ie.0)
	if (card == 0)
	{
		cout << "";
	}
	//Define Suits.
	else
	{
		//Hearts
		if ((card >= 101) && (card <= 114))
		{
			cout << static_cast<char>(3) << "]";
		}

		//Diamonds
		else if ((card >= 201) && (card <= 214))
		{
			cout << static_cast<char>(4) << "]";
		}

		//Clubs
		else if ((card >= 301) && (card <= 314))
		{
			cout << static_cast<char>(5) << "]";
		}

		//Spades
		else if ((card >= 401) && (card <= 414))
		{
			cout << static_cast<char>(6) << "]";
		}
	}
}

/* ==================================================
Name: displayAllCards
Description: Displays rank and suite for an array of
cards given; also handles hiding dealer's card
================================================== */
void displayAllCards(const int deck[], int numCards, bool hideFirstCard)
{
	// If true, hide dealer's first card
	if (hideFirstCard)
	{
		cout << "[**] ";
	}

	// If false, show player's first card
	else
	{
		showCard(deck[0]);
		cout << " ";
	}

	// Display all cards after first in hand
	for (int i = 1; i < numCards; i++)
	{
		// Show Hand
		if (deck[i] != 0)
		{
			showCard(deck[i]);
			cout << " ";
		}

		// Show nothing for 0 cards
		else
		{
			cout << "";
		}
	}
}

/* ==================================================
Name: CardValue
Description: Returns a given card's value.
================================================== */
int CardValue(int card)
{
	//Declare local variable
	int cardVal;

	//Get the card's background 10s value and assign the card a point value
	switch (card % 100)
	{
	case 1:
		cardVal = 11;
		break;
	case 11:
	case 12:
	case 13:
		cardVal = 10;
		break;
	case 14:
		cardVal = 1;
		break;
		//For non-face cards, just use their backround 10s value as point value.
	default:
		cardVal = (card % 100);
	}
	return cardVal;
}

/* ==================================================
Name: getTopCard
Description: When dealing, grabs top card from deck[]
================================================== */
int getTopCard(int deck[])
{
	// Declaring local variable
	int topCard = 0;

	//Loop through the deck
	for (int i = 0; i < 51; i++)
	{
		//Find a card that isn't empty
		if (deck[i] != 0)
		{
			//Clone card for safe keeping 
			//Replace first card with the empty card value
			//Return the clone of the card
			topCard = deck[i];
			deck[i] = 0;
			return topCard;
		}
		else
		{
			continue;
		}
	}
}

/* ==================================================
Name: addToHand
Description: Adds the given card to the players hand.
================================================== */
void addToHand(int hand[], int cardToAdd)
{
	//Loop through the hand
	for (int i = 0; i < 9; i++)
	{
		//If there is an empty card spot
		if (hand[i] == 0)
		{
			//Replace it with the card to add
			hand[i] = cardToAdd;
			break;
		}
	}
}

/* ==================================================
Name: hitUntilStandAI
Description: AI logic causing the dealer to hit until a soft 17.
================================================== */
void hitUntilStandAI(int dHand[], int deck[], int &level)
{
	// Loops through Dealer's hand
	for (int i = 0; i < 9; i++)
	{
		// If dealer hits
		if (getHandValue(dHand) < 17)
		{
			// If Level of difficulty is 1;
			if (level == 1)
			{
				addToHand(dHand, getTopCard(deck));
			}

			// More aware of Aces if level of difficulty is 2
			else if (level == 2)
			{
				addToHand(dHand, getTopCard(deck));
				checkSoftOrHardAI(dHand);
			}
		}
		// If Dealer stands
		else
		{
			// If dealer didn't hit to get to 17
			if (i == 0)
			{
				cout << "The Dealer stands." << endl;
				break;
			}
			// Dealer hit once to get to 17
			else if (i == 1)
			{
				cout << "The Dealer hit a card and stands." << endl;
				break;
			}
			// Dealer hit more than once to get to 17
			else
			{
				cout << "The Dealer hit " << i << " cards and stands." << endl;
				break;
			}
		}
		// Double-Checking Ace AI
		if (level == 2)
		{
			checkSoftOrHardAI(dHand);
		}
	}
}

/* ==================================================
Name: getHandValue
Description: Calculates and returns the value of given hand.
================================================== */
int getHandValue(const int hand[])
{
	//Declare local variables
	int addCardValues = 0;

	//Loop through the hand and add up the card values
	for (int i = 0; i < 9; i++)
	{
		addCardValues = addCardValues + CardValue(hand[i]);
	}

	//Give the sum of the card values as the hand value
	return addCardValues;
}

/* ==================================================
Name: playAnotherHand
Description: Prompts user to play another hand
==================================================*/
bool playAnotherHand(char& keepPlaying, int players)
{
	//Prompt user to see if they would like to play another round.
	cout << endl << "\nWould you like to play another hand? (Enter Y or N) ";
	cin >> keepPlaying;
	cout << "\n" << endl;

	// If yes, restart program 
	if (keepPlaying == 'y' || keepPlaying == 'Y')
	{
		return(true);
	}
	// If no, quit
	else
	{
		return(false);
	}
}

/* ==================================================
Name: hitOrStand
Description: Determines if user wants to hit.
================================================== */
bool hitOrStand(char& Draw, bool dd, int player, double winnings[], double bets[])
{
	if (!dd)
	{
		// Prompt user to see if they would like to enter a new time.
		cout << endl << "\nWould you like to hit or stand? (Enter H or S) ";
		cin >> Draw;
		cout << "\n";

		//If they do, return true and start the loop to draw another card.
		if (Draw == 'h' || Draw == 'H')
		{
			return(true);
		}
		// If they stand, skip the loop. 
		else if (Draw == 's' || Draw == 'S')
		{
			return(false);
		}
		else
		{
			return(false);
		}
	}
	// If user double-downed
	else if (dd)
	{
		cout << "\nPlayer " << (player + 1) << " doubled his bet.\n";
		return(false);
	}
	else
	{
		return(false);
	}
}

/* ==================================================
Name: determineWinner
Description: Determines the winner of each round
================================================== */
void determineWinner(const int pHand[], const int dHand[], int players, double winnings[], double bets[], bool naturalBJ[])
{
	// Static variable to hold how many times function has been called;
	// Useful to determine which player was sent as an argument
	static int call_count = 1;

	// Declaring local variables
	int playerScore = getHandValue(pHand);
	int dealerScore = getHandValue(dHand);

	// Show hand from round
	cout << "Player " << call_count << ":\n";
	showScore(pHand, dHand);

	// Human vs Computer (1 Player Mode)
	if (players == 1)
	{
		// Winning tree
		// If user got natural blackjack and dealer didn't
		if (naturalBJ[0] && dealerScore != 21)
		{
			cout << "\n";
			cout << "You WON with a natural blackjack this round!" << endl;
			winnings[0] += (bets[0] + (1.5 * bets[0]));
		}
		// If user didn't get natural blackjack but has 21 or is closer than dealer; or if dealer busts
		else if ((playerScore < 22) && (playerScore > dealerScore)
			|| ((dealerScore > 21) && (playerScore < 22)))
		{
			cout << "\n";
			cout << "You WON this round!" << endl;
			winnings[0] += (bets[0] * 2);
		}
		// Losing tree; if user is not in better position than dealer
		else
		{
			// If scores are tied, Inform player that this round was a push
			// This includes if both have blackjack
			if (playerScore == dealerScore)
			{
				cout << "\n";
				cout << "That round was a TIE - betting amount returned." << endl;
				winnings[0] += bets[0];
			}

			// If user straight-up lost, inform player that they lost
			else
			{
				cout << "\n";
				cout << "You LOSE this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
			}
		}
	}
	else if (players > 1)
	{
		// Winning tree
		// If Natural Blackjack
		if (naturalBJ[0] && call_count == 1)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[0] += (bets[0] + (1.5 * bets[0]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[1] && call_count == 2)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[1] += (bets[1] + (1.5 * bets[1]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[2] && call_count == 3)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[2] += (bets[2] + (1.5 * bets[2]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[3] && call_count == 4)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[3] += (bets[3] + (1.5 * bets[3]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[4] && call_count == 5)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[4] += (bets[4] + (1.5 * bets[4]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[5] && call_count == 6)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[5] += (bets[5] + (1.5 * bets[5]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		else if (naturalBJ[6] && call_count == 7)
		{
			cout << "\nPlayer " << call_count << " WON with a Natural Blackjack this round!" << endl;
			winnings[6] += (bets[6] + (1.5 * bets[6]));
			if (call_count < players)
			{
				call_count++;
			}
			else
			{
				call_count = 1;
			}
		}
		// If player wins without natural blackjack
		else if ((playerScore < 22) && (playerScore > dealerScore)
			|| ((dealerScore > 21) && (playerScore < 22)))
		{
			if (call_count == 1)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[0] += (bets[0] + bets[0]);
				if (call_count < players)
				{
					call_count++;
				}
				// This re-initializes call_count in case user(s) want to play another round
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 2)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[1] += (bets[1] + bets[1]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 3)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[2] += (bets[2] + bets[2]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 4)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[3] += (bets[3] + bets[3]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 5)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[4] += (bets[4] + bets[4]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 6)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[5] += (bets[5] + bets[5]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 7)
			{
				cout << "\nPlayer " << call_count << " WON this round!" << endl;
				winnings[6] += (bets[6] + bets[6]);
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
		}
		// Tie Tree
		else if (playerScore == dealerScore)
		{
			if (call_count == 1)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[0] += bets[0];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 2)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[1] += bets[1];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 3)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[2] += bets[2];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 4)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[3] += bets[3];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 5)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[4] += bets[4];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 6)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[5] += bets[5];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 7)
			{
				cout << "\nPlayer " << call_count << " TIED the dealer; Betting amount returned.\n";
				winnings[6] += bets[6];
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
		}
		// Losing Tree
		else
		{
			if (call_count == 1)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 2)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 3)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 4)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 5)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 6)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
			else if (call_count == 7)
			{
				cout << "\nPlayer " << call_count << " LOST this round." << endl;
				// No need to deduct from winnings because beting 
				// amount was already subtracted in takeBets()
				if (call_count < players)
				{
					call_count++;
				}
				else
				{
					call_count = 1;
				}
			}
		}
	}
}

/* ==================================================
Name: checkIfBust
Description: Check if Player or Dealer bust - score > 21
This function is for instances when every players is checked
================================================== */
void checkIfBust(const int p1Hand[], const int p2Hand[], const int p3Hand[], const int p4Hand[], const int p5Hand[], const int p6Hand[], const int p7Hand[], const int dHand[], int players)
{
	//Define local variables.
	int p1Score = getHandValue(p1Hand);
	int p2Score = getHandValue(p2Hand);
	int p3Score = getHandValue(p3Hand);
	int p4Score = getHandValue(p4Hand);
	int p5Score = getHandValue(p5Hand);
	int p6Score = getHandValue(p6Hand);
	int p7Score = getHandValue(p7Hand);
	int dealerScore = getHandValue(dHand);

	// If Player 1 busts
	if (p1Score > 21)
	{
		cout << "Player 1 busted with " << getHandValue(p1Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 2 busts
	else if (p2Score > 21)
	{
		cout << "Player 2 busted with " << getHandValue(p2Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 3 busts
	else if (p3Score > 21)
	{
		cout << "Player 3 busted with " << getHandValue(p3Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 4 busts
	else if (p4Score > 21)
	{
		cout << "Player 4 busted with " << getHandValue(p4Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 5 busts
	else if (p5Score > 21)
	{
		cout << "Player 5 busted with " << getHandValue(p5Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 6 busts
	else if (p6Score > 21)
	{
		cout << "Player 6 busted with " << getHandValue(p6Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If Player 7 busts
	else if (p7Score > 21)
	{
		cout << "Player 7 busted with " << getHandValue(p7Hand) << " points." << endl;
		cout << "\n" << endl;
	}
	// If dealer busts
	else if (dealerScore > 21)
	{
		if (players == 1)
		{
			showScore(p1Hand, dHand);
			cout << "\n" << endl;
			cout << "The Dealer went bust. You Win!" << endl;
			cout << "\n" << endl;
		}
		else
		{
			cout << "\n" << endl;
			cout << "The Dealer went bust. Everybody else Wins!" << endl;
			cout << "\n" << endl;
		}
	}
}

/* ==================================================
Name: checkForBust
Description: Check if Player or Dealer bust - score > 21
This function is for instances when individual players are checked
Used after player hits and individual check is needed for bust
================================================== */
void checkForBust(const int pHand[], const int dHand[], int players)
{
	// Define local variables.
	int playerScore = getHandValue(pHand);
	int dealerScore = getHandValue(dHand);

	// Check if Player busts
	if (playerScore > 21)
	{
		cout << "You bust with " << getHandValue(pHand) << " points." << endl;
		cout << "\n" << endl;
	}
}

/* ==================================================
Name: checkBlackJack
Description: Checks if a given hand has a blackjack.
================================================== */
void checkBlackJack(const int p1Hand[], const int p2Hand[], const int p3Hand[], const int p4Hand[], const int p5Hand[],
	const int p6Hand[], const int p7Hand[], const int dHand[], int players, bool naturalBJ[])
{
	//Define local Variables
	int p1Score = getHandValue(p1Hand);
	int p2Score = getHandValue(p2Hand);
	int p3Score = getHandValue(p3Hand);
	int p4Score = getHandValue(p4Hand);
	int p5Score = getHandValue(p5Hand);
	int p6Score = getHandValue(p6Hand);
	int p7Score = getHandValue(p7Hand);
	int dealerScore = getHandValue(dHand);

	//If Player 1 has blackjack but Dealer doesn't. 
	if ((p1Score == 21) && (dealerScore != 21))
	{
		// Display message
		cout << "\n\n";
		showScore(p1Hand, dHand);
		cout << "\n";
		cout << "Player 1 got a natural Blackjack!" << endl;
		cout << "\n";
		naturalBJ[0] = true;
	}

	// Multi-Player Mode
	if (players > 1)
	{
		//If Player 2 has blackjack but Dealer doesn't. 
		if ((p2Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p2Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 2 Wins" << endl;
			cout << "\n";
			naturalBJ[1] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p2Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p2Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";

		}

		//If Player 3 has blackjack but Dealer doesn't. 
		if ((p3Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p3Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 3 Wins" << endl;
			cout << "\n";
			naturalBJ[2] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p3Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p3Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";

		}

		//If Player 4 has blackjack but Dealer doesn't. 
		if ((p4Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p4Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 4 Wins" << endl;
			cout << "\n";
			naturalBJ[3] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p4Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p4Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";

		}

		//If Player 5 has blackjack but Dealer doesn't. 
		if ((p5Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p5Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 5 Wins" << endl;
			cout << "\n";
			naturalBJ[4] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p5Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p5Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";

		}

		//If Player 6 has blackjack but Dealer doesn't. 
		if ((p6Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p6Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 6 Wins" << endl;
			cout << "\n";
			naturalBJ[5] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p6Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p6Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";

		}

		//If Player 7 has blackjack but Dealer doesn't. 
		if ((p7Score == 21) && (dealerScore != 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			cout << "\n\n";
			showScore(p7Hand, dHand);
			cout << "\n";
			cout << "Blackjack! Player 7 Wins" << endl;
			cout << "\n";
			naturalBJ[6] = true;

		}
		//If both Player and Dealer have blackjack. 
		else if ((p7Score == 21) && (dealerScore == 21))
		{
			//Display message, compute new winnings multiplier, ask to play another hand.
			showScore(p7Hand, dHand);
			cout << "\n";
			cout << "The Dealer and you both got Blackjack." << endl;
			cout << "\n";
		}
	}
}

/* ==================================================
Name: showScore
Description: Displays scoreboard with cards for each hand
as well as totals for each hand for each player and dealer
================================================== */
void showScore(const int playerHand[], const int dHand[])
{
	//Display Player's cards/hand value
	cout << "Player hand: ";
	displayAllCards(playerHand, 10, false);
	cout << " (" << getHandValue(playerHand) << "pts)." << endl;

	cout << "V.S." << endl;

	//Display Dealer's cards/hand value
	cout << "Dealer hand: ";
	displayAllCards(dHand, 10, false);
	cout << " (" << getHandValue(dHand) << "pts)." << endl;
}

/* ==================================================
Name: checkIfSoftOrHard
Description: Asks Player if they would like their Ace to
be used as 1 or 11 points - same as function below this
except that it is for passing a single hand, as opposed to all
================================================== */
void checkIfSoftOrHard(int pHand[], int players)
{
	// Loops through player's hand
	// Starts at third card
	// First two cards have already been tested for Ace's
	for (int i = 2; i<9; i++)
	{
		//Define local variables.
		int checkAce = CardValue(pHand[i]);
		int softOrHard;

		//If card in hand is an Ace prompt Player for input
		if (checkAce == 1 || checkAce == 11)
		{
			cout << "\n";
			cout << "\nWould you like your Ace to count as 1 or 11 points? (1/11): ";
			cin >> softOrHard;

			//If Player chooses 1
			if (softOrHard == 1)
			{
				if (checkAce == 11)
					pHand[i] = pHand[i] + 13;
			}

			//If Player chooses 11
			else if (softOrHard == 11)
			{
				if (checkAce == 1)
				{
					pHand[i] = pHand[i] - 13;
				}
			}

			// Input validation - If player doesn't input 1 or 11
			else if (softOrHard != 1 || softOrHard != 11)
			{
				// Clears input error flags and removes everything currently in the input buffer. 
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// Display error message and restart the function
				cout << "\nPlease enter the number 1 or 11." << endl;
				checkIfSoftOrHard(pHand, players);
			}
		}
	}
}

/* ==================================================
Name: checkSoftOrHard
Description: Asks Player if they would like their Ace to
be used as 1 or 11 points and then computes updates hand
================================================== */
void checkSoftOrHard(int p1Hand[], int p2Hand[], int p3Hand[], int p4Hand[], int p5Hand[], int p6Hand[], int p7Hand[], int players)
{
	// Declaring local variable
	bool cond;
	int softOrHard;
	int checkAce;

	// Human vs Computer Mode
	if (players == 1)
	{
		//check cards in hand
		for (int i = 0; i<9; i++)
		{
			checkAce = CardValue(p1Hand[i]);

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				do
				{
					cout << "\n\nWould you like your Ace to count as 1 or 11 points?: ";
					cin >> softOrHard;
					// Input Validation - If user enters wrong datatype.. 
					cond = cin.fail();
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				} while (cond);

				// More input validation
				while (softOrHard != 1 && softOrHard != 11)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					// Display error message and gets input
					cout << "\nPlease enter the number 1 or 11:" << endl;
					cin >> softOrHard;
				}
				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}
				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}
				else
				{
					cout << "\nUNKNOWN ERROR!\n";
				}
			}
		}
	}
	// Human vs Human mode
	else if (players == 2)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else if (players == 3)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 3
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p3Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 3 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p3Hand[i] = p3Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p3Hand[i] = p3Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else if (players == 4)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 3
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p3Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 3 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p3Hand[i] = p3Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p3Hand[i] = p3Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 4
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p4Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 4 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p4Hand[i] = p4Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p4Hand[i] = p4Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else if (players == 5)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 3
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p3Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 3 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p3Hand[i] = p3Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p3Hand[i] = p3Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 4
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p4Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 4 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p4Hand[i] = p4Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p4Hand[i] = p4Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 5
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p5Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 5 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p5Hand[i] = p5Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p5Hand[i] = p5Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else if (players == 6)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 3
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p3Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 3 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p3Hand[i] = p3Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p3Hand[i] = p3Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 4
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p4Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 4 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p4Hand[i] = p4Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p4Hand[i] = p4Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 5
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p5Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 5 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p5Hand[i] = p5Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p5Hand[i] = p5Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 6
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p6Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 6 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p6Hand[i] = p6Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p6Hand[i] = p6Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else if (players == 7)
	{
		// Player 1
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p1Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 1 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p1Hand[i] = p1Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p1Hand[i] = p1Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 2
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p2Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 2 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p2Hand[i] = p2Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p2Hand[i] = p2Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 3
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p3Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 3 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p3Hand[i] = p3Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p3Hand[i] = p3Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 4
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p4Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 4 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p4Hand[i] = p4Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p4Hand[i] = p4Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 5
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p5Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 5 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p5Hand[i] = p5Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p5Hand[i] = p5Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 6
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p6Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 6 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p6Hand[i] = p6Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p6Hand[i] = p6Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}

		// Player 7
		for (int i = 0; i<9; i++)
		{
			//Define local variables.
			int checkAce = CardValue(p7Hand[i]);
			int softOrHard;

			//If card in hand is an Ace prompt Player for input
			if (checkAce == 1 || checkAce == 11)
			{
				cout << "\n";
				cout << "\nWould Player 7 prefer the Ace to count as 1 or 11 points? (Enter 1 or 11): ";
				cin >> softOrHard;

				//If Player chooses 1
				if (softOrHard == 1)
				{
					if (checkAce == 11)
						p7Hand[i] = p7Hand[i] + 13;
				}

				//If Player chooses 11
				else if (softOrHard == 11)
				{
					if (checkAce == 1)
					{
						p7Hand[i] = p7Hand[i] - 13;
					}
				}

				// Input validation - If player doesn't input 1 or 11
				else if (softOrHard != 1 || softOrHard != 11)
				{
					//Clears input error flags and removes everything currently in the input buffer. 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					//Display error message and restart the function
					cout << "\nPlease enter the number 1 or 11." << endl;
					checkSoftOrHard(p1Hand, p2Hand, p3Hand, p4Hand, p5Hand, p6Hand, p7Hand, players);
				}
			}
		}
	}
	else
	{
		system("cls");
		cout << "UNKNOWN ERROR: Game Restart Required.\n";
	}
}

/* ==================================================
Name: checkSoftOrHardAI
Description: Logical test based on if Dealer has an Ace.
================================================== */
void checkSoftOrHardAI(int dHand[])
{
	// Loops through dealer's cards
	for (int i = 0; i<9; i++)
	{
		// If Ace card is found
		if (CardValue(dHand[i]) == 1 || CardValue(dHand[i]) == 11)
		{
			// If Ace == 11 and dealer busted
			if (CardValue(dHand[i]) == 11 && getHandValue(dHand) > 21)
			{
				dHand[i] = dHand[i] + 13;			// this converts it to a 1
			}
			// If Ace == 1 and score is less than blackjack
			else if (CardValue(dHand[i]) == 1 && getHandValue(dHand) < 21)
			{
				// Dealer attempts to avoid busting
				if (getHandValue(dHand) - CardValue(dHand[i]) + 11 < 22)
				{
					dHand[i] = dHand[i] - 13;		// this converts it to a 11
				}
			}
		}
	}
}

/* ==================================================
Name: updateEarnings
Description: Updates total earnings and rank for each user account
and updates text file database for alter retrieval.
================================================== */
void updateEarnings(class User(&accounts)[100], int numUsers, int &p1, int &p2, int &p3, int &p4,
	int &p5, int &p6, int &p7, int players, double winnings[], double bets[])
{
	// Updating total earnings for each user
	accounts[p1].earnings += (winnings[0]);
	// If Multi-player mode
	if (players > 1)
	{
		if (players == 2)
		{
			accounts[p2].earnings += (winnings[1]);
		}
		else if (players == 3)
		{
			accounts[p2].earnings += (winnings[1]);
			accounts[p3].earnings += (winnings[2]);
		}
		else if (players == 4)
		{
			accounts[p2].earnings += (winnings[1]);
			accounts[p3].earnings += (winnings[2]);
			accounts[p4].earnings += (winnings[3]);
		}
		else if (players == 5)
		{
			accounts[p2].earnings += (winnings[1]);
			accounts[p3].earnings += (winnings[2]);
			accounts[p4].earnings += (winnings[3]);
			accounts[p5].earnings += (winnings[4]);
		}
		else if (players == 6)
		{
			accounts[p2].earnings += (winnings[1]);
			accounts[p3].earnings += (winnings[2]);
			accounts[p4].earnings += (winnings[3]);
			accounts[p5].earnings += (winnings[4]);
			accounts[p6].earnings += (winnings[5]);
		}
		else if (players == 7)
		{
			accounts[p2].earnings += (winnings[1]);
			accounts[p3].earnings += (winnings[2]);
			accounts[p4].earnings += (winnings[3]);
			accounts[p5].earnings += (winnings[4]);
			accounts[p6].earnings += (winnings[5]);
			accounts[p7].earnings += (winnings[6]);
		}
	}
	// Updating rank for each user
	determineRank(accounts, numUsers);
}

/* ==================================================
Name: determineRank
Description: Stores each user's total earnings into single
array and determines rank of each user.
(If user has 0 earnings, their rank is 0)
================================================== */
void determineRank(class User(&accounts)[100], int numUsers)
{
	// Counter that holds temporary rank
	int tempRank = 1;

	// Sorting array
	selectionSort(accounts, SIZE);

	// Determining rank (assuming earnings != 0.00)
	for (int i = 0; i < numUsers; i++)
	{
		if (accounts[i].earnings == 0.00)
		{
			accounts[i].rank = 0;
			continue;
		}
		else
		{
			accounts[i].rank = tempRank;
			tempRank++;
		}
	}

}

/* ==================================================
Name: selectionSort
Description: Sorts array of doubles in descending order
================================================== */
void selectionSort(class User(&array)[100], int elems)
{
	int seek;      //array position currently being put in order
	int maxCount;  //location of largest value found
	double maxValue;  //holds the largest value found

	User tempArray[SIZE];

	for (seek = 0; seek < (elems - 1); seek++)  // outer loop performs the swap
												// and then increments seek
	{
		maxCount = seek;
		maxValue = array[seek].earnings;
		for (int index = seek + 1; index < elems; index++)
		{
			if (array[index].earnings > maxValue)
			{
				maxValue = array[index].earnings;
				maxCount = index;
			}
		}

		tempArray[seek] = array[maxCount];
		array[maxCount] = array[seek];
		array[seek].name = tempArray[seek].name;
		array[seek].email = tempArray[seek].email;
		array[seek].username = tempArray[seek].username;
		array[seek].pw = tempArray[seek].pw;
		array[seek].earnings = maxValue;
	}
}

/* ==================================================
Name: updateDB
Description: At the end of the program, the accounts
array of objects (AKA User Accounts) are stored to a
text file for further retrieval
================================================== */
void updateDB(class User(&accounts)[100], int numUsers)
{
	// Declaring file object (User accounts stored here)
	ofstream outputFile;

	// Opening text file
	outputFile.open("userDB.txt");

	outputFile << numUsers << endl;

	// Saving all changes to text file for later retrieval (Each time the file is overwritten)
	for (int i = 0; i < numUsers; i++)
	{
		outputFile << accounts[i].name << endl;
		outputFile << accounts[i].email << endl;
		outputFile << accounts[i].username << endl;
		outputFile << accounts[i].pw << endl;
		outputFile << accounts[i].earnings << endl;
		outputFile << accounts[i].rank << endl;
	}
	outputFile.close();
}

/* ==================================================
Name: displayResults
Description: Displays results at end of game
================================================== */
void displayResults(class User(&accounts)[100], int numUsers)
{
	// Clears screen and displays game results
	system("cls");
	cout << "\nLEADERBOARD:\n";
	cout << "------------\n\n";

	for (int i = 0; i < numUsers; i++)
	{
		if (i == 4 || i == 8 || i == 12 || i == 16 || i == 20 || i == 24 || i == 28 || i == 32 || i == 36 || i == 40)
		{
			cout << "(All Players start with $50)\n\n";
			system("pause");
			system("cls");
			cout << "\nLEADERBOARD (CONTINUED):\n";
			cout << "------------------------\n\n";
		}
		cout << "Username: " << accounts[i].username << endl;
		cout << "Total Earnings: $" << accounts[i].earnings << endl;
		cout << "Player Rank: " << accounts[i].rank << endl << endl;
	}
	cout << "(All Players start with $50)\n\n";
	system("pause");
}