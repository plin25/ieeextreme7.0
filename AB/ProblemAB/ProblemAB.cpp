#include <iostream>
#include <string>
#include <vector>
using namespace std;

//typedef map<char, int> valuemap;
class ValueMap
{
private:
	static const int a = 1;
	int b;
	int c;
	int d;
	int e;
public:
	ValueMap(int b = 1, int c = 1, int d = 1, int e = 1)
		: b(b), c(c), d(d), e(e) {}
	bool increment()
	{
		if (++e <= 5) return true;
		e = 1;
		if (++d <= 5) return true;
		d = 1;
		if (++c <= 5) return true;
		c = 1;
		return (++b <= 5);
	}
	bool twoDifferentValues()
	{
		int first = b, second = 0;
		if (c != first) second = c;
		if (d != first)
			if (second == 0)
				second = d;
			else
				if (second != d)
					return false;
		if (e != first)
			if (second == 0)
				second = e;
			else
				if (second != e)
					return false;
		return second != 0;
	}
	int operator[](char in)
	{
		switch (in)
		{
		case 'A':
			return a;
		case 'B':
			return b;
		case 'C':
			return c;
		case 'D':
			return d;
		case 'E':
			return e;
		default:
			return -1;
		}
	}
};

bool wasFined(const string& in, ValueMap values)
{
	int sum = 0;
	for (char c : in)
		sum += values[c];
	return sum%5!=0;
}

enum ValidationCode
{
	OK,
	TOO_LONG,
	ERR
};

ValidationCode validate(vector<string>& in)
{
	if (in.size() > 31) return TOO_LONG;
	ValueMap values;

	bool twoValuesAndFined = false;
	bool twoValuesAndNotFined = false;
	bool notTwoValuesAndFined = false;
	bool notTwoValuesAndNotFined = false;
	do
	{
		bool fined = false;
		for (string day : in)
			fined ^= wasFined(day, values);

		if (fined)
		{
			if (values.twoDifferentValues())
				twoValuesAndFined = true;
			else
				notTwoValuesAndFined = true;
		}
		else
		{
			if (values.twoDifferentValues())
				twoValuesAndNotFined = true;
			else
				notTwoValuesAndNotFined = true;
		}

		if (twoValuesAndFined && notTwoValuesAndFined)
			return ERR;
		if (twoValuesAndNotFined && notTwoValuesAndNotFined)
			return ERR;
	} while (values.increment());
	return OK;
}

int main()
{
	vector<string> input;
	string in;
	while (getline(cin, in) && in != "")
		input.push_back(in);
	switch (validate(input))
	{
	case OK:
		cout << "Ok" << endl;
		break;
	case TOO_LONG:
		cout << "Too long" << endl;
		break;
	case ERR:
		cout << "Err" << endl;
		break;
	}
}