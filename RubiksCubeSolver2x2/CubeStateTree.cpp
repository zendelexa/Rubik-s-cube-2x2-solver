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
	for (const Move& move : Cube::moves)
	{
		node.children.emplace_back(node.cube, &node, move.name);
		node.children.back().cube.move(move, reverse_moves ? -1 : 1);
	}

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
