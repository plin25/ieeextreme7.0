#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
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
	int t;
	cin >> t;
	
	int nums[t];
	int max = -1;
	for ( int j = 0; j < t; ++j ) {
		cin >> nums[j];
		if ( nums[j] > max )
			max = nums[j];
	}
	
	int positions[max+1];
	int position = 1;
	int dir = 1;
	for ( int i = 1; i <= max; ++i ) {
		positions[i] = position;
		if ( divByOrContains(i,7) )
			dir *= -1;
		position += dir;
		if ( position < 1 )
			position = 1337 - position;
		else if ( position > 1337 )
			position -= 1337;
	}
	
	for ( int j = 0; j < t; ++j )
		cout << positions[nums[j]] << endl;
	
	return 0;
}
