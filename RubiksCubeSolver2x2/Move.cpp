#include "Move.h"

Move::Move(int cycles_amount, int cycle_length, const vector3D<int>& cycles)
	: cycles_amount(cycles_amount),
	  cycle_length(cycle_length),
	  cycles(cycles)
{}