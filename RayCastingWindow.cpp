/*
 * RayCastingWindow.cpp
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: User
 */
#include <iostream>
#include <cmath>
#include "RayCastingWindow.h"

#include <cassert>


//перменные для алгоритма бросания лучей
		constexpr double H = .5;//коэффициент высоты - высота стены
		constexpr double eps = 0.0001;
		constexpr double FOV = Pi/3;//полный угол обзора игрока
		double dx, dy;
		double rx, ry;
		int h;
		double D;//расстояние до стены
		double sd;//расстояние от глаз до экрана
		double gamma, beta;
		double D_H, D_V;
		double tx, txH, txV;


RayCastingWindow::RayCastingWindow(int width, int height)
:Window(width, height)
{
	_map = std::make_shared<Map>("map01.txt");
	_player = std::make_shared<Player>(_map);

	assert(_renderer != nullptr);

	_wall_texture = std::make_shared<Texture>(_renderer, "texture.jpg");
}

void RayCastingWindow::render()
{
	SDL_Rect r_sky{ 0, 0, getWidth(), getHeight() / 2};
	SDL_Rect r_floor { 0, getHeight() / 2, getWidth(), getHeight() / 2};
	SDL_SetRenderDrawColor(_renderer.get(), 0, 128, 192, 255);
	SDL_RenderClear(_renderer.get());
	SDL_RenderFillRect(_renderer.get(), &r_sky);

	SDL_SetRenderDrawColor(_renderer.get(), 0, 128, 0, 255);
	SDL_RenderFillRect(_renderer.get(), &r_floor);

	//SDL_SetRenderDrawColor(_renderer.get(), 64, 96, 192, 255);



	SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);
	//Рисование стен с использованием алгоритма бросания лучей
	sd = getWidth()/(2 * tan (FOV/2));
	for (int col = 0; col < getWidth(); ++col){

//		Здесь будет алгоритм

		gamma = atan((col - getWidth()/2) / sd);
		beta = _player->getDir() + gamma;
				if (sin(beta) > eps)
				{
					dy = 1;
					ry = floor(_player->getY()) + eps;
					dx = 1 / tan(beta);
					rx = _player->getX() - (_player->getY() - ry) * dx;
					do{
						rx += dx;
						ry += dy;
					}while (not _map->wall(rx, ry));
					D_H = sqrt((rx - _player->getX()) * (rx - _player->getX()) + (ry - _player->getY()) * (ry - _player->getY()));

				}
				else if (sin(beta) < -eps)
					{
						dy = -1;
						ry = ceil(_player->getY()) - eps;
						dx = 1 / tan(-beta);
						rx = _player->getX() - (ry - _player->getY()) * dx;
						do{
							rx += dx;
							ry += dy;
						}while (not _map->wall(rx, ry));
						D_H = sqrt((rx - _player->getX()) * (rx - _player->getX()) + (ry - _player->getY()) * (ry - _player->getY()));

					}
				else
				{
					D_H = INFINITY;
				}
				txH = rx - floor(rx);

				if(cos(beta) > eps){
					dx = 1;
					rx = floor(_player->getX()) + eps;
					dy = tan(beta);
					ry = _player->getY() - (_player->getX() - rx) * dy;
					do{
						rx += dx;
						ry += dy;
					}while (not _map->wall(rx, ry));
					D_V = sqrt((rx - _player->getX()) * (rx - _player->getX()) + (ry - _player->getY()) * (ry - _player->getY()));

				}
				else if(cos(beta) < -eps){
							dx = -1;
							rx = ceil(_player->getX()) - eps;
							dy = tan(-beta);
							ry = _player->getY() - (rx - _player->getX()) * dy;
							do{
								rx += dx;
								ry += dy;
							}while (not _map->wall(rx, ry));
							D_V = sqrt((rx - _player->getX()) * (rx - _player->getX()) + (ry - _player->getY()) * (ry - _player->getY()));

				}
				else{
					D_V = INFINITY;
				}
				txV = ry - floor(ry);

				if (D_H < D_V){
					D = D_H;
					tx = txH;
				}

				else{
					D = D_V;
					tx = txV;
				}
				h = int((sd * H)/D / cos(gamma)) ;

				if (_textured){
					draw_textured_col(col, h, tx);
				}
				else{
					//видимая высота стены на экране
					draw_col(col, h);
				}
				SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
				SDL_RenderDrawLine(_renderer.get(), _player->getX(), _player->getY(), rx, ry);
	}


	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 150);
	for (int y = 0; y < _map -> getHeight(); ++y)
		for (int x = 0; x < _map-> getWidth();++x){
			SDL_Rect r {x * 100, y * 100, 100, 100};
			if (_map->wall (x, y))
				SDL_RenderFillRect(_renderer.get(), &r);
		}


	SDL_Rect r_player {
			int (_player->getX() * 100)-10,
			int (_player->getY() * 100)-10,
			20, 20
	};

	SDL_Rect r_player_eye {
				int (_player->getX() * 100 + 20*cos(_player->getDir()))-5,
				int (_player->getY() * 100 + 20*sin(_player->getDir()))-5,
				10, 10
		};
	SDL_SetRenderDrawColor(_renderer.get(), 255, 64, 64, 255);
	SDL_RenderFillRect(_renderer.get(), &r_player);
	SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 0, 255);
	SDL_RenderFillRect(_renderer.get(), &r_player_eye);

	//стена
	//_wall_texture->draw(nullptr, nullptr);
}

void RayCastingWindow::draw_col(int col, int h)
{
	SDL_SetRenderDrawColor(_renderer.get(), 64, 64, 64, 255);
	int y1 = getHeight() / 2 - h / 2 ;
	int y2 = getHeight() / 2 + h / 2;
	SDL_RenderDrawLine(_renderer.get(), col, y1, col, y2);
}

void RayCastingWindow::draw_textured_col(int col, int h, double tx)
{
	SDL_Rect what { int(floor(_wall_texture->getWidth() * tx)),
		0, 1, _wall_texture->getHeight()};
	SDL_Rect where { col, getHeight()/2 - h/2, 1, h };
	_wall_texture->draw(&what, &where);
}

void RayCastingWindow::handle_keys(const Uint8 *keys)
{
	if (keys [SDL_SCANCODE_W]) _player->walk_forward();
	if (keys [SDL_SCANCODE_S]) _player->walk_backward();
	if (keys [SDL_SCANCODE_D]) _player->shift_right();
	if (keys [SDL_SCANCODE_A]) _player->shift_left();
	if (keys [SDL_SCANCODE_E]) _player->turn_right();
	if (keys [SDL_SCANCODE_Q]) _player->turn_left();
	if (keys [SDL_SCANCODE_T]) _textured = false;
	else _textured = true;




}
