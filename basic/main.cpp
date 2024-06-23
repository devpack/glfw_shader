#include "display.h"
#include "camera.h"
#include "input.h"
#include "shader.h"
#include "render.h"

#include <sstream>
#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <GLFW/glfw3.h>

using namespace std;

// --------------------------------------------------------------------------------------------

float xrand(float xl, float xh)
{
	return (xl + (xh - xl) *  drand48() ); 
}

// --------------------------------------------------------------------------------------------

// screen globals
const int screen_width = 1280;
const int screen_height = 800;
bool fullscreen = false; // if true display->screen_width / screen_height are overwritten by monitor size
bool vsync = false;

// camera globals
float keyboard_sensitivity = 0.01f;
float mouse_sensitivity = 0.1f;
float znear = 0.01f;
float zfar = 1000.0f;
float fov = 45.0f;
glm::vec3 camera_pos = glm::vec3(0, 0, 5);

//
int main(int argc, char* argv[])
{     
	auto display = make_shared<MyDisplay>(screen_width, screen_height, fullscreen, vsync);

	srand(time(NULL));

	// keyboard / mouse callbacks binded to display->mainWindow
	auto input = make_shared<Input>(display->mainWindow);

	// data vao/vbo
	auto render = make_shared<Render>();

	// shaders
	auto shader = make_shared<Shader>("../shaders/quad_vs.glsl", "../shaders/quad_fs.glsl");

	// camera
	auto camera = make_shared<Camera>(camera_pos, fov, (float)display->screen_width/(float)display->screen_height, znear, zfar, mouse_sensitivity, keyboard_sensitivity);

	// --- main loop

	// cube motion
	float motion_counter = 0.0f;

    // FPS
    double t, t0, fps;
    char fpstr[50];
    int frames = 0;

    t0 = glfwGetTime();
	
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // loop until ESC press
	while(!glfwWindowShouldClose(display->mainWindow))
	{
        // FPS
        t = glfwGetTime();

        if( (t-t0) > 1.0 || frames == 0 )
        {
            fps = (double)frames / (t-t0);
            sprintf( fpstr, "FPS = %.1f", fps );
            glfwSetWindowTitle(display->mainWindow, fpstr);
            t0 = t;
            frames = 0;
        }

        frames ++;

		// clear
		display -> Clear(0.0f, 0.0f, 0.0f, 1.0f);

		// glUseProgram
		shader -> Bind();

		// compute the ViewProjection matrix (projection * lookAt)
		camera -> ProcessMouse(input->mdx, input->mdy, true);
		input -> mdx = 0;
		input -> mdy = 0;

		camera -> ProcessKeyboard(input->forward, input->backward, input->left, input->right, input->up, input->down, 10.0);

		// compute a Model matrix (some motion for our cube)
		glm::vec3 pos = glm::vec3();
		pos.x = 1 * sinf(motion_counter);
		glm::mat4 tr_mx = glm::translate(pos);

		glm::vec3 rotx = glm::vec3();
		rotx.x = motion_counter;
		glm::mat4 rotx_mx = glm::rotate(rotx.x, glm::vec3(1.0f, 0.0f, 0.0f));

		glm::vec3 roty = glm::vec3();
		roty.y = -motion_counter;
		glm::mat4 rot_my = glm::rotate(roty.y, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 rotz = glm::vec3();
		rotz.z = -motion_counter;
		glm::mat4 rot_mz = glm::rotate(rotz.z, glm::vec3(0.0f, 0.0f, 1.0f));

		// send our MVP matrix to the currently bound shader
		// camera = view_project matrix; model matrix = tr_mx * rotx_mx * rot_my * rot_mz 
		shader -> Update(tr_mx * rotx_mx * rot_my * rot_mz, camera);

		// no motion
		//shader -> Update(glm::mat4(1.0f), camera);

		// vao / vbo
		render -> Draw();

		// show back buffer
		display -> SwapBuffers();

		// for our cube motion
		if(!input->stop_motion) {
			motion_counter += 0.01f;
		}

    	glfwPollEvents();

	} // end while loop

    return 0;
}
