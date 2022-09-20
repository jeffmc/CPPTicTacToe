// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This program should run tic tac toe. TODO: Finish this!

#include <iostream>

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
		bool set(int x, int y, unsigned char p); // returns true if space was available and set, false if already occupied.
	private:
		void checkSets(unsigned char* sf, int* sfb, int boff, int bStr, int cStr, int nSets);
};

TTTState::TTTState() {
	for (int i=0;i<BOARD_SQ;i++) board[i] = 0b100;
}

unsigned char TTTState::winner() {
	unsigned char sf[(BOARD_SZ+1)*2] = { };
	int sfi = 0;
	checkSets(sf,&sfi,0,BOARD_SZ,1,BOARD_SZ); // rows
	checkSets(sf,&sfi,0,1,BOARD_SZ,BOARD_SZ); // cols
	checkSets(sf,&sfi,0,0,BOARD_SZ+1,1); // top left
	checkSets(sf,&sfi,BOARD_SZ-1,0,BOARD_SZ-1,1); // top right

	return -1; // TODO: Fix
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

int main() {
	
	unsigned char turn = 1; // X always starts

	bool playing = true;
	char simp;
	while (playing) {
		cout << endl << (turn == 1 ? 'X' : 'O') << "'s turn!" << endl;

		TTTState *ttt = new TTTState();
		cout << ttt;

		int res = ttt->winner();
		switch (res) {
			case 0b100:
				cout << "Still playing...";
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

		cin >> simp;
		
		if (turn == 1) {
			turn = 2;
		} else {
			turn = 1;
		}

	}


	return 0;

};
