#define _USE_MATH_DEFINES
#include "gui.h"
#include "config.h"
#include <jpegio.h>
#include <iostream>
#include <debuggl.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace {
	// Intersect a cylinder with radius 1/2, height 1, with base centered at
	// (0, 0, 0) and up direction (0, 1, 0).
	bool IntersectCylinder(const glm::vec3& origin, const glm::vec3& direction,
			float radius, float height, float* t)
	{
		//FIXME perform proper ray-cylinder collision detection
		return true;
	}
}

GUI::GUI(GLFWwindow* window)
	:window_(window)
{
	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, KeyCallback);
	glfwSetCursorPosCallback(window_, MousePosCallback);
	glfwSetMouseButtonCallback(window_, MouseButtonCallback);

	glfwGetWindowSize(window_, &window_width_, &window_height_);
	float aspect_ = static_cast<float>(window_width_) / window_height_;
	projection_matrix_ = glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
}

GUI::~GUI()
{
}

void GUI::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GL_TRUE);
		return ;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
		// TODO: Better screenshot function here, screenshotDD-MM-YY-#
		//       Need to check if the file exists before creating screenshot
		char* fname = "./screenshot.jpeg";
		int buffer_size = (window_width_ * window_height_ * 3);
		unsigned char* buffer = (unsigned char *)calloc(1, buffer_size);
		glReadPixels(0, 0, window_width_, window_height_, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		SaveJPEG(fname, window_width_, window_height_, buffer);
	}

	if (captureWASDUPDOWN(key, action))
		return ;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
		if (key == GLFW_KEY_RIGHT) {
			rotateEye(-look_, roll_speed);
		} else {
			rotateEye(-look_, -roll_speed);
		}
	}
	else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		fps_mode_ = !fps_mode_;
	}
}

void GUI::mousePosCallback(double mouse_x, double mouse_y)
{
	lPos = cPos;
	cPos = glm::vec2(mouse_x, mouse_y);
	dV = lPos - cPos;
	dVl = glm::length(dV);
	if (current_button_ == GLFW_MOUSE_BUTTON_LEFT && drag_state_) {
		if (!fps_mode_) {
			rotateEye(up_, rotation_speed_ * (dV.x / dVl)*0.5);
			rotateEye(tangent_, rotation_speed_ * (dV.y / dVl)*0.5);
		}
		else {
			eye_ += -0.2f*(dV.x / dVl)*pan_speed_*tangent_;
			center_ += -0.2f*(dV.x / dVl)*pan_speed_*tangent_;
			eye_ += -0.2f*(dV.y / dVl)*pan_speed_*up_;
			center_ += -0.2f*(dV.y / dVl)*pan_speed_*up_;
		}
	}
	else if ((current_button_ == GLFW_MOUSE_BUTTON_RIGHT) && drag_state_) {
		if (fps_mode_)
			eye_ += (dV.y/dVl)*zoom_speed_ * look_;
		else
			camera_distance_ -= (dV.y/dVl)*zoom_speed_;
	}
}

void GUI::mouseButtonCallback(int button, int action, int mods)
{
	drag_state_ = (action == GLFW_PRESS);
	current_button_ = button;
}

void GUI::updateMatrices()
{
	// Compute our view, and projection matrices.
	if (fps_mode_)
		center_ = eye_ + camera_distance_ * look_;
	else
		eye_ = center_ - camera_distance_ * look_;

	view_matrix_ = glm::lookAt(eye_, center_, up_);
	light_position_ = glm::vec4(eye_, 1.0f);

	aspect_ = static_cast<float>(window_width_) / window_height_;
	projection_matrix_ =
		glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	model_matrix_ = glm::mat4(1.0f);
}

MatrixPointers GUI::getMatrixPointers() const
{
	MatrixPointers ret;
	ret.projection = &projection_matrix_[0][0];
	ret.model= &model_matrix_[0][0];
	ret.view = &view_matrix_[0][0];
	return ret;
}

bool GUI::captureWASDUPDOWN(int key, int action)
{
	if (key == GLFW_KEY_W) {
		if (fps_mode_)
			eye_ += zoom_speed_ * look_;
		else
			camera_distance_ -= zoom_speed_;
		return true;
	} else if (key == GLFW_KEY_S) {
		if (fps_mode_)
			eye_ -= zoom_speed_ * look_;
		else
			camera_distance_ += zoom_speed_;
		return true;
	} else if (key == GLFW_KEY_A) {
		if (fps_mode_)
			eye_ -= pan_speed_ * tangent_;
		else
			rotateEye(up_, rotation_speed_);
		return true;
	} else if (key == GLFW_KEY_D) {
		if (fps_mode_)
			eye_ += pan_speed_ * tangent_;
		else
			rotateEye(up_, -rotation_speed_);
		return true;
	} else if (key == GLFW_KEY_DOWN) {
		if (fps_mode_)
			eye_ -= pan_speed_ * up_;
		else
			rotateEye(tangent_, rotation_speed_);
		return true;
	} else if (key == GLFW_KEY_UP) {
		if (fps_mode_)
			eye_ += pan_speed_ * up_;
		else
			rotateEye(tangent_, -rotation_speed_);
		return true;
	}
	return false;
}


// Delegrate to the actual GUI object.
void GUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->keyCallback(key, scancode, action, mods);
}

void GUI::MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mousePosCallback(mouse_x, mouse_y);
}

void GUI::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(button, action, mods);
}

void GUI::rotateEye(glm::vec3 axis_of_rotation, float angle) {
	glm::mat4x4 V = { tangent_.x, up_.x, look_.x, eye_.x,
					  tangent_.y, up_.y, look_.y, eye_.y,
					  tangent_.z, up_.z, look_.z, eye_.z,
					  0.0f      , 0.0f , 0.0f   , 1.0f };

	V = glm::rotate(V, angle, axis_of_rotation);
	tangent_ = glm::vec3(V[0][0], V[1][0], V[2][0]);
	up_ = glm::vec3(V[0][1], V[1][1], V[2][1]);
	look_ = glm::vec3(V[0][2], V[1][2], V[2][2]);
	eye_ = glm::vec3(V[0][3], V[1][3], V[2][3]);
	camera_distance_ = glm::distance(eye_, center_);
}
