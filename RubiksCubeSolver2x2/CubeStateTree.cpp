#include "CubeStateTree.h"
#include "Cube.h"

CubeStateTree::CubeStateTree(const Cube& root_cube)
:
	root(root_cube, nullptr, "")
{
	seen[root_cube.stringify()] = &root;

	bfs_order.push(&root);
}

void CubeStateTree::makeChildren(CubeStateTreeNode& node, bool reverse_moves)
{
	

	node.children.emplace_back(node.cube, &node, "F");
	if (!reverse_moves)
		node.children[0].cube.moveF();
	else
		node.children[0].cube.moveFp();

	node.children.emplace_back(node.cube, &node, "U");
	if (!reverse_moves)
		node.children[1].cube.moveU();
	else
		node.children[1].cube.moveUp();

	node.children.emplace_back(node.cube, &node, "R");
	if (!reverse_moves)
		node.children[2].cube.moveR();
	else
		node.children[2].cube.moveRp();

	for (auto& child : node.children)
	{
		std::string hash_string = child.cube.stringify();
		bool has_child_been_seen = seen.find(hash_string) != seen.end();
		if (!has_child_been_seen)
			seen[hash_string] = &child;
		else
			child.is_usable = false;
	}
}
