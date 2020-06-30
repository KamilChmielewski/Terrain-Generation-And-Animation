#pragma once

class DiamondSquare
{
private:
	const int _width;
	const int _height;
	const int _range;
	int** map;

public:

	DiamondSquare(int width, int height, int range);
	~DiamondSquare();

	void InitializeMap();

	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
	int** GetMap() { return map; }

	void SquareStep(int** array, int x, int z, int reach);
	void DiamondStep(int** array, int x, int z, int reach);
	void DiamondSquareProcess(int** array, int size);

	int RandomNumber(int range);
};