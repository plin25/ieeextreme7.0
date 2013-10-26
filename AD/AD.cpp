#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cctype>
using namespace std;

string trim(const string& str,
                 const string& whitespace = " \r\t")
{
    int strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    int strEnd = str.find_last_not_of(whitespace);
    int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

int main() {
	string temp1, temp2, trtemp1;
	
	vector< pair<string,string> > words;
	cin >> temp1;
	int len = 0;
	while ( (trtemp1 = trim(temp1)) != "." ) {
		cin >> temp2;
		if ( trtemp1.size() > len )
			len = trtemp1.size();
		pair<string,string> wordPair(trtemp1, trim(temp2));
		words.push_back(wordPair);
		cin >> temp1;
	}
	
	vector< pair< pair<char,char>, pair<char,char> > > keys;
	for ( int i = 0; i < len; ++i ) {
		bool found = false;
		for ( int c1 = 1; c1 <= 26 && !found; ++c1 ) {
			for ( int n1 = 1; n1 < 10 && !found; ++n1 ) {
				for ( int c2 = 1; c2 <= 26 && !found; ++c2 ) {
					for ( int n2 = 1; n2 < 10 && !found; ++n2 ) {
						int base = c1*n1+c2*n2;
						int j = 0;
						for ( ; j < words.size(); ++j ) {
							if ( i >= words[j].first.size() )
								continue;
							int val = toupper(words[j].first[i])-'A'+1;
							char converted = (base+val) % 26 + 'A';
							if ( converted != toupper(words[j].second[i]) )
								break;
						}
						if ( j == words.size() ) {
							pair<char,int> K1(c1+'A'-1,n1+'0');
							pair<char,int> K2(c2+'A'-1,n2+'0');
							pair< pair<char,int>, pair<char,int> > KP(K1,K2);
							keys.push_back(KP);
							found = true;
						}
					}
				}
			}
		}
		if ( !found ) {
			pair<char,int> K1('?','?');
			pair<char,int> K2('?','?');
			pair< pair<char,int>, pair<char,int> > KP(K1,K2);
			keys.push_back(KP);
		}
	}
	for ( int i = 0; i < keys.size(); ++i )
		cout << keys[i].first.first;
	cout << endl;
	for ( int i = 0; i < keys.size(); ++i )
		cout << keys[i].first.second;
	cout << endl;
	for ( int i = 0; i < keys.size(); ++i )
		cout << keys[i].second.first;
	cout << endl;
	for ( int i = 0; i < keys.size(); ++i )
		cout << keys[i].second.second;
	cout << endl;
}
