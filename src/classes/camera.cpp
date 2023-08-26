#include "camera.h"
/*
    as you can probably see, this file has way more comments than the others
    please do not delete them, just know that i need those to remember which function does wich shit
*/


Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
    projection = glm::perspective(glm::radians(FOVdeg), aspectRatio, nearPlane, farPlane);

    cameraMatrix = projection * view;

}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::UpdateAspectRatio(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    aspectRatio = static_cast<float>(width) / height;
}



void Camera::Inputs(SDL_Window* window)
{
    // Get keyboard state
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // Handles key inputs
    if (keyboardState[SDL_SCANCODE_W])
    {
        Position += speed * Orientation;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        Position += speed * -Orientation;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (keyboardState[SDL_SCANCODE_SPACE])
    {
        Position += speed * Up;
    }
    if (keyboardState[SDL_SCANCODE_LCTRL])
    {
        Position += speed * -Up;
    }
    if (keyboardState[SDL_SCANCODE_LSHIFT])
    {
        speed = 0.1f;
    }
    else
    {
        speed = 0.05f;
    }

    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        SDL_ShowCursor(SDL_DISABLE);

        if (firstClick)
        {
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
            SDL_GetMouseState(&mouseX, &mouseY);
            firstClick = false;
        }


        float deltaX = static_cast<float>(mouseX - (width / 2));
        float deltaY = static_cast<float>(mouseY - (height / 2));

        // Calculate the rotation angles based on the change in mouse position
        float rotX = sensitivity * deltaY / height;
        float rotY = sensitivity * deltaX / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Decides whether or not the next vertical orientation is legal or not
        if (std::abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }
        
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Set mouse cursor to the middle of the screen so that it doesn't end up roaming around
        SDL_WarpMouseInWindow(window, width / 2, height / 2);
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
        firstClick = true;
    }
}

