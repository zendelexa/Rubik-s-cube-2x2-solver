#pragma once
#include <vector>
#include <string>
#include "TypeDefs.h"

class Move
{
public:
	std::string name;
	int cycles_amount;
	int cycle_length;
	vector3D<int> cycles;

	Move(const std::string& name, int cycles_amount, int cycle_length, const vector3D<int>& cycles);
};