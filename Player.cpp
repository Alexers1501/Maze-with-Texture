/*
 * Player.cpp
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: User
 */

#include "Player.h"

Player::Player(std::shared_ptr<Map> &map)
:_map(map)
{
	reset();

}

void Player::reset()
{
	_x = _map->getStartX();
	_y = _map->getStartY();
	_dir = _map->getStartDir();
}

void Player::walk_forward(double dist)
{
	double nx, ny;
	nx = _x + dist * cos(_dir);
	ny = _y + dist * sin(_dir);
	if (not _map->wall(nx, ny))
	{
		_x = nx;
		_y = ny;
	}
}

void Player::walk_backward(double dist)
{
	walk_forward(-dist);
}

void Player::shift_right(double dist)
{
	double nx, ny;
		nx = _x + dist * cos(_dir + Pi / 2);
		ny = _y + dist * sin(_dir + Pi / 2);
		if (not _map->wall(nx, ny))
		{
			_x = nx;
			_y = ny;
		}
}

void Player::shift_left(double dist)
{
	shift_right(-dist);
}

void Player::turn_right(double angle)
{
	_dir += angle;
	if (_dir >= 2. * Pi)
		_dir -= 2. * Pi;
}

void Player::turn_left(double angle)
{
	_dir -= angle;
	if (_dir < 0.)
		_dir += 2. * Pi;
}
