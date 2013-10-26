#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <queue>
using namespace std;

void parse(istream& in, char& target, map<char, set<char>>& map)
{
	in >> target;
	char from, to;
	in >> from >> to;
	from = toupper(from);
	to = toupper(to);
	while (from != 'A' && to != 'A')
	{
		map[from].insert(to);
		map[to].insert(from);
		in >> from >> to;
		from = toupper(from);
		to = toupper(to);
	}
}

pair<int, string> solve(char target, map <char, set<char>>& map)
{
	if (map.count('F') == 0)
		return pair<int, string>(0, "");
	queue<string> frontier;
	frontier.emplace("F");
	int solCount = 0;
	string firstSol = "";
	while (!frontier.empty())
	{
		string currPath = frontier.front();
		frontier.pop();
		char last = currPath[currPath.length() - 1];
		if (last == target)
		{
			if (firstSol == "" || (firstSol.length() == currPath.length() && currPath < firstSol))
				firstSol = currPath;
			++solCount;
		}
		for (char c : map[last])
		{
			if (currPath.find(c) != string::npos) continue;
			frontier.emplace(currPath + c);
		}
	}
	return pair<int, string>(solCount, firstSol);
}

int main()
{
	map<char, set<char>> map;
	char target;
	parse(cin, target, map);
	pair<int, string> solution = solve(target, map);
	int solCount = solution.first;
	string firstSol = solution.second;
	unsigned solLength = solution.second.length();
	if (solCount > 0)
	{
		cout << "Total Routes: " << solCount << endl
			<< "Shortest Route Length: " << solLength << endl
			<< "Shortest Route after Sorting of Routes of length " << solLength << ": ";
		for (unsigned i = 0; i < solLength; ++i)
			cout << firstSol[i] << (i < (solLength - 1) ? ' ' : '\n');
	}
	else
		cout << "No Route Available from F to " << target << endl;
}