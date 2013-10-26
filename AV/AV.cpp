#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
#include <queue>
#include <limits>
using namespace std;

const double MAX_D = numeric_limits<double>::max();

double dists[11][11] = {
		{MAX_D,MAX_D,MAX_D,MAX_D,MAX_D,5,MAX_D,MAX_D,MAX_D,MAX_D,MAX_D},
		{MAX_D,MAX_D,MAX_D,5,4.5,4,4.5,5,MAX_D,MAX_D,MAX_D},
		{MAX_D,MAX_D,4.5,4,3.5,3,3.5,4,4.5,MAX_D,MAX_D},
		{MAX_D,5,4,3,2.5,2,2.5,3,4,5,MAX_D},
		{MAX_D,4.5,3.5,2.5,1.5,1,1.5,2.5,3.5,4.5,MAX_D},
		{5,4,3,2,1,0,1,2,3,4,5},
		{MAX_D,4.5,3.5,2.5,1.5,1,1.5,2.5,3.5,4.5,MAX_D},
		{MAX_D,5,4,3,2.5,2,2.5,3,4,5,MAX_D},
		{MAX_D,MAX_D,4.5,4,3.5,3,3.5,4,4.5,MAX_D,MAX_D},
		{MAX_D,MAX_D,MAX_D,5,4.5,4,4.5,5,MAX_D,MAX_D,MAX_D},
		{MAX_D,MAX_D,MAX_D,MAX_D,MAX_D,5,MAX_D,MAX_D,MAX_D,MAX_D,MAX_D},
	};

typedef pair<int,int> coord;

int main() {
	int M, N;
	cin >> M >> N;
	char map[M][N];
	coord start(-1,-1);
	coord end(-1,-1);
	for ( int m = 0; m < M; ++m ) {
		for ( int n = 0; n < N; ++n ) {
			cin >> map[m][n];
			switch(map[m][n]) {
				case 'S':
					start.first = m;
					start.second = n;
					break;
				case 'E':
					end.first = m;
					end.second = n;
					break;
				// default:
			}
		}
	}
	
	double dist[M][N];
	for ( int m = 0; m < M; ++m ) {
		for ( int n = 0; n < N; ++n ) {
			dist[m][n] = MAX_D;
		}
	}
	dist[end.first][end.second] = 0;
	queue<coord> q;
	q.push(end);
	while ( ! q.empty() ) {
		coord curr = q.front(); q.pop();
		for ( int i = -5; i <= 5 ; ++i ) {
			int m = curr.first + i;
			if ( m < 0 || m >= M )
				continue;
			for ( int j = -5; j <= 5; ++j ) {
				int n = curr.second + j;
				if ( n < 0 || n >= N )
					continue;
				if ( map[m][n] == 'D' )
					continue;
				double distance = dists[i+5][j+5];
				if ( dist[curr.first][curr.second] + distance < dist[m][n] ) {
					dist[m][n] = dist[curr.first][curr.second] + distance;
					// cout << m << " " << n << " " << dist[m][n] << endl;
					coord next(m,n);
					q.push(next);
				}
			}
		}
	}
	double minDist = dist[start.first][start.second];
	if ( minDist < MAX_D )
		cout << minDist << endl;
	else
		cout << "IMPOSSIBLE" << endl;
	
	return 0;
}
