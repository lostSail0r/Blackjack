#include <string>
using namespace std;

// Declaring Class for user accounts
class User
{
	// I know this may not be best practice but
	// accessing class attributes outside the 
	// function just made more sense to me.
public:
	string name;				// Name of user
	string email;				// User's email address
	string username;			// User's nickname/username
	string pw;					// User's Password
	double earnings;			// User's cumulative earnings in Blackjack
	int rank;					// How user ranks in terms of money earned
};