#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

struct path {
	//path(string p, int r) : pathStr(p), risk(r) {}
	string pathStr;
	int risk;
};

string int_to_string( int Number )
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}

int main() {
	int M, N;
	cin >> M >> N;
	int table[M][N];
	for ( int m = 0; m < M; ++m ) {
		for ( int n = 0; n < N; ++n ) {
			cin >> table[m][n];
		}
	}
	
	path pathTable[M][N];
	for ( int n = 0; n < N; ++n ) {
		pathTable[M-1][n].pathStr = string("[")+int_to_string(M-1)+","+int_to_string(n)+"]";
		pathTable[M-1][n].risk = table[M-1][n];
	}
	
	for ( int m = M-2; m >= 0; --m ) {
		for ( int n = 0; n < N; ++n ) {
			long minRisk = numeric_limits<long>::max();
			int minRiskPath = -1;
			for ( int i = max(0, n-1); i <= min(N-1,n+1); ++i ) {
				if ( pathTable[m+1][i].risk < minRisk ) {
					minRisk = pathTable[m+1][i].risk;
					minRiskPath = i;
				}
			}
			pathTable[m][n].pathStr = string("[")+int_to_string(m)+","+int_to_string(n)+"]"+pathTable[m+1][minRiskPath].pathStr;
			pathTable[m][n].risk = table[m][n]+minRisk;
		}
	}
	
	long minRisk = numeric_limits<long>::max();
	int minRiskPath = -1;
	for ( int i = 0; i < N; ++i ) {
		if ( pathTable[0][i].risk < minRisk ) {
			minRisk = pathTable[0][i].risk;
			minRiskPath = i;
		}
	}
	
	cout << "Minimum risk path = " << pathTable[0][minRiskPath].pathStr << "\n"
		<< "Risks along the path = " << minRisk << "\n";
	
	return 0;
}
