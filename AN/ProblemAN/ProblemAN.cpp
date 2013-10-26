#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


struct Plaintext {
	int k1, k2, k3, blocklength;
	const string& charlist;

	void set(int newK1, int newK2, int newK3, int newBlocklength) {
		k1 = newK1;
		k2 = newK2;
		k3 = newK3;
		blocklength = newBlocklength;
	}

	Plaintext(const string& charlist, int k1 = 0, int k2 = 0, int k3 = 0, int blocklength = 1)
		: charlist(charlist), k1(k1), k2(k2), k3(k3), blocklength(blocklength) {}

	unsigned length() { return charlist.length(); }

	char shift(char in, int shift) {
		in += shift;
		while (in < 'A')
			in += 26;
		while (in > 'Z') {
			in -= 26;
		}
		return in;
	}

	char operator[](int index) {
		switch ((index / blocklength) % 3)
		{
		case 0:
			return shift(charlist[index], k1);
		case 1:
			return shift(charlist[index], k2);
		case 2:
			return shift(charlist[index], k3);
		}
	}

	string getText() {
		stringstream ss;
		for (unsigned i = 0; i < charlist.length(); ++i) {
			ss << this->operator [](i);
		}
		return ss.str();
	}
};

ostream& operator<<(ostream& out, Plaintext text) {
	out << text.getText();
	return out;
}

class Dictionary {
private:
	struct DictionaryNode {
		DictionaryNode() : children(), isWord(false) {}
		DictionaryNode* child(char c) { return children[c - 'A']; }
		bool isWord;
		DictionaryNode* children[26];
	};
	struct ValidationNode {
		ValidationNode(DictionaryNode* node, unsigned index) : node(node), index(index) {}
		void increment(char c) { node = node->child(c); ++index; }
		DictionaryNode* node;
		unsigned index;
	};
	DictionaryNode* children[26];
public:
	Dictionary() : children() {}

	//A string is valid if it is only made of words in the dictionary
	bool validate(Plaintext text) {
		queue<ValidationNode*> words;
		unsigned length = text.length();
		DictionaryNode* startNode = children[text[0] - 'A'];
		words.push(new ValidationNode(startNode, 0));
		while (!words.empty()) {
			ValidationNode* current = words.front();
			words.pop();
			if (current->node == NULL) //This is no longer a valid substring
				continue;
			if (current->index == length) //We've reached the end of the string and it's still valid
				return true;
			if (current->node->isWord) //It's possible that this is the end of a word in the plaintext, in which case we want to look for another word
				words.push(new ValidationNode(children[text[current->index + 1] - 'A'], current->index + 1));

			//It's also possible that it isn't, in which case we want to see if it's part of a longer word
			current->index += 1;
			current->node = current->node->child(text[current->index]);
			words.push(current);
		}
		return false;
	}
	void insert(const string& word) {
		if (word.length() == 0) return;
		DictionaryNode* child = children[word[0] - 'A'];
		if (child == NULL) child = children[word[0] - 'A'] = new DictionaryNode();
		unsigned index = 1;
		while (index < word.length()) {
			DictionaryNode* nextChild = child->children[word[index] - 'A'];
			if (nextChild == NULL) {
				nextChild = child->children[word[index] - 'A'] = new DictionaryNode();
			}
			child = nextChild;
			++index;
		}
		child->isWord = true;
	}
};

Dictionary test(int dictionarySize, const string& dictionaryValues) {
	Dictionary dictionary;
	stringstream ss(dictionaryValues);
	string out;
	for (int i = 0; i < dictionarySize; ++i) {
		ss >> out;
		dictionary.insert(out);
	}
	return dictionary;
}

int main()
{
	string ciphertext, input;
	int dictionarySize = 500;
	cin >> ciphertext >> dictionarySize;
	Dictionary dictionary;
	while (cin >> input)
		dictionary.insert(input);

	Plaintext text(ciphertext, 0, 0, 0, 1);
	for (int k3 = 0; k3 < 26; ++k3) {
		for (int k2 = 0; k2 < 26; ++k2) {
			for (int k1 = 0; k1 < 26; ++k1) {
				for (int blocklength = 1; blocklength <= 10; ++blocklength) {
					text.set(k1, k2, k3, blocklength);
					if (dictionary.validate(text)) {
						cout << blocklength << endl
							<< (k1 == 0 ? 0 : 26 - k1) << endl
							<< (k2 == 0 ? 0 : 26 - k2) << endl
							<< (k3 == 0 ? 0 : 26 - k3) << endl
							<< text << endl;
						return 0;
					}
				}
			}
		}
	}
}