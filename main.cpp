// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This program should run tic tac toe. TODO: Finish this!

#include <iostream>
#include <limits>

using namespace std;

#define BOARD_SZ 3 
#define BOARD_SQ BOARD_SZ*BOARD_SZ

#define ALPHABET_STR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#if BOARD_SZ>26
	#error BOARD_SZ cannot be more than 26.
#endif

class TTTState {
	public:
		unsigned char board[BOARD_SQ]; // 0b100 for empty, 0b010 for X, 0b001 for O.
		public:
		TTTState();
		bool finished(); // returns true if no more moves can be made.
		unsigned char winner(); // returns 0b100 for CAT, 0b010 for X win, 0b001 O win, should return 0b000 if game not over.
		bool setPiece(int x, int y, unsigned char p); // returns true if space was available and set, false if already occupied.
	private:
		void checkSets(unsigned char* sf, int* sfb, int boff, int bStr, int cStr, int nSets);
};


TTTState::TTTState() {
	for (int i=0;i<BOARD_SQ;i++) board[i] = 0b100;
}

bool TTTState::setPiece(int x, int y, unsigned char p) {
	this->board[x + y * BOARD_SZ] = p; 
	return true;
}

unsigned char TTTState::winner() {
	int setCt = (BOARD_SZ+1)*2;
	unsigned char sf[setCt] = { };
	int sfi = 0;
	checkSets(sf,&sfi,0,BOARD_SZ,1,BOARD_SZ); // rows
	checkSets(sf,&sfi,0,1,BOARD_SZ,BOARD_SZ); // cols
	checkSets(sf,&sfi,0,0,BOARD_SZ+1,1); // top left
	checkSets(sf,&sfi,BOARD_SZ-1,0,BOARD_SZ-1,1); // top right
  bool cat = true;
	for (int i=0;i<setCt;i++) {
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
				cout << "Unknown set flag state: " << (int)sf[i] << endl;
				break;
		}
	}
	return cat ? 0b100 : 0b000; // TODO: Fix
}

void TTTState::checkSets(unsigned char* sf, int* sfb, int boff, int bStr, int cStr, int nSets) {
	int b = boff; // beginning.
	for (int n=0;n<nSets;n++) {
		int c = b;	
		for (int i=0;i<BOARD_SZ;i++) {
			// cout << c << ' ';
			sf[*sfb+n] |= this->board[c];
			c += cStr;	
		}
		// cout << endl;
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
	
	for (int j=0;j<BOARD_SZ;j++) {
		out.width(4);
		out << right <<	(j+1) << ' ';
		
		out.width(0);
		for (int i=0;i<BOARD_SZ;i++) {
			unsigned char s = ttt->board[j*BOARD_SZ+i];
			switch (s) {
				case 0b100:
					out << '_';
					break;
				case 0b010:
					out << 'X';
					break;
				case 0b001:
					out << 'O';
					break;
				default:
					out << "GARBLED DATA!!!!";
			}
			out << ' ';
		}
		out << endl;
	}
	return out;
};

int playGame() {
		char simp[3] = { };
		unsigned char turn = 0b010; // X always starts	
		bool playing = true;	
		TTTState *ttt = new TTTState();

		while (playing) {
			cout << (turn == 0b010 ? 'X' : 'O') << "'s turn!" << endl;
			cout << endl << ttt;
			
			// User input
			cin.getline(simp, 3);
			const char alphabet[] = ALPHABET_STR;
			int x = -1, y = -1;
			for (int i=0;i<BOARD_SZ;i++) {
				if (alphabet[i] == simp[0]) {
					x = i;
					break;
				}
			}
			switch (simp[1]) {
				case '1': y = 0; break;
				case '2': y = 1; break;
				case '3': y = 2; break;
				case '4': y = 3; break;
				case '5': y = 4; break;
				case '6': y = 5; break;
				case '7': y = 6; break;
				case '8': y = 7; break;
				case '9': y = 8; break;
				default: cout << "ERRORRR!!!!" << endl;
				break;
			}
			// Board manipulation
			ttt->setPiece(x,y,turn);

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
