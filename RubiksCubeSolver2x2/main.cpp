#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include"Move.h"
#include"Cube.h"

class CubeStateTreeNode
{
public:
	Cube* cube = NULL;
	CubeStateTreeNode* parent = NULL;
	CubeStateTreeNode* children[3] = {};
	std::string move = "";
	CubeStateTreeNode(Cube* cube_, CubeStateTreeNode* parent_, std::string move_)
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

	void updateSeenChildren(std::map<std::string, CubeStateTreeNode*>& seen)
	{
		for (int i = 0; i < 3; i++)
		{
			std::string hash_string = children[i]->cube->stringify();
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

std::string reverseMove(std::string move)
{
	if (move.size() == 1)
		return move + '\'';
	if (move[1] == '\'')
		return "" + move[0];
	return move;
}

std::map<std::string, CubeStateTreeNode*> seen_start_tree, seen_end_tree;
std::queue<CubeStateTreeNode*> bfs_order_start_tree, bfs_order_end_tree;
std::vector<std::string> solve(Cube* start_cube, Cube* end_cube)
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
		std::queue<CubeStateTreeNode*>* current_bfs_order = &bfs_order_start_tree;
		std::queue<CubeStateTreeNode*>* opposite_bfs_order = &bfs_order_end_tree;
		std::map<std::string, CubeStateTreeNode*>* current_seen = &seen_start_tree;
		std::map<std::string, CubeStateTreeNode*>* opposite_seen = &seen_end_tree;
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
			CubeStateTreeNode* current_end_tree_cube = nullptr;
			CubeStateTreeNode* current_start_tree_cube = nullptr;

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

			std::stack<std::string> tmp;
			std::vector<std::string> ans;
			while (current_start_tree_cube->parent != nullptr)
			{
				tmp.push(current_start_tree_cube->move);
				current_start_tree_cube = current_start_tree_cube->parent;
			}
			while (!tmp.empty())
			{
				ans.push_back(tmp.top());
				tmp.pop();
			}
			while (current_end_tree_cube->parent != nullptr)
			{
				ans.push_back(current_end_tree_cube->move);
				current_end_tree_cube = current_end_tree_cube->parent;
			}

			for (const std::string& move : ans)
			{
				std::cout << move << " ";
			}

			seen_end_tree.clear();

			return ans;  // Function stops here!
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

	/*cube.moveR();
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
	cube.print();*/

	//F' R F' U' F2 R2 U R F R'

	cube.moveFp();
	cube.moveR();
	cube.moveFp();
	cube.moveUp();
	cube.moveF2();
	cube.moveR2();
	cube.moveU();
	cube.moveR();
	cube.moveF();
	cube.moveRp();
	cube.print();

	Cube cube2 = Cube();
	cube2.load("input.txt");
	solve(&cube, &cube2);

	std::cout << "HELLOWRLD";

	/*cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();
	cube.moveF();
	cube.print();*/

	return 0;
}