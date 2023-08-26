
#include <stdio.h>
#include <direct.h>
#include "classes/mesh.h"
#include <SDL.h>
#include <SDL_opengl.h>
/*we don't need to include all the things we were including
    mesh.h already ahs a reference to those
*/

/* no ImGui for now, i removed it since i don't need it and i can't make it work
* if you can make it work, please do it
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/imgui/imgui_impl_sdl2.h"*/




const unsigned int width = 800;
const unsigned int height = 800;


/* do not delete this comment, 

                In the vertices array there are 4 columns
    ( first 3 numbers, second 3 numbers, third 2 numbers, fourth 3 numbers)

    1- The first column dictates the coordinates for that vertex.

    2- The second column is used to give a custom color to each vertex.

    3- The third column is is used for normals, wich is 
       basically just some parameters for my light

    4- The fourth and final column is used for the coordinates of the textures

*/
Vertex vertices[] =
{
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

Vertex lightVertices[] =
{
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};




int main(int argc, char* argv[]) {

    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    else {
        perror("getcwd() error");
    }

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
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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


    Texture textures[]
    {
        Texture("assets/planks_mat.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("assets/planks_spc.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };


    Shader shaderProgram("shaders/default.vts", "shaders/default.fgs");
    // store mesh data in vectors for the mesh
    std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    // create floor mesh
    Mesh floor(verts, ind, tex);


    // shader for light cube
    Shader lightShader("shaders/light.vts", "shaders/light.fgs");
    // store mesh data in vectors for the mesh
    std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    // create light mesh
    Mesh light(lightVerts, lightInd, tex);


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    // create the camera
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    bool quit = false;

    glEnable(GL_DEPTH_TEST);

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // update the viewport and projection matrix
                glViewport(0, 0, event.window.data1, event.window.data2);
                camera.UpdateAspectRatio(event.window.data1, event.window.data2);
            }
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        floor.Draw(shaderProgram, camera);
        light.Draw(lightShader, camera);


        SDL_GL_SwapWindow(window);

        SDL_Delay(16);
    }


    shaderProgram.Delete();
    lightShader.Delete();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}