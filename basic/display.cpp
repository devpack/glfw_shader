#include "display.h"

#include <vector> 

using namespace std;

//#define GLFW_EXPOSE_NATIVE_X11
//#include "GLFW/glfw3native.h"

/*---------------------------------------------------------------------------*/

MyDisplay::MyDisplay(int screen_width, int screen_height, bool fullscreen, bool vsync)
{
	
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // GLFW Hints
    if(true) {
        glfwDefaultWindowHints();

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

        if(fullscreen) {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
        }
        else {
            glfwWindowHint(GLFW_DECORATED, GL_TRUE);
        }

        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_FLOATING, GL_FALSE); // top most
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    }

	if(true) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}

	// window
	if(!fullscreen) {
		this->screen_width = screen_width;
		this->screen_height = screen_height;
	}

	mainWindow = glfwCreateWindow(this->screen_width, this->screen_height, "", NULL, NULL);

    if (!mainWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	// screen size
	int actual_screen_width, actual_screen_height;
    glfwGetFramebufferSize(mainWindow, &actual_screen_width, &actual_screen_height);
    cout << "Frame buffer size " << actual_screen_width << "x" << actual_screen_height << endl;

    glfwMakeContextCurrent(mainWindow);
    
    glfwSetWindowPos(mainWindow, 0, 0);
    glfwFocusWindow(mainWindow);

    // string version
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    cout << "GPU: " << renderer << endl;
    cout << "OpenGL Version: " << version << endl;

	if(vsync) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW
    glewExperimental = GL_TRUE;
	
    auto init_res = glewInit();
    if(init_res != GLEW_OK)
    {
        std::cout << glewGetErrorString(glewInit()) << std::endl;
    }

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_RASTERIZER_DISCARD);
	

}

/*---------------------------------------------------------------------------*/

void MyDisplay::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*---------------------------------------------------------------------------*/

void MyDisplay::SwapBuffers()
{
	glfwSwapBuffers(mainWindow);
}

/*---------------------------------------------------------------------------*/

MyDisplay::~MyDisplay()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}



