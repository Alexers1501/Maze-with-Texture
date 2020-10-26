/*
 * Window.h
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: User
 */

#ifndef WINDOW_H_
#define WINDOW_H_

class Window
{
public:
	static constexpr int DEFAULT_WIDTH = 1280;
	static constexpr int DEFAULT_HEIGHT = 720;
public:
	Window(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	virtual ~Window() = default;
	Window(const Window &other) = delete;
	Window(Window &&other) = default;
};

#endif /* WINDOW_H_ */
