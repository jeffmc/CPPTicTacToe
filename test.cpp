#include <iostream>
#include <bitset>
#include <limits>

using namespace std;

int main() {
	char simp[3] = { };
	
	while (true) {
		cout << "I want 2 chars:";
		cin.get(simp, 3);
		if (cin.gcount() < 1 ) cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl << "Got:" << cin.gcount() << endl;
		for (int i=0;i<3;i++) cout << (bitset<8>) simp[i] << ' ' << (unsigned int) simp[i] << endl; 
		cout << endl;
		break;
	}

	return 0;
}
