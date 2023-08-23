#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdio.h>



// i hate rotation matrix shit
void createRotationMatrix(float angle, float x, float y, float z, float* matrix) {
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0f - c;

    matrix[0] = t * x * x + c;
    matrix[1] = t * x * y - z * s;
    matrix[2] = t * x * z + y * s;
    matrix[3] = 0.0f;

    matrix[4] = t * x * y + z * s;
    matrix[5] = t * y * y + c;
    matrix[6] = t * y * z - x * s;
    matrix[7] = 0.0f;

    matrix[8] = t * x * z - y * s;
    matrix[9] = t * y * z + x * s;
    matrix[10] = t * z * z + c;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}




const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 model;

    void main()
    {
        gl_Position = model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    else {
        SDL_Log("SDL initialized correctly!");
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 + OPENGL = MENTAL ILLINOIS",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    else {
        SDL_Log("GLAD initialized correctly!");
    }



    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));





    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom left
         0.5f, -0.5f, 0.0f, // Bottom right
         0.5f,  0.5f, 0.0f, // Top right
        -0.5f,  0.5f, 0.0f  // Top left
    };
    // i know i'm a dum dum, i can't remember wich is wich

    unsigned int indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    bool quit = false;
    float angle = 0.0f;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);


        float rotationMatrix[16];
        createRotationMatrix(angle, 0.0f, 0.0f, 1.0f, rotationMatrix);
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotationMatrix);

        // Draw the rectangle
        glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);

        angle += 0.03f;


        SDL_Delay(16);
    }
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}