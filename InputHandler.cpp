#include "InputHandler.hpp"

float InputHandler::movementSpeed = 2.0;
float InputHandler::deltaTime;
float InputHandler::lastFrameTime;

float InputHandler::speed = 20.0f;
float InputHandler::mouseSpeed = 0.5f;

bool InputHandler::mouse_middle = false;
bool InputHandler::shift_mouse_middle = false;

double InputHandler::last_xpos = 0.0;
double InputHandler::last_ypos = 0.0;