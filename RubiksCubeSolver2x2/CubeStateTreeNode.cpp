#include "CubeStateTreeNode.h"

CubeStateTreeNode::CubeStateTreeNode(const Cube& cube, CubeStateTreeNode* parent, const std::string& move)
:
	cube(cube),
	parent(parent),
	move(move)
{}