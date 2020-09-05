#ifndef HANDLER_HEADER_H
#define HANDLER_HEADER_H

#include "Global.h"
#include "ElementCreator.h"
#include "VertecesHandler.h"
// Include GLFW
#include <glfw3.h>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void handelHover();

#endif HANDLER_HEADER_H
