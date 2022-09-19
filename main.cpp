// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This program should run tic tac toe. TODO: Finish this!

#include <iostream>

using namespace std;

#define BOARD_SZ 10
#define BOARD_SQ BOARD_SZ*BOARD_SZ

#define ALPHABET_STR       "abcdefghijklmnopqrstuvwxyz"
#define ALPHABET_STR_UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#if BOARD_SZ>26
	#error BOARD_SZ cannot be more than 26.
#endif

class TTTState {
	public:
		unsigned char board[BOARD_SQ]; // 0 for empty, 1 for X, 2 for O.
		public:
		TTTState();
		bool gameFinished(); // returns true if no more moves can be made.
		unsigned char gameWinner(); // returns 1 for X win, 2 for O win, 0 for cat game, should return -1 if game not over.
		bool set(int x, int y, unsigned char p); // returns true if space was available and set, false if already occupied.
};

TTTState::TTTState() {
	for (int i=0;i<BOARD_SQ;i++) board[i] = 0;
}

// Override ostream << operator with a TTTState arg in order to easily print to std::cout.
ostream& operator<<(ostream &out, TTTState ttt) {
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
			unsigned char s = ttt.board[j*BOARD_SZ+i];
			switch (s) {
				case 0:
					out << '_';
					break;
				case 1:
					out << 'X';
					break;
				case 2:
					out << 'O';
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
		cout << *ttt;

		cin >> simp;
		
		if (turn == 1) {
			turn = 2;
		} else {
			turn = 1;
		}

	}


	return 0;

};


