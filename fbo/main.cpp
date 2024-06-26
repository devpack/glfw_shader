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
const int screen_width = 1280;  // for non fullscreen
const int screen_height = 800; // for non fullscreen
bool fullscreen = false; // if true display->screen_width / screen_height are overwritten by monitor size
bool vsync = true;
bool osr_framebuffer = false;

// camera globals
float keyboard_sensitivity = 0.01f;
float mouse_sensitivity = 0.1f;
float znear = 0.01f;
float zfar = 100.0f;
float fov = 45.0f;
glm::vec3 camera_pos = glm::vec3(0, 0, 5);


int main(int argc, char* argv[])
{     
	auto display = make_shared<MyDisplay>(::screen_width, ::screen_height, ::fullscreen, ::vsync);

	srand(time(NULL));

	// cube vertices
	vector<glm::vec3> cube;

    for(unsigned int i = 0; i < 10000; i++) {
		cube.push_back(glm::vec3(xrand(-1.0, 1.0), xrand(-1.0, 1.0), xrand(-1.0, 1.0)));
	}

	// keyboard / mouse callbacks binded to display->mainWindow
	auto input = make_shared<Input>(display->mainWindow);

	// data vao/vbo
	auto render = make_shared<Render>(cube, display->screen_width, display->screen_height, ::osr_framebuffer);

	// scene shader
	auto scene_shader = make_shared<Shader>("../shaders/scene_vs.glsl", "../shaders/scene_fs.glsl");

	// quad screen shader
	auto quad_screen_shader = make_shared<Shader>("../shaders/quad_vs.glsl", "../shaders/quad_fs.glsl");
	quad_screen_shader -> Use();
	quad_screen_shader -> setInt("screenTexture", 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// camera
	auto camera = make_shared<Camera>(::camera_pos, ::fov, (float)display->screen_width/(float)display->screen_height, ::znear, ::zfar, ::mouse_sensitivity, ::keyboard_sensitivity);

	// cube motion
	float motion_counter = 0.0f;

    // FPS
    double t, t0, fps;
    char fpstr[50];
    int frames = 0;

    t0 = glfwGetTime();

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

		// 1. Render the scene into a color texture attached to our new custom framebuffer object (bound as the active framebuffer)

		if(::osr_framebuffer) {
			// bind to framebuffer and draw scene as we normally would to color texture 
			glBindFramebuffer(GL_FRAMEBUFFER, render->custom_framebuffer);
			glEnable(GL_DEPTH_TEST);
		}

		// clear
		display -> Clear(0.0f, 0.0f, 0.0f, 1.0f);

		// glUseProgram
		scene_shader -> Use();

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
		glm::mat4 vp = camera->GetViewProjection();
		scene_shader -> setMat4("mvp", vp * tr_mx * rotx_mx * rot_my * rot_mz);

		// vao / vbo
		render -> DrawScene();

		if(::osr_framebuffer) {
			// 2. now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			
			display -> Clear(1.0f, 1.0f, 1.0f, 1.0f);

			// 3. Draw a quad that spans the entire screen with the new framebuffer's color buffer as its texture
			quad_screen_shader -> Use();
			//quad_screen_shader -> setMat4("mvp", vp);

			render -> DrawQuadScreen();
		}

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
