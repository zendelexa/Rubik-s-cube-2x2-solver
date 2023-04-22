#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<stack>

using namespace std;

class Move
{
public:
	int cycles_amount, cycle_length;
	vector<vector<vector<int>>> cycles;
	Move(int cycles_amount_, int cycle_length_, vector<vector<vector<int>>> cycles_)
	{
		cycles_amount = cycles_amount_;
		cycle_length = cycle_length_;
		cycles = cycles_;
	}
};

class Cube
{
	const int U = 0, F = 1, L = 2, B = 3, R = 4, D = 5;

	string faces_names = "UFLBRD";

	Move move_R = Move(3, 4, { { {R, 0, 0}, {R, 0, 1}, {R, 1, 1}, {R, 1, 0} },
							   { {F, 0, 1}, {U, 0, 1}, {B, 1, 0}, {D, 0, 1} },
							   { {F, 1, 1}, {U, 1, 1}, {B, 0, 0}, {D, 1, 1} } });

	Move move_Y = Move(6, 4, { { {U, 0, 0}, {U, 0, 1}, {U, 1, 1}, {U, 1, 0} },
							   { {F, 0, 0}, {L, 0, 0}, {B, 0, 0}, {R, 0, 0} },
							   { {F, 0, 1}, {L, 0, 1}, {B, 0, 1}, {R, 0, 1} },
							   { {F, 1, 0}, {L, 1, 0}, {B, 1, 0}, {R, 1, 0} },
							   { {F, 1, 1}, {L, 1, 1}, {B, 1, 1}, {R, 1, 1} },
							   { {D, 0, 0}, {D, 1, 0}, {D, 1, 1}, {D, 0, 1} } });

	Move move_U = Move(3, 4, { { {U, 0, 0}, {U, 0, 1}, {U, 1, 1}, {U, 1, 0} },
							   { {F, 0, 0}, {L, 0, 0}, {B, 0, 0}, {R, 0, 0} },
							   { {F, 0, 1}, {L, 0, 1}, {B, 0, 1}, {R, 0, 1} } });

	Move move_F = Move(3, 4, { { {F, 0, 0}, {F, 0, 1}, {F, 1, 1}, {F, 1, 0} },
							   { {U, 1, 0}, {R, 0, 0}, {D, 0, 1}, {L, 1, 1} },
							   { {U, 1, 1}, {R, 1, 0}, {D, 0, 0}, {L, 0, 1} } });

public:
	char tiles[6][2][2] = {}; //UFLBRD
	Cube() {}
	void load(string filename)
	{
		ifstream file;
		file.open(filename);
		for (int face = 0; face < 6; face++)
		{
			for (int i = 0; i < 2; i++)
			{
				string row;
				file >> row;
				tiles[face][i][0] = row[0];
				tiles[face][i][1] = row[1];
			}
		}
		file.close();
	}

	void print()
	{
		for (int face = 0; face < 6; face++)
		{
			cout << faces_names[face] << ":" << endl;
			for (int i = 0; i < 2; i++)
			{
				cout << tiles[face][i][0] << tiles[face][i][1] << endl;
			}
			cout << endl;
		}
	}

	void move(Move& move, int iterations)
	{
		vector<vector<vector<int>>>& cycles = move.cycles;
		int cycles_amount = move.cycles_amount;
		int cycle_length = move.cycle_length;

		for (int i = 0; i < iterations; i++)
		{
			for (int cycle = 0; cycle < cycles_amount; cycle++)
			{
				for (int tile = cycle_length - 1; tile > 0; tile--)
				{
					int face = cycles[cycle][tile][0];
					int row = cycles[cycle][tile][1];
					int column = cycles[cycle][tile][2];

					int next_tile = (tile + 1) % cycle_length;
					int next_face = cycles[cycle][next_tile][0];
					int next_row = cycles[cycle][next_tile][1];
					int next_column = cycles[cycle][next_tile][2];

					swap(tiles[face][row][column], tiles[next_face][next_row][next_column]);
				}
			}
		}
	}

	void moveR()  { move(move_R, 1); }
	void moveR2() { move(move_R, 2); }
	void moveRp() { move(move_R, 3); }

	void moveU()  { move(move_U, 1); }
	void moveU2() { move(move_U, 2); }
	void moveUp() { move(move_U, 3); }

	void moveF()  { move(move_F, 1); }
	void moveF2() { move(move_F, 2); }
	void moveFp() { move(move_F, 3); }

	string stringify()
	{
		string ans = "";
		for (int face = 0; face < 6; face++)
		{
			ans += tiles[face][0][0];
			ans += tiles[face][0][1];
			ans += tiles[face][1][1];
			ans += tiles[face][1][0];
		}
		return ans;
	}

	bool isSolved()
	{
		for (int face = 0; face < 6; face++)
		{
			char target_color = tiles[face][0][0];
			if (tiles[face][0][1] != target_color) return false;
			if (tiles[face][1][0] != target_color) return false;
			if (tiles[face][1][0] != target_color) return false;
		}
		return true;
	}

	Cube* makeCopy()
	{
		Cube* ans = new Cube();
		for (int face = 0; face < 6; face++)
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					ans->tiles[face][i][j] = tiles[face][i][j];
				}
			}
		}
		return ans;
	}
};

class CubeStateTreeNode
{
public:
	Cube* cube = NULL;
	CubeStateTreeNode* parent = NULL;
	CubeStateTreeNode* children[3] = {};
	string move = "";
	CubeStateTreeNode(Cube* cube_, CubeStateTreeNode* parent_, string move_)
	{
		cube = cube_;
		parent = parent_;
		move = move_;
	}

	~CubeStateTreeNode()
	{
		delete cube;
	}

	void makeChildren()
	{
		children[0] = new CubeStateTreeNode(cube->makeCopy(), this, "F");
		children[0]->cube->moveF();

		children[1] = new CubeStateTreeNode(cube->makeCopy(), this, "U");
		children[1]->cube->moveU();

		children[2] = new CubeStateTreeNode(cube->makeCopy(), this, "R");
		children[2]->cube->moveR();
	}

	void updateSeenChildren(set<string>& seen)
	{
		for (int i = 0; i < 3; i++)
		{
			string hash_string = children[i]->cube->stringify();
			if (seen.find(hash_string) != seen.end())
			{
				delete children[i];
				children[i] = NULL;
			}
			else
			{
				seen.insert(hash_string);
			}
		}
	}
};

set<string> seen_start_tree, seen_end_tree;
void solve(Cube* cube)
{
	CubeStateTreeNode* start_node = new CubeStateTreeNode(cube, NULL, "");
	queue<CubeStateTreeNode*> bfs_order;
	bfs_order.push(start_node);
	bool keep_going = true;
	while (keep_going)
	{
		CubeStateTreeNode* current_node = bfs_order.front();
		bfs_order.pop();

		//current_node->cube->print();
		//cout << "\n\n\n";
		if (current_node->cube->isSolved())
		{
			stack<string> moves;
			while (current_node->parent != NULL)
			{
				moves.push(current_node->move);
				current_node = current_node->parent;
			}
			while (!moves.empty())
			{
				cout << moves.top() << " ";
				moves.pop();
			}
			return;
		}

		current_node->makeChildren();
		current_node->updateSeenChildren(seen_start_tree);
		for (CubeStateTreeNode* child : current_node->children)
		{
			if (child != NULL) bfs_order.push(child);
		}
	}
}

int main()
{
	Cube cube = Cube();
	cube.load("input.txt");
	//cube.moveF();

	cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();
	solve(&cube);

	/*cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();
	cube.moveF();
	cube.print();*/

	return 0;
}