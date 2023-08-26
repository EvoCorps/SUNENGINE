#ifndef MESH_CLASS
#define MESH_CLASS

#include <string>
#include "Texture.h"
#include "EBO.h"
#include "camera.h"
#include "VAO.h"


class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	VAO VAO;
	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh
	void Draw(Shader& shader, Camera& camera);
};

#endif // !MESH_CLASS
