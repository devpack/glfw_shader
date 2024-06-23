#include "render.h"

/*---------------------------------------------------------------------------*/

Render::Render()
{

		float quad_screen[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-0.8f,  0.8f,  0.0f, 0.8f,
			-0.8f, -0.8f,  0.0f, 0.0f,
			0.8f, -0.8f,  0.8f, 0.0f,

			-0.8f,  0.8f,  0.0f, 0.8f,
			0.8f, -0.8f,  0.8f, 0.0f,
			0.8f,  0.8f,  0.8f, 0.8f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_screen), &quad_screen, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	    glBindVertexArray(0);
}

/*---------------------------------------------------------------------------*/

Render::~Render()
{

		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
}

/*---------------------------------------------------------------------------*/

void Render::Draw()
{
	// bind our VAO as the current used object: so any operation that would affect a VAO will affect this particular VAO
	glBindVertexArray(quadVAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}
