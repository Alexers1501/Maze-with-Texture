/*
 * Map.h
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: User
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <string>
#include <cmath>

class Map
{
protected:
	int _width {0}, _height {0};
	double _start_x {0.}, _start_y {0.}, _start_dir {0.};
	std::vector<std::string> _data;
public:
	Map (const char * filename);

	int getHeight() const {
		return _height;
	}

	double getStartDir() const {
		return _start_dir;
	}

	double getStartX() const {
		return _start_x;
	}

	double getStartY() const {
		return _start_y;
	}

	int getWidth() const {
		return _width;
	}

	bool wall(int x, int y)
	{
		if (x < 0 or x >= _width
				or y < 0 or y >= _height)
			return true;
		return _data[y][x] == '#';
	}
	bool wall(double x, double y)
	{
		return wall(int(floor(x)), int(floor(y)));//округляем в меньшую сторону
	}
};

#endif /* MAP_H_ */
