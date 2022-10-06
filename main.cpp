// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This is a TicTacToe game written in C++. X will always start the game. Players specify moves using
// alphanumeric coordinates (a1, b3, c2, ...) If a playable move is left on the board, the game will continue. 
// At the end of each game, the win totals will be printed for gloating purposes.

#include <iostream>
#include <limits>
#include <cstring>
#include <bitset> // for compact state storage

using namespace std;

#define BOARD_SZ 3 
#define BOARD_SQ BOARD_SZ*BOARD_SZ

#define ALPHABET_STR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#if BOARD_SZ>9
	#error BOARD_SZ cannot be more than 9. // will cause formatting errors and many other unforeseen problems.
#endif

typedef unsigned char uchar; // makes code shorter.
typedef unsigned int uint;

// TTTCoord should contain an x and y value corresponding to a position within TTTState's board member.
struct TTTCoord {
	uchar x, y;
	TTTCoord() {
		this->x = numeric_limits<uchar>::max();
		this->y = numeric_limits<uchar>::max();
	}
	TTTCoord(const uchar x, const uchar y) {
		this->x = x;
		this->y = y;
	}
	void set(const uchar x, const uchar y) {
		this->x = x;
		this->y = y;
	};
	void fromIdx(const uint &idx) {
		this->x = idx % BOARD_SZ;
		this->y = idx / BOARD_SZ;
	}
};

typedef bitset<2> TTTCell;
class TTTState {
	public:
		TTTState();
		void getState(TTTCell &state, const TTTCoord &coord); 
		bool setState(const TTTCoord &coord, const TTTCell &p); // returns true if space was available and set, false if already occupied.
		uchar winner(); // returns 0b100 for CAT, 0b010 for X win, 0b001 O win, should return 0b000 if game not over.
	private:	
		bitset<BOARD_SQ*2> board; // each cell takes 2 bits, 00 for empty, 10 for X, 01 for O.
		void checkSets(uchar* sf, uint* sfb, int boff, int bStr, int cStr, uint nSets); // see func def for more info.
};


TTTState::TTTState() {
	for (int i=0;i<BOARD_SQ*2;i++) board[i] = 0;
}

// Pull state out of board into reference bitset<2>
void TTTState::getState(TTTCell &state, const TTTCoord &coord) {	
  uint i = (coord.x + coord.y*BOARD_SZ) * 2;
	state[0] = this->board[i];
	state[1] = this->board[i+1];
}

// Set state within bitset
bool TTTState::setState(const TTTCoord &coord, const TTTCell &p) {
  TTTCell temp; // get state at coord
	this->getState(temp,coord);
	if (temp != 0) return false; // if coord not empty, don't allow move.
	uint i = (coord.x + coord.y*BOARD_SZ) * 2;
	this->board[i]   = p[0];
	this->board[i+1] = p[1];
	return true; // move made successfully
}

// Utilize the checkSets() function to get set flags, based upon set flags determine if a winner has been found
// a tie game has occurred, or if there are significant moves left.
uchar TTTState::winner() {
	uint setCt = (BOARD_SZ+1)*2; // BOARD_SZ rows, BOARD_SZ cols, and 2 diagonals.
	uchar sf[setCt] = { }; // set flags array
	uint sfi = 0; // set flags index.
	checkSets(sf,&sfi,0,BOARD_SZ,1,BOARD_SZ); // rows
	checkSets(sf,&sfi,0,1,BOARD_SZ,BOARD_SZ); // cols
	checkSets(sf,&sfi,0,0,BOARD_SZ+1,1); // top left diag
	checkSets(sf,&sfi,BOARD_SZ-1,0,BOARD_SZ-1,1); // top right diag
  bool cat = true;
	for (uint i=0;i<setCt;i++) {
		switch (sf[i]) {
			case 0b100: // Empty and occupied by 1 player or nobody. 
			case 0b110: // Empty and single player (winnable)
			case 0b101:
				cat = false;
				break;
			case 0b010: // Set filled by single player (won)
			case 0b001:
				return sf[i];
				break;
			case 0b111: // A CAT set, already occupied fully by each player.
			case 0b011:
				break;
			default:
				cout << "Unknown set flag state: " << (uint)sf[i] << endl;
				break;
		}
	}
	return cat ? 0b100 : 0b000; // TODO: Fix
}

