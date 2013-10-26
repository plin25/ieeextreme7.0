#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct Cipher {
	Cipher() : mapping() {}
	char mapping[26];
	char& operator [](const char index) {
		return mapping[index - 'A'];
	}
	char operator [](const char index) const{
		return mapping[index - 'A'];
	}

	//Does not test equality; tests compatibility
	bool operator==(const Cipher& other) const {
		for (int i = 0; i < 26; ++i)
			if (mapping[i] != 0 && other.mapping[i] != 0 && mapping[i] != other.mapping[i])
				return false;
		return true;
	}

};

class Dictionary {
private:
	map<int, set<string>> childrenOfLength;
public:
	set<Cipher> findMatches(const string& cipheredWord) {
		set<Cipher> ciphers;
		unsigned length = cipheredWord.length();
		for (string s : childrenOfLength[length]) {
			Cipher cipher;
			for (int i = 0; i < length; ++i) {
				if (cipher[cipheredWord[i]] != 0 && cipher[cipheredWord[i]] != s[i])
					continue;
				cipher[cipheredWord[i]] = s[i];
			}
			ciphers.insert(cipher);
		}
		return ciphers;
	}

	void insert(const string& word) {
		childrenOfLength[word.length()].insert(word);
	}
};



int main()
{
	Dictionary dictionary;
	int lines;
	cin >> lines;
	string input;
	for (int i = 0; i < lines; ++i) {
		cin >> input;
		dictionary.insert(input);
	}
	vector<string> words;
	while (cin >> input)
		words.push_back(input);


}