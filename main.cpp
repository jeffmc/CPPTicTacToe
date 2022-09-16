// Jeff McMillan 9-16-22 CPP Tic Tac Toe
// This program should run tic tac toe. TODO: Finish this!

#include <iostream>

using namespace std;

#define BOARD_SZ 3
#define BOARD_SQ BOARD_SZ*BOARD_SZ

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

ostream& operator<<(ostream &out, TTTState ttt) {
	out << "  a b c" << endl;
	for (int j=0;j<BOARD_SZ;j++) {
		out << (j+1) << ' ';
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
	
	TTTState ttt;
	cout << ttt;

	return 0;

};


