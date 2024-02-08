#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<set>
#include <queue>
#include <stack>
#include "Move.h"
#include "TypeDefs.h"


const int FACES_AMOUNT = 6;
const int CUBE_SIZE = 2;

class Cube
{
public:
	vector3D<char> tiles = vector3D<char>
		(FACES_AMOUNT,	   vector2D<char>
		(CUBE_SIZE,		   std::vector<char>
		(CUBE_SIZE)));

	Cube() = default;
	
	void load(const std::string& filename);

	void print() const;

	void move(const Move& move, int iterations);

	void moveR();
	void moveR2();
	void moveRp();
	void moveU();
	void moveU2();
	void moveUp();
	void moveF();
	void moveF2();
	void moveFp();

	std::string stringify() const;

	bool isSolved() const;

	Cube* makeCopy();

};