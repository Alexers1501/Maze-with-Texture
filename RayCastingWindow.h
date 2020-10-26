/*
 * RayCastingWindow.h
 *
 *  Created on: 26 ���. 2020 �.
 *      Author: User
 */

#ifndef RAYCASTINGWINDOW_H_
#define RAYCASTINGWINDOW_H_
#include <memory>

#include "Window.h"
#include "Map.h"
#include "Player.h"

class RayCastingWindow: public Window
{
protected:
	std::shared_ptr<Map> _map;
	std::shared_ptr<Player> _player;
public:
	RayCastingWindow(
			int width = DEFAULT_WIDTH,
			int height = DEFAULT_HEIGHT);
	virtual ~RayCastingWindow() = default;
	RayCastingWindow(const RayCastingWindow &other) = delete;
	RayCastingWindow(RayCastingWindow &&other) = default;

	virtual void render() override;
	virtual void handle_keys(const Uint8*) override;
};

#endif /* RAYCASTINGWINDOW_H_ */
