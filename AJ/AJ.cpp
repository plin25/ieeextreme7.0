#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
	int N;
	cin >> N;
	long hills[N];
	for ( int i = 0; i < N; ++i ) {
		cin >> hills[i];
	}
	long vel[N+1];
	vel[N] = 0;
	long max_vel = 0;
	for ( int i = N-1; i >= 0; --i ) {
		vel[i] = vel[i+1] + hills[i];
		if ( vel[i] > max_vel )
			max_vel = vel[i];
	}
	cout << max_vel << endl;
	return 0;
}
