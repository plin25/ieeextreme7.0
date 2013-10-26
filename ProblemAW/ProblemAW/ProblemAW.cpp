#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct CipherCombination {
	CipherCombination() : mapping() {}
	set<char> mapping[26];
	void insert(char c, char v) {
		mapping[c].insert(v);
	}
	vector<Cipher> possibleCiphers() {
		vector<Cipher> result;
		Cipher cipher;
	}
};
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

Cipher& combine(const vector<Cipher>& ciphers) {
	Cipher cipher;
	for (Cipher cipher : ciphers) {
		for (char c = 'A'; c <= 'Z'; ++c) {
			if (cipher [c] != 0)
		}
	}
}

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