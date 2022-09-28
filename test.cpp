#include <iostream>
#include <limits>
#include <cstring>

using namespace std;

// https://isocpp.org/wiki/faq/input-output#istream-and-ignore

bool getChars(char arr[], int len) {
	while (true) {
		while (true) {
			cout << endl << "Need chars(" << len-1 << "): ";
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
		cout << "Got chars: " << arr << endl;
	}
	return true;
}

int main() {
	char chars[4] = { };
	getChars(chars, 4);
	//cout << endl << "Returned: " << chars; 

	return 0;
}

