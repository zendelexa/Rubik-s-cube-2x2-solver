#pragma once
#include <queue>
#include <unordered_map>
#include "CubeStateTreeNode.h"
#include "Cube.h"

class CubeStateTree
{
public:
	std::queue<CubeStateTreeNode*> bfs_order;
	std::unordered_map<std::string, CubeStateTreeNode*> seen;

	CubeStateTreeNode root;

	CubeStateTree(const Cube& root);

	void makeChildren(CubeStateTreeNode& node, bool reverse_moves);
};