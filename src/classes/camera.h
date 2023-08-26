#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <SDL.h>
#include <SDL_main.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <imgui/imgui.h>

#include"shaderClass.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	bool firstClick = true;
	int width;
	int height;
	float speed = 0.1f;
	float sensitivity = 100.0f;

	Camera(int width, int height, glm::vec3 position);
	void Matrix(Shader& shader, const char* uniform);
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Inputs(SDL_Window* window);
	void UpdateAspectRatio(int newWidth, int newHeight);
private:
	float aspectRatio = static_cast<float>(width) / height;

};


#endif // !CAMERA_CLASS_H

