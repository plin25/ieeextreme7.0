#include <iostream>

using namespace std;

bool divByOrContains(int num, int x) {
	if ( num % x == 0 )
		return true;
	
	while ( num > 0 ) {
		if ( num % 10 == x )
			return true;
		num /= 10;
	}
	return false;
}

int main() {
	for ( int i = 1; i < 40; ++i ) {
		cout << i << " " << divByOrContains(i,7) << endl;
	}
}