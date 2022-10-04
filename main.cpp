// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This program should run tic tac toe. TODO: Finish this!

#include <iostream>
#include <limits>
#include <cstring>
#include <bitset> // for compact state storage

using namespace std;

#define BOARD_SZ 3 
#define BOARD_SQ BOARD_SZ*BOARD_SZ

#define ALPHABET_STR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#if BOARD_SZ>26
	#error BOARD_SZ cannot be more than 26.
#endif

typedef unsigned char uchar;
typedef unsigned int uint;

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
		bool finished(); // returns true if no more moves can be made.
		uchar winner(); // returns 0b100 for CAT, 0b010 for X win, 0b001 O win, should return 0b000 if game not over.
		bool setState(const TTTCoord &coord, const TTTCell &p); // returns true if space was available and set, false if already occupied.
	private:	
		bitset<BOARD_SQ*2> board; // each cell takes 2 bits, 00 for empty, 10 for X, 01 for O.
		void checkSets(uchar* sf, uint* sfb, int boff, int bStr, int cStr, uint nSets);
};


TTTState::TTTState() {
	for (int i=0;i<BOARD_SQ*2;i++) board[i] = 0;
}

void TTTState::getState(TTTCell &state, const TTTCoord &coord) {	
  uint i = (coord.x + coord.y*BOARD_SZ) * 2;
	state[0] = this->board[i];
	state[1] = this->board[i+1];
}

bool TTTState::setState(const TTTCoord &coord, const TTTCell &p) {
  uint i = (coord.x + coord.y*BOARD_SZ) * 2;
	this->board[i]   = p[0];
	this->board[i+1] = p[1];
	return true;
}

uchar TTTState::winner() {
	uint setCt = (BOARD_SZ+1)*2;
	uchar sf[setCt] = { };
	uint sfi = 0;
	checkSets(sf,&sfi,0,BOARD_SZ,1,BOARD_SZ); // rows
	checkSets(sf,&sfi,0,1,BOARD_SZ,BOARD_SZ); // cols
	checkSets(sf,&sfi,0,0,BOARD_SZ+1,1); // top left
	checkSets(sf,&sfi,BOARD_SZ-1,0,BOARD_SZ-1,1); // top right
  bool cat = true;
	for (uint i=0;i<setCt;i++) {
		switch (sf[i]) {
			case 0b110:
			case 0b101:
				cat = false;
				break;
			case 0b010:
			case 0b001:
				return sf[i];
				break;
			case 0b100:
			case 0b111:
			case 0b011:
				break;
			default:
				cout << "Unknown set flag state: " << (uint)sf[i] << endl;
				break;
		}
	}
	return cat ? 0b100 : 0b000; // TODO: Fix
}

void TTTState::checkSets(uchar* sf, uint* sfb, int boff, int bStr, int cStr, uint nSets) {
	int b = boff; // beginning.
	TTTCell cell;
	TTTCoord coord;
	for (int n=0;n<nSets;n++) {
		int c = b;	
		for (int i=0;i<BOARD_SZ;i++) {
			coord.fromIdx(c);
			this->getState(cell, coord);
			switch (cell.to_ulong()) {
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
			c += cStr;	
		}
		b += bStr;
	}
	*sfb+=nSets;

}

// Override ostream << operator with a TTTState arg in order to easily print to std::cout.
ostream& operator<<(ostream &out, TTTState* ttt) {
	const char* alpha = ALPHABET_STR;

	const int rowTitleWidth = 4; // Should be determined based upon BOARD_SZ ceil(log(BOARD_SZ,10));
	out.fill(' ');
	out.width(rowTitleWidth+1);
	out << ' ';
	for (int i=0;i<BOARD_SZ;i++) out << alpha[i] << ' ';
	out << endl;

	TTTCoord coord;
	TTTCell cell;
	for (coord.y=0;coord.y<BOARD_SZ;coord.y++) {
		out.width(rowTitleWidth);
		out << right <<	(coord.y+1) << ' ';
		
		out.width(0);
		for (coord.x=0;coord.x<BOARD_SZ;coord.x++) {
			ttt->getState(cell, coord);
			switch (cell.to_ulong()) {
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
	while (true) {
		cout << endl << prompt; 
		cin.get(arr, len);
		
		if (cin.peek() == '\n' && strlen(arr) == len-1) {
			cin.ignore();
			break;
		} else {
			cout << "Not valid!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return true;
}

bool getCoord(TTTCoord& coord) {
		const char* prompt = "Enter a move: ";
		char buf[3] = {};
		const char alphabet[] = ALPHABET_STR;
		TTTCoord temp = TTTCoord();

		while (true) {
			getChars(buf,3,prompt);
			for (int i=0;i<BOARD_SZ;i++) {
				if (alphabet[i] == buf[0]) {
					temp.x = i;
					break;
				}
			}
			if (temp.x < 0 || temp.x >= BOARD_SZ) {
					cout << "Not valid!" << endl;
					continue;
			}
			switch (buf[1]) {
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
			if (temp.y < 0 || temp.y >= BOARD_SZ) {
					cout << "Not valid!" << endl;
					continue;
			}
			break;
		}
		coord = temp;
		return true;
}


int playGame() {
		unsigned char turn = 0b010; // X always starts	
		bool playing = true;	
		TTTState *ttt = new TTTState();
		TTTCoord coord = TTTCoord();

		while (playing) {
			cout << (turn == 0b010 ? 'X' : 'O') << "'s turn!" << endl;
			cout << endl << ttt;
			
			// User input
			getCoord(coord);

			// Board manipulation
			ttt->setState(coord,turn);

			// Board logic checking
			int res = ttt->winner();
			switch (res) {
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
					cout << "GARBLED RESULT!";
					break;
			}	
			cout << endl;	
			if (turn == 0b010) {
				turn = 0b001;
			} else {
				turn = 0b010;
			}
			// Game finished?
			if (res != 0b000) {
				cout << endl << ttt;
				return res; // Return value of player who won game or CAT.
			}
		}
		return 0b000; // shouldn't be returned!
}

int main() {
	bool playing = true;
	while (playing) {
		int res = playGame();
		break;
	}

	return 0;

};