// sf    : set flags pointer
// sfb   : set flags pointer offset
// boff  : beginning offset (set)
// bStr  : beginning stride (between sets)
// cStr  : cell stride (between cells)
// nSets : number of sets
void TTTState::checkSets(uchar* sf, uint* sfb, int boff, int bStr, int cStr, uint nSets) {
	int b = boff; // beginning.
	TTTCell cell; // cell value
	TTTCoord coord; // cell coordinate

	for (int n=0;n<nSets;n++) { // each set
		int c = b; // cell index
		for (int i=0;i<BOARD_SZ;i++) { // set length should equal BOARD_SZ to win.
			coord.fromIdx(c);
			this->getState(cell, coord); // retreive state from board.
			switch (cell.to_ulong()) { // assign flags for sets based on cell states.
				case 0b00:	
					sf[*sfb+n] |= 0b100;
					break;
				case 0b10:	
					sf[*sfb+n] |= 0b010;
					break;
				case 0b01:	
					sf[*sfb+n] |= 0b001;
					break;
			}
			c += cStr; // increment cell index by cell stride
		}
		b += bStr; // increment next set beginning by beginning stride.
	}
	*sfb+=nSets;

}

// Override ostream << operator with a TTTState arg in order to easily print to std::cout.
ostream& operator<<(ostream &out, TTTState* ttt) { // Saw this on cplusplus.com forums
	const char* alpha = ALPHABET_STR;

	const int rowTitleWidth = 2; // Should be determined based upon BOARD_SZ ceil(log(BOARD_SZ,10));
	out.fill(' '); // Set fill char to whitespace
	out.width(rowTitleWidth+1); 
	out << ' '; // Will output width # of whitespace chars.
	for (int i=0;i<BOARD_SZ;i++) out << alpha[i] << ' '; // print header
	out << endl;

	TTTCoord coord;
	TTTCell cell;
	for (coord.y=0;coord.y<BOARD_SZ;coord.y++) {
		out.width(rowTitleWidth);
		out << right <<	(coord.y+1) << ' ';
		
		out.width(0); // set width back to 0.
		for (coord.x=0;coord.x<BOARD_SZ;coord.x++) {
			ttt->getState(cell, coord);
			switch (cell.to_ulong()) { // convert bitset<2> to ulong so usable in switch statement.
				case 0b00:
					out << '_';
					break;
				case 0b10:
					out << 'X';
					break;
				case 0b01:
					out << 'O';
					break;
				default:
					out << "GARBLED DATA: " << cell;
			}
			out << ' ';
		}
		out << endl;
	}
	return out;
};

bool getChars(char arr[], int len, const char* prompt) {
	while (true) { // Run until valid input is receieved.
		cout << endl << prompt; // Print provided prompt.
		cin.get(arr, len); // Get specified # of chars from stream.
		
		if (cin.peek() == '\n' && strlen(arr) == len-1) {
			cin.ignore(); // ignore the delimiting char.
			break; // Break once valid input is receieved.
		} else {
			cout << "Not valid!" << endl;
			cin.clear(); // Clear the flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore until the next delimiting char.
		}
	}
	return true; // Return true once valid input is receieved.
}

