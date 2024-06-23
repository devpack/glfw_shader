#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/*---------------------------------------------------------------------------*/

class Render
{
	public:
		Render(std::vector<glm::vec3> vertices, int screen_width, int screen_height, bool use_frambuffer);
		virtual ~Render();

		void DrawScene();
		void DrawQuadScreen();

	public:
		bool use_frambuffer;
       	int screen_width;
       	int screen_height;

	public:
		// Scene attributes
		GLuint vao;
		GLuint vertex_vbo;

		unsigned int nb_vertices;

	public:
		// Custom framebuffer attributes
		unsigned int custom_framebuffer;
		unsigned int textureColorbuffer;
		unsigned int rbo;

		// quad screen 
		unsigned int quadVAO, quadVBO;
};
