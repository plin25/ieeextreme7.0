#include <cmath>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;

vector<string> allSeqs;

class ATree {
public:
	ATree(string s) {
		str = s;
		parent = NULL;
	}
	ATree(string s, ATree * p) {
		str = s;
		parent = p;
	}
	// pair< string, vector< ATree * > > children;
	string str;
	ATree * parent;
};

// This is because they have fucking \r\n's
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

bool increment(int * bins, int J) {
	bool carry;
	int pos = J;
	while ( pos > 0 && bins[pos] == 0 ) pos--;
	if ( pos == 0 ) {
		return false;
	} else {
		bins[pos-1]++;
		int sum = -1;
		for ( int i = J; i >= pos; i-- ) {
			sum += bins[i];
			bins[i] = 0;
		}
		bins[J] = sum;
	}
	return true;
}

vector<string> ancestorStrings(ATree * pT) {
	vector<string> result;
	ATree * ancestor = pT;
	while ( ancestor != NULL && ancestor->str != "" ) {
		// cout << ancestor->str << endl;
		result.push_back(ancestor->str);
		ancestor = ancestor->parent;
	}
	return result;
}

bool generateAlignments(ATree * pT, string * sequences, int maxChars,
						int seqInd, int N) {
	if ( seqInd >= N ) {
		vector<string> ancestorStrs = ancestorStrings(pT);
		bool reject = false;
		for ( int i = 0; i < ancestorStrs[0].size(); ++i ) {
			if ( ancestorStrs[0][i] != '-' )
				continue;
			bool allDash = true;
			bool compared = false;
			for ( int j = 1; j < ancestorStrs.size(); ++j ) {
				compared = true;
				if ( ancestorStrs[j][i] != '-' )
					allDash = false;
			}
			if ( compared && allDash ) {
				reject = true;
				break;
			}
		}
		if ( !reject ) {
			ostringstream oss;
			for ( int i = ancestorStrs.size() - 1; i >= 0; --i ) {
				oss << ancestorStrs[i] << "\n";
			}
			allSeqs.push_back(oss.str());
		}
		return false;
	}
	
	if ( pT->str != "" && sequences[seqInd].size() > pT->str.size() ) {
		return false;
	}
		
	string sequence = sequences[seqInd];
	int J = sequence.size(); // Number of chars in sequence
	int maxDashes = pT->str == "" ? maxChars - J : pT->str.size() - J;
	int minDashes = pT->str == "" ? 0 : maxDashes;
	for ( int dashes = minDashes; dashes <= maxDashes; ++dashes ) {
		int bins[J+1];
		for ( int i = 0; i < J; ++i ) {
			bins[i] = 0;
		}
		bins[J] = dashes;
		do {
			// cout << "cp" << seqInd << endl;
			ostringstream oss;
			for ( int i = 0; i < J; ++i ) {
				for ( int j = 0; j < bins[i]; ++j )
					oss << "-";
				oss << sequence[i];
			}
			for ( int j = 0; j < bins[J]; ++j )
				oss << "-";
			string out = oss.str();
/*
			ATree * ancestor = pT;
			bool allMatch = true;
			bool compared = false''
			while ( ancestor != NULL && ancestor->str != "" ) {
				compared = true;
				string ancStr = ancestor->str;
				bool match = true;
				for ( int i = 0; i < maxChars; ++i ) {
					if ( (ancStr[i] == '-' && out[i] != '-') ||
							(ancStr[i] == '-' && out[i] != '-') ) {
						match = false;
						break;
					}
				}
				if ( !match ) {
					allMatch = false;
					break;
				}
				ancestor = ancestor->parent;
			}
			if ( compared && allMatch ) {
				cout << pT->str << " " << out << "bad" << endl;
				continue;
			}
*/
			// cout << out << endl;
			// cout << pT->str << " " << out << " " << reject << endl;
			// if ( !reject ) {
				ATree * nT = new ATree(out, pT);
				// pT->trees().push_back(nT);
				// cout << pT->trees()[pT->trees().size()-1]->str << endl;
				generateAlignments(nT,
							sequences, maxChars, seqInd+1, N);
			// }
		} while ( increment(bins, J) );
	}
	return true;
}

int main() {
    int N, K;
	string temp;
	getline(cin, temp);
	istringstream iss(temp);
    iss >> N;
	// cin.ignore(); // Clears out the newline.
    string sequences[N];
	int maxChars = 0;
    for ( int i = 0; i < N; ++i ) {
		getline(cin, temp);
		sequences[i] = trim(temp);
		maxChars += sequences[i].size();
    }
	cin >> K;
	int positions[K];
	for ( int i = 0; i < K; ++i ) {
		cin >> positions[i];
	}
	ATree * T = new ATree("");
	generateAlignments(T, sequences, maxChars, 0, N);
/*	for ( int i = 0; i < T->trees().size(); ++i ) {
		ATree * S = T->trees()[i];
		for ( int j = 0; j < S->trees().size(); ++j ) {
			cout << S->str << endl;
			cout << S->trees()[j]->str << endl;
			cout << "-------" << endl;
		}
	}
*/
/*
	for ( int i = 0; i < allSeqs.size(); ++i ) {
		cout << allSeqs[i] << "____________\n";
	}
*/
	cout << "Possible Alignments: " << allSeqs.size() << endl;
	for ( int i = 0; i < K; ++i ) {
		int pos = positions[i];
		if ( 0 < pos && pos <= allSeqs.size() ) {
			cout << "Alignment at Position: " << pos << "\n" << allSeqs[pos-1];
		} else {
			cout << "There is no alignment at position: " << pos << endl;
		}
	}
	
    return 0;
}
