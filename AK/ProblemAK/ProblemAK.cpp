#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <vector>
using namespace std;

int main()
{
	int count, from, to;
	cin >> count;
	vector<int>* forward = (vector<int>*)calloc(count, sizeof(vector<int>));
	vector<int>* backward = (vector<int>*)calloc(count, sizeof(vector<int>));
	bool* notRoot = (bool*) calloc(count, sizeof(bool));
	while (cin >> from >> to) {
		forward[from - 1].push_back(to - 1);
		backward[to - 1].push_back(from - 1);
		notRoot[to - 1] = true;
	}

	deque<int> solution;

	int end;
	for (int i = 0; i < count; ++i) {
		if (!notRoot[i]) {
			end = i;
			break;
		}
	}
	while (forward[end].size() > 0) {
		if (forward[end].size() > 1 && solution.size() == 0) {
			solution.push_back(end);
			while (backward[end].size() == 1) {
				end = backward[end][0];
				solution.push_back(end);
			}
			end = solution.back();
		}
		end = forward[end][0];
	}
	if (solution.size() == 0) {
		solution.push_front(end);
		while (backward[end].size() == 1) {
			end = backward[end][0];
			solution.push_front(end);
		}
		end = solution.back();
	}

	sort(solution.begin(), solution.end());
	for (unsigned i = 0; i < solution.size(); ++i)
		cout << solution[i] + 1 << endl;
}