bool getCoord(TTTCoord& coord) {
		const char* prompt = "Enter a move: ";
		char buf[3] = {};
		const char alphabet[] = ALPHABET_STR;
		TTTCoord temp;

		while (true) {
			getChars(buf,3,prompt); // Fill buffer with 2 characters and ntc.
			
			temp = TTTCoord(); // initialize to invalid values
			for (int i=0;i<BOARD_SZ;i++) { // Assign x if alphabet index was found within BOARD_SZ
				if (alphabet[i] == buf[0]) { 
					temp.x = i;
					break;
				}
			}
			if (temp.x < 0 || temp.x >= BOARD_SZ) { // Boundchecking x...
					cout << "Not valid!" << endl;
					continue;
			}
			switch (buf[1]) { // Convert single digit chars >0 to integer counterparts.
				case '1': temp.y = 0; break;
				case '2': temp.y = 1; break;
				case '3': temp.y = 2; break;
				case '4': temp.y = 3; break;
				case '5': temp.y = 4; break;
				case '6': temp.y = 5; break;
				case '7': temp.y = 6; break;
				case '8': temp.y = 7; break;
				case '9': temp.y = 8; break;
				default: temp.y = numeric_limits<uchar>::max(); break;
			}
			if (temp.y < 0 || temp.y >= BOARD_SZ) { // Boundchecking y...
					cout << "Not valid!" << endl;
					continue;
			}
			break;
		}
		coord = temp; // Setting the ref coord to our temp val.
		return true; // Successfully got a valid coordinate.
}


int playGame() {
		unsigned char turn = 0b010; // X always starts	
		TTTState *ttt = new TTTState(); // Board
		TTTCoord coord = TTTCoord(); // Coordinate which to point moves (reassigned and used each time)

		while (true) {
			// Print current turn and state of board.
			cout << (turn == 0b010 ? 'X' : 'O') << "'s turn!" << endl;
			cout << endl << ttt;
			
			while (true) {
				// User input
				getCoord(coord);
				cout << endl;
				
				// Board manipulation
				if (ttt->setState(coord,turn)) break; // if move was made successfully, break out of decision loop
				cout << "Position already occupied. Try again!" << endl;
			}


			// Board logic checking
			int res = ttt->winner();
			switch (res) { // Evaluate the state returned by TTTState::winner()
				case 0b000:
					cout << "Still playing...";
					break;
				case 0b100:
					cout << "CAT game!";
					break;
				case 0b010:
					cout << "X won!";
					break;
				case 0b001:
					cout << "O won!";
					break;
				default:
					cout << "GARBLED RESULT!"; // Should never print (not sure how to throw exceptions yet)
					break;
			}	
			if (turn == 0b010) { // Switch turns
				turn = 0b001;
			} else {
				turn = 0b010;
			}
			// Game finished?
			if (res != 0b000) {
				cout << endl << ttt; // Print final board.
				return res; // Return value of player who won game or CAT.
			}
		}
		return 0b000; // shouldn't be reached!
}

bool playAgain() { // Return true if the user would like to play again.
	char arr[2];
	while (true) {
		getChars(arr,2,"Would you like to play again? (y/n) ");
		if (arr[0] == 'y' || arr[0] == 'Y') {
			return true;
		} else if (arr[0] == 'n' || arr[0] == 'N') {
			return false;
		} else {
			cout << "Not valid!" << endl;
		}
	}
	return false; // this should never be reached
}

struct WinTotals { // single-use structt
	int x,o,cat;
};

void printTotals(const WinTotals &totals) { // give win totals as constref and print the values
	cout << endl << "Results: " << endl;
	cout << "  X: " << totals.x << " wins" << endl;
	cout << "  O: " << totals.o <<  " wins" << endl;
	cout << "  " << totals.cat <<  " tied games" << endl;
}

int main() {
	WinTotals totals {0,0,0};
	while (true) { // Play until user explicitly quits (probably will never happen since my game is so enjoyable.)
		switch (playGame()) { // increment values based on game result	
			case 0b100: totals.cat++; break;
			case 0b010: totals.x++; break;
			case 0b001: totals.o++; break;
			default:
				cout << "GARBLED WIN RESPONSE!"; // Should never print (not sure how to throw exceptions yet)
				return 1;
		}
		printTotals(totals); // print values
		if (!playAgain()) break; // prompt user to play again and break if declined.
	}
	cout << "Thank you for playing!" << endl; // Always use your manners.
	return 0;
};
