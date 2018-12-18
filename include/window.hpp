#pragma once

#include "common.hpp"

namespace window {

	bool Init(const std::string& title = "Untitled", int width = 1280, int height = 720);
	void Free();

	void swapBuffers();
	void setShouldClose(bool b);
	bool shouldClose();

	GLFWwindow* handle();
	int width();
	int height();

} // namespace window