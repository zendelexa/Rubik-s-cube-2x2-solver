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

	void makeChildren(bool reverse_moves)
	{
		children[0] = new CubeStateTreeNode(cube->makeCopy(), this, "F");
		if (!reverse_moves)
			children[0]->cube->moveF();
		else
			children[0]->cube->moveFp();

		children[1] = new CubeStateTreeNode(cube->makeCopy(), this, "U");
		if (!reverse_moves)
			children[1]->cube->moveU();
		else
			children[1]->cube->moveUp();

		children[2] = new CubeStateTreeNode(cube->makeCopy(), this, "R");
		if (!reverse_moves)
			children[2]->cube->moveR();
		else
			children[2]->cube->moveRp();
	}

	void updateSeenChildren(map<string, CubeStateTreeNode*>& seen)
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
				seen[hash_string] = children[i];
			}
		}
	}
};

string reverseMove(string move)
{
	if (move.size() == 1)
		return move + '\'';
	if (move[1] == '\'')
		return "" + move[0];
	return move;
}

map<string, CubeStateTreeNode*> seen_start_tree, seen_end_tree;
queue<CubeStateTreeNode*> bfs_order_start_tree, bfs_order_end_tree;
vector<string> solve(Cube* start_cube, Cube* end_cube)
{
	CubeStateTreeNode* start_node = new CubeStateTreeNode(start_cube, NULL, "");
	CubeStateTreeNode* end_node = new CubeStateTreeNode(end_cube, NULL, "");

	seen_start_tree[start_node->cube->stringify()] = start_node;
	seen_end_tree[end_node->cube->stringify()] = end_node;

	bfs_order_start_tree.push(start_node);
	bfs_order_end_tree.push(end_node);

	int current_side = 1;
	while (true)
	{
		queue<CubeStateTreeNode*>* current_bfs_order = &bfs_order_start_tree;
		queue<CubeStateTreeNode*>* opposite_bfs_order = &bfs_order_end_tree;
		map<string, CubeStateTreeNode*>* current_seen = &seen_start_tree;
		map<string, CubeStateTreeNode*>* opposite_seen = &seen_end_tree;
		if (current_side == 2)
		{
			current_bfs_order = &bfs_order_end_tree;
			opposite_bfs_order = &bfs_order_start_tree;
			current_seen = &seen_end_tree;
			opposite_seen = &seen_start_tree;
		}

		CubeStateTreeNode* current_node = current_bfs_order->front();
		current_bfs_order->pop();

		if (opposite_seen->find(current_node->cube->stringify()) != opposite_seen->end())
		{
			//current_node->cube->print();
			//cout << "\n\n\n";
			CubeStateTreeNode* current_end_tree_cube = NULL;
			CubeStateTreeNode* current_start_tree_cube = NULL;

			if (current_side == 1)
			{
				current_end_tree_cube = seen_end_tree[current_node->cube->stringify()];
				current_start_tree_cube = current_node;
				
			}
			else
			{
				current_end_tree_cube = current_node;
				current_start_tree_cube = seen_start_tree[current_node->cube->stringify()];
				
			}

			stack<string> tmp;
			vector<string> ans;
			while (current_start_tree_cube->parent != NULL)
			{
				tmp.push(current_start_tree_cube->move);
				current_start_tree_cube = current_start_tree_cube->parent;
			}
			while (!tmp.empty())
			{
				ans.push_back(tmp.top());
				tmp.pop();
			}
			while (current_end_tree_cube->parent != NULL)
			{
				ans.push_back(current_end_tree_cube->move);
				current_end_tree_cube = current_end_tree_cube->parent;
			}

			for (string& move : ans)
			{
				cout << move << " ";
			}
			return ans;
		}

		current_node->makeChildren(current_side == 2);
		current_node->updateSeenChildren(*current_seen);
		for (CubeStateTreeNode* child : current_node->children)
		{
			if (child != NULL) current_bfs_order->push(child);
		}

		current_side = (current_side == 1) ? 2 : 1;
	}
}

int main()
{
	Cube cube = Cube();
	cube.load("input.txt");
	//cube.moveF();

	/*cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();*/

	/*cube.moveU2();
	cube.moveRp();
	cube.moveU();
	cube.moveF();
	cube.moveUp();
	cube.moveR2();
	cube.moveFp();
	cube.moveUp();
	cube.moveR2();
	cube.moveF();
	cube.moveR();
	cube.print();*/

	cube.moveR();
	cube.moveUp();
	cube.moveFp();
	cube.moveUp();
	cube.moveFp();
	cube.moveU2();
	cube.moveR2();
	cube.moveF();
	cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.print();

	Cube cube2 = Cube();
	cube2.load("input.txt");
	solve(&cube, &cube2);

	/*cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();
	cube.moveF();
	cube.print();*/

	return 0;
}