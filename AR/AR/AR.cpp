#include <algorithm>
#include <climits>
#include <deque>
#include <iostream>
using namespace std;

struct SpecialShow {
	SpecialShow(int duration, int priority)
		: duration(duration), priority(priority) {}
	int duration;
	int priority;
};

struct Schedule {

	Schedule(deque<int> normalShows, deque<SpecialShow> specialShows) : normalShows(normalShows), specialShows(specialShows) {}
	deque<int> normalShows;
	deque<SpecialShow> specialShows;

	pair<int, int> overlap() {
		pair<int, int> overlap(0, INT_MAX);
		int i = 0;
		for (int j = 0; j < specialShows.size(); ++j) {
			int timeLeft = specialShows[j].duration;
			while (timeLeft > 0 && i < normalShows.size()) {
				timeLeft -= normalShows[i++];
			}
			if (timeLeft < 0 && specialShows[j].priority < overlap.second) {
				overlap.first = -timeLeft;
				overlap.second = specialShows[j].priority;
			}
		}
		if (overlap.first == 5 && overlap.second == 2)
			cout << "found it";
		return overlap;
	}

	bool operator< (Schedule rhs) {
		pair<int, int> lhsOverlap, rhsOverlap;
		lhsOverlap = overlap();
		rhsOverlap = rhs.overlap();/*
		for (int i = 0; i < normalShows.size(); ++i)
			cout << normalShows[i] << " ";
		cout << ": " << lhsOverlap.first << " at priority " << lhsOverlap.second << endl;
		for (int i = 0; i < rhs.normalShows.size(); ++i)
			cout << rhs.normalShows[i] << " ";
		cout << ": " << rhsOverlap.first << " at priority " << rhsOverlap.second << endl;*/
		if (lhsOverlap.second != rhsOverlap.second)
			return lhsOverlap.second > rhsOverlap.second;
		else
			return lhsOverlap.first < rhsOverlap.first;
	}
};

bool compare(Schedule lhs, Schedule rhs) {
	return lhs < rhs;
}

deque<deque<int>> permute(deque<int>::iterator start, deque<int>::iterator end) {
	deque<deque<int>> result;
	sort(start, end);
	//reverse(start, end);
	result.push_back(deque<int>(start, end));
	while (next_permutation(start, end))
		result.push_back(deque<int>(start, end));
	return result;
	/*
	if (start == end) return deque<deque<int>>();
	deque<int>::iterator second = start;
	++second;
	deque<deque<int>> smallerSet = permute(second, end);
	deque<deque<int>> newSet;
	if (smallerSet.size() == 0) {
		newSet.push_back(deque<int>(1, *start));
		return newSet;
	}

	for (deque<deque<int>>::iterator i = smallerSet.begin(); i != smallerSet.end(); ++i) {
		for (deque<int>::iterator j = i->begin(); j != i->end(); ++j) {
			deque<int> newOne(i->begin(), j);
			newOne.push_back(*start);
			newOne.insert(newOne.end(), j, i->end());
			newSet.push_back(newOne);
		}
		deque<int> newOne(i->begin(), i->end());
		newOne.push_back(*start);
		newSet.push_back(newOne);
	}
	return newSet;*/
}

deque<Schedule> permute(deque<int> normalShows, deque<SpecialShow> specialShows) {
	deque<Schedule> schedules;
	deque<deque<int>> permutations = permute(normalShows.begin(), normalShows.end());
	for (deque<int> permutation : permutations) {
		schedules.push_back(Schedule(permutation, specialShows));
	}
	return schedules;
}


int main()
{
	int shows, duration, priority;
	deque<int> normalShows;
	deque<SpecialShow> specialShows;
	cin >> shows;
	for (int i = 0; i < shows; ++i) {
		cin >> duration;
		normalShows.push_back(duration);
	}
	cin >> shows;
	for (int i = 0; i < shows; ++i) {
		cin >> duration >> priority;
		specialShows.push_back(SpecialShow(duration, priority));
	}

	//If this works I'll be shocked
	deque<Schedule> schedules = permute(normalShows, specialShows);
	sort(schedules.begin(), schedules.end(), compare);
	Schedule solution = schedules.front();

	for (int i = 0; i < solution.normalShows.size(); ++i)
		cout << solution.normalShows[i] << (i == solution.normalShows.size() - 1 ? '\n' : ' ');
	pair<int, int> thing = solution.overlap();
	if (thing.second == INT_MAX)
		cout << "Overlap Zero" << endl;
	else
		cout << "Overlap " << thing.first << " of Level " << thing.second << endl;
}