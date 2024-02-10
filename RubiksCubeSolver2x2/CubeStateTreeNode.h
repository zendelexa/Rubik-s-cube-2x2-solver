#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Cube.h"

const int AVAILABLE_MOVES_AMOUNT = 3; // F, U, R

class CubeStateTreeNode
{
public:
	Cube cube;
	CubeStateTreeNode* parent = nullptr;
	std::vector<CubeStateTreeNode> children;
	std::string move = "";
	bool is_usable = true;

	CubeStateTreeNode() = default;
	
	CubeStateTreeNode(const Cube& cube, CubeStateTreeNode* parent, const std::string& move);
};