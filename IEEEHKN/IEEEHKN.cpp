#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	uint32_t a,b;
	char comma;
	cin >> a >> comma >> b;
	uint32_t count = 0;
	
	for ( uint32_t i = a; i <= b; ++i ) {
		if ( i == 0 ) {
			++count;
			continue;
		}
		if (i % 2 == 0) continue;
		
		int j = 0, k = i;
		while (j < i) {
			j = j << 1;
			j += k % 2;
			k = k >> 1;
		}
		if (i == j) ++count;
	}
	cout << count << endl;
	return 0;
}
