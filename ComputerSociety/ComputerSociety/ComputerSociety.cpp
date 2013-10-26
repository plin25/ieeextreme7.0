#include <iostream>
using namespace std;

class LCG
{
private:
	static const unsigned long long m = 1Ui64 << 32;
	static const int a = 1664525;
	static const int c = 1013904223;
	int current;
public:
	LCG(int seed) : current(seed) {}
	int randInt(int max) { return next() % max; }
	int next() { return current = (a * current + c) % m; }
	int randInt(int min, int max) { return abs(next() % (max - min + 1) + min); }
};


enum Direction
{
	NONE = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8
};

class Maze
{
private:
	int** maze;
	int rows, cols;
	int startRow, startCol;
	int endRow, endCol;
public:
	Maze(int rows, int cols) : rows(rows), cols(cols)
	{
		LCG lcg(999);
		maze = ((int**)malloc(rows * sizeof(int*)));
		for (int i = 0; i < rows; ++i)
			maze[i] = ((int*) calloc(cols, sizeof(int)));
		int borderRows = rows - 2, borderCols = cols - 2;

		//Create start position
		int startPos = lcg.randInt(1, (borderRows + borderCols) * 2);
		if (startPos > borderCols + 2 * borderRows)
		{
			startRow = rows - 1;
			startCol = startPos - (borderCols + 2 * borderRows);
		}
		else if (startPos > borderCols)
		{
			startRow = (startPos - borderCols + 1) / 2;
			startCol = (startPos - borderCols % 2 != 0) ? 0 : borderCols + 1;
		}
		else
		{
			startRow = 0;
			startCol = startPos;
		}

		//Create end position
		int endPos = lcg.randInt(1, (borderRows + borderCols) * 2 - 1);
		if (endPos >= startPos) endPos++;
		if (endPos > borderCols + 2 * borderRows)
		{
			endRow = rows - 1;
			endCol = endPos - (borderCols + 2 * borderRows);
			maze[endRow][endCol] |= DOWN;
		}
		else if (endPos >= borderCols)
		{
			endRow = (endPos - borderCols + 1) / 2;
			if (endPos - borderCols % 2 != 0)
			{
				endCol = 0;
				maze[endRow][endCol] |= LEFT;
			}
			else
			{
				endCol = borderCols + 1;
				maze[endRow][endCol] |= RIGHT;
			}
		}
		else
		{
			endRow = 0;
			endCol = endPos;
			maze[endRow][endCol] |= UP;
		}

		//Generate inner cells
		for (int col = 1; col < cols - 2; ++col)
		{
			for (int row = 1; row < rows - 2; ++row)
			{
				switch (lcg.randInt(1, 4))
				{
				case 1:
					maze[row][col] |= UP;
					maze[row - 1][col] |= DOWN;
					break;
				case 2:
					maze[row][col] |= DOWN;
					maze[row + 1][col] |= UP;
					break;
				case 3:
					maze[row][col] |= LEFT;
					maze[row][col - 1] |= RIGHT;
					break;
				case 4:
					maze[row][col] |= RIGHT;
					maze[row][col + 1] |= LEFT;
					break;
				}
			}
		}
	}

	void print()
	{
		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
				printf("x%sx", (maze[row][col] & UP ? " " : "x"));
			cout << endl;
			for (int col = 0; col < cols; ++col)
				printf("%s%s%s", (maze[row][col] & LEFT ? " " : "x"), (row == startRow && col == startCol ? "S" : (row == endRow && col == endCol ? "E" : " ")), (maze[row][col] & RIGHT ? " " : "x"));
			cout << endl;
			for (int col = 0; col < cols; ++col)
				printf("x%sx", (maze[row][col] & DOWN ? " " : "x"));
			cout << endl;
		}
	}

	~Maze()
	{
		for (int i = 0; i < rows; ++i)
			delete[] maze[i];
		delete[] maze;
	}
};

int main()
{
	Maze maze(10, 10);
	maze.print();
}