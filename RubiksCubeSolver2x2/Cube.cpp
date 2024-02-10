#include "Cube.h"
#include <algorithm>

enum Faces { U, F, L, B, R, D };

const Move MOVE_R = Move("R", 3, 4, {{{R, 0, 0}, {R, 0, 1}, {R, 1, 1}, {R, 1, 0}},
									{ {F, 0, 1}, {U, 0, 1}, {B, 1, 0}, {D, 0, 1} },
									{ {F, 1, 1}, {U, 1, 1}, {B, 0, 0}, {D, 1, 1} } });

const Move MOVE_Y = Move("y", 6, 4, {{{U, 0, 0}, {U, 0, 1}, {U, 1, 1}, {U, 1, 0}},
									{ {F, 0, 0}, {L, 0, 0}, {B, 0, 0}, {R, 0, 0} },
									{ {F, 0, 1}, {L, 0, 1}, {B, 0, 1}, {R, 0, 1} },
									{ {F, 1, 0}, {L, 1, 0}, {B, 1, 0}, {R, 1, 0} },
									{ {F, 1, 1}, {L, 1, 1}, {B, 1, 1}, {R, 1, 1} },
									{ {D, 0, 0}, {D, 1, 0}, {D, 1, 1}, {D, 0, 1} } });

const Move MOVE_U = Move("U", 3, 4, {{{U, 0, 0}, {U, 0, 1}, {U, 1, 1}, {U, 1, 0}},
									{ {F, 0, 0}, {L, 0, 0}, {B, 0, 0}, {R, 0, 0} },
									{ {F, 0, 1}, {L, 0, 1}, {B, 0, 1}, {R, 0, 1} } });

const Move MOVE_F = Move("F", 3, 4, { { {F, 0, 0}, {F, 0, 1}, {F, 1, 1}, {F, 1, 0} },
									{ {U, 1, 0}, {R, 0, 0}, {D, 0, 1}, {L, 1, 1} },
									{ {U, 1, 1}, {R, 1, 0}, {D, 0, 0}, {L, 0, 1} } });

const std::vector<Move> Cube::moves{ MOVE_R, MOVE_U, MOVE_F };

const std::string FACES_NAMES = "UFLBRD";


void Cube::load(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	for (auto& face : tiles)
	{
		for (auto& cube_row : face)
		{
			std::string row;
			file >> row;
			for (int col = 0; col < CUBE_SIZE; col++)
				cube_row[col] = row[col];
		}
	}

	file.close();
}

void Cube::print() const
{
	for (int face_index = 0; face_index < FACES_AMOUNT; face_index++)
	{
		std::cout << FACES_NAMES[face_index] << ":" << std::endl;

		const auto& face = tiles[face_index];
		for (const auto& row : face)
		{
			for (const auto& tile : row)
				std::cout << tile;
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}

void Cube::move(const Move& move, int iterations)
{
	const std::vector<std::vector<std::vector<int>>>& cycles = move.cycles;
	int cycles_amount = move.cycles_amount;
	int cycle_length = move.cycle_length;

	int increment = iterations > 0 ? 1 : -1;

	for (int i = 0; i < abs(iterations); i++)
	{
		for (int cycle = 0; cycle < cycles_amount; cycle++)
		{
			if (iterations > 0)
				for (int tile = cycle_length - 1; tile > 0; tile--)
				{
					int face = cycles[cycle][tile][0];
					int row = cycles[cycle][tile][1];
					int column = cycles[cycle][tile][2];

					int next_tile = (tile + 1) % cycle_length;
					int next_face = cycles[cycle][next_tile][0];
					int next_row = cycles[cycle][next_tile][1];
					int next_column = cycles[cycle][next_tile][2];

					std::swap(tiles[face][row][column], tiles[next_face][next_row][next_column]);
				}
			else
				for (int tile = 0; tile < cycle_length - 1; tile++)
				{
					int face = cycles[cycle][tile][0];
					int row = cycles[cycle][tile][1];
					int column = cycles[cycle][tile][2];

					int next_tile = (tile + 1) % cycle_length;
					int next_face = cycles[cycle][next_tile][0];
					int next_row = cycles[cycle][next_tile][1];
					int next_column = cycles[cycle][next_tile][2];

					std::swap(tiles[face][row][column], tiles[next_face][next_row][next_column]);
				}
		}
	}
}

void Cube::moveR() { move(MOVE_R, 1); }
void Cube::moveR2() { move(MOVE_R, 2); }
void Cube::moveRp() { move(MOVE_R, 3); }

void Cube::moveU() { move(MOVE_U, 1); }
void Cube::moveU2() { move(MOVE_U, 2); }
void Cube::moveUp() { move(MOVE_U, 3); }

void Cube::moveF() { move(MOVE_F, 1); }
void Cube::moveF2() { move(MOVE_F, 2); }
void Cube::moveFp() { move(MOVE_F, 3); }

std::string Cube::stringify() const
{
	std::string ans = "";
	for (const auto& face : tiles)
		for (const auto& row : face)
			for (const auto& tile : row)
				ans += tile;
	return ans;
}