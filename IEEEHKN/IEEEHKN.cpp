#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

uint32_t pow2(uint32_t p) {
	if ( p < 1 ) {
		return 1;
	}
	uint32_t res = 2;
	for ( uint32_t i = 1; i < p; ++i ) {
		res *= 2;
	}
	return res;
}

uint32_t log2(uint32_t p) {
	uint32_t res = 0;
	while ( p > 1 ) {
		++res;
		p /= 2;
	}
}

int main() {
	uint32_t a,b;
	char comma;
	cin >> a >> comma >> b;
	uint32_t count = 0;
	for ( uint32_t i = a; i <= b; ++i ) {
		if ( i == 0 )
			++count;
		else if ( i % 2 != 0 ) {
			int highBit = log2(i);
			uint32_t twohigh = pow2(highBit);
			uint32_t j = i-1-twohigh;
			--highBit;
			int lowBit = 1;
			bool match = true;
			while ( lowBit < highBit ) {
				// cout << j << endl;
				bool even = ( j % pow2(lowBit) ) == 0;
				twohigh = pow2(highBit);
				if ( j >= twohigh && ! even ) {
					j -= twohigh;
				} else if ( j < twohigh && even ){
					// Do nothing
				} else {
					match = false;
					break;
				}
				++lowBit;
				--highBit;
			}
			if ( match ) {
				++count;
			}
		}
	}
	cout << count << endl;
	return 0;
}
