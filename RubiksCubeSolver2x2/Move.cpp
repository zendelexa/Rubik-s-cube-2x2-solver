#include "Move.h"

Move::Move(const std::string& name, int cycles_amount, int cycle_length, const vector3D<int>& cycles)
:
	name(name),
	cycles_amount(cycles_amount),
	cycle_length(cycle_length),
	cycles(cycles)
{}