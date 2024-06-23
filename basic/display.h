#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*---------------------------------------------------------------------------*/

class MyDisplay
{
public:
       int screen_width;
       int screen_height;

       GLFWwindow* mainWindow;

public:
       MyDisplay(int screen_width, int screen_height, bool fullscreen, bool vsync);
       virtual ~MyDisplay();

       void Clear(float r, float g, float b, float a);
       void SwapBuffers();
};

#endif
