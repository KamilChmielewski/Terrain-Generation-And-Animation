#include "DiamondSquare.h"
#include <stdlib.h>
#include <iostream>

DiamondSquare::DiamondSquare(int width, int height, int range) : _width(width), _height(height), _range(range)
{
	InitializeMap();

	DiamondSquareProcess(map, _width);
	std::cout << "done" << std::endl;

	//for (int i = 0; i < _width; i++)
	//{
	//	for (int j = 0; j < _width; j++)
	//	{
	//		std::cout << map[i][j] << " : " << i << " " << j << std::endl;
	//	}
	//}
}

DiamondSquare::~DiamondSquare()
{
	//Freeing each sub-array
	for (int i = 0; i < _width; i++)
	{
		delete[] map[i];
	}

	//removes array of pointer;
	delete[] map;
}

void DiamondSquare::InitializeMap()
{
	map = (int**)malloc(_width * sizeof(int));

	for (int i = 0; i < _width; i++)
	{
		map[i] = (int*)malloc(_height * sizeof(int));
		
		for (int j = 0; j < _height; j++)
		{
			int row = i;
			int cols = j;
			map[i][j] = 0;
		}
	}

	map[0][0] = RandomNumber(_range);
	map[0][_width - 1] = RandomNumber(_range);
	map[_width - 1][0] =  RandomNumber(_range);
	map[_width - 1][_width - 1] = RandomNumber(_range);
}


void DiamondSquare::DiamondSquareProcess(int** array, int size)
{
	int half = size / 2;
	if (half < 1)
		return;

	//SquareStep
	for (int z = half; z < _height; z+=size)
	{
		for (int x = half; x < _width; x+=size)
		{
			SquareStep(map, x % _width, z % _height, half);
		}
	}

	//diamondStep
	int col = 0;

	for (int x = 0; x < _width; x+= half)
	{
		col++;
		//check if coloumn is odd
		if (col % 2 == 1)
			for (int z  = half;  z < _height; z+=size)
			{
				DiamondStep(map, x % _width, z % _height, half);
			}
		else
			for (int z = 0; z < _height; z+=size)
			{
				DiamondStep(map, x % _width, z % _height, half);
			}
	}
	DiamondSquareProcess(map, size / 2);
}

void DiamondSquare::DiamondStep(int** array, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0)
	{
		avg += map[x - reach][z];
		count++;
	}
	if (x + reach < _width)
	{
		avg += map[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += map[x][z - reach];
		count++;
	}
	if (z + reach < _height)
	{
		avg += map[x][z + reach];
		count++;
	}

	avg += RandomNumber(reach);
	avg /= count;
	map[x][z] = round(avg);
}

void DiamondSquare::SquareStep(int** array, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;

	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += map[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < _height)
	{
		avg += map[x - reach][z + reach];
		count++;
	}
	if (x + reach < _width && z - reach >= 0)
	{
		avg += map[x + reach][z - reach];
		count++;
	}
	if (x + reach < _width && z + reach < _height)
	{
		avg += map[x + reach][z + reach];
		count++;
	}

	avg += RandomNumber(reach);
	avg /= count;
	map[x][z] = round(avg);
}

int DiamondSquare::RandomNumber(int range)
{
	return (rand() % (range * 2)) - range;
}