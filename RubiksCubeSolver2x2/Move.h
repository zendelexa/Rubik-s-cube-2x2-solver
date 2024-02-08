#pragma once
#include <vector>
#include "TypeDefs.h"

class Move
{
public:
	int cycles_amount;
	int cycle_length;
	vector3D<int> cycles;

	Move(int cycles_amount, int cycle_length, const vector3D<int>& cycles);
};