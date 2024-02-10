#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include <set>
#include <queue>
#include <stack>
#include "Move.h"
#include "Cube.h"
#include "CubeStateTreeNode.h"
#include "CubeStateTree.h"

std::string reverseMove(std::string move)
{
	if (move.size() == 1)
		return move + '\'';
	if (move[1] == '\'')
		return "" + move[0];
	return move;
}

enum class TreeSide { START_SIDE, END_SIDE };

std::vector<std::string> formResult(CubeStateTree& start_tree, CubeStateTree& end_tree, CubeStateTreeNode& current_node, TreeSide current_side)
{
	CubeStateTreeNode* current_end_tree_cube;
	CubeStateTreeNode* current_start_tree_cube;
	if (current_side == TreeSide::START_SIDE)
	{
		current_end_tree_cube = end_tree.seen[current_node.cube.stringify()];
		current_start_tree_cube = &current_node;
	}
	else
	{
		current_end_tree_cube = &current_node;
		current_start_tree_cube = start_tree.seen[current_node.cube.stringify()];
	}

	std::stack<std::string> tmp;
	std::vector<std::string> res;
	while (current_start_tree_cube->parent != nullptr)
	{
		tmp.push(current_start_tree_cube->move);
		current_start_tree_cube = current_start_tree_cube->parent;
	}
	while (!tmp.empty())
	{
		res.push_back(tmp.top());
		tmp.pop();
	}
	while (current_end_tree_cube->parent != nullptr)
	{
		res.push_back(current_end_tree_cube->move);
		current_end_tree_cube = current_end_tree_cube->parent;
	}

	return res;
}

std::vector<std::string> solve(const Cube& start_cube, const Cube& end_cube)
{
	CubeStateTree start_tree(start_cube);
	CubeStateTree end_tree(end_cube);

	TreeSide current_side = TreeSide::START_SIDE;
	while (true)
	{
		CubeStateTree* current_tree = &start_tree;
		CubeStateTree* opposite_tree = &end_tree;
		if (current_side == TreeSide::END_SIDE)
			std::swap(current_tree, opposite_tree);

		CubeStateTreeNode* current_node = current_tree->bfs_order.front();
		current_tree->bfs_order.pop();

		bool do_trees_connect = opposite_tree->seen.find(current_node->cube.stringify()) != opposite_tree->seen.end();
		if (do_trees_connect)
			return formResult(start_tree, end_tree, *current_node, current_side); // Function exits here!

		current_tree->makeChildren(*current_node, current_side == TreeSide::END_SIDE);
		for (CubeStateTreeNode& child : current_node->children)
			if (child.is_usable) 
				current_tree->bfs_order.push(&child);

		current_side = (current_side == TreeSide::START_SIDE) ? TreeSide::END_SIDE : TreeSide::START_SIDE;
	}
}

int main()
{
	Cube cube;
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
	auto solution = solve(cube, cube2);
	for (auto& move : solution) std::cout << move << " ";

	/*cube.moveR();
	cube.moveU();
	cube.moveRp();
	cube.moveUp();
	cube.moveF();
	cube.print();*/

	return 0;
}