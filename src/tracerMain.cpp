// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

GLFWwindow* window;

// dimensions of drawing window
int w_width  = 640;
int w_height = 480;

void init() {
    // make pixel buffer with tracer
    // setup OpenGL
}

void display() {
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void keyboard( GLFWwindow *window, int key, int scan, int action, int modes ) {
    switch( key ) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
}

void glfwError( int code, const char *desc ) {
    cerr << "GLFW error " << code << ": " << desc << endl;
    // exit( 2 );
}

int main( void ) {

    // establish 
    glfwSetErrorCallback( glfwError );

    // initialise GLFW
    if ( !glfwInit() ) {
        std::cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // open a window and create its OpenGL context
    window = glfwCreateWindow( w_width, w_height, "wassup B", NULL, NULL);
    if ( !window ) {
        std::cerr << "GLFW FAILED" << endl;
        glfwTerminate();
        return -1;
    }
    // this sets the OpenGL context to our window
    glfwMakeContextCurrent( window );

    // this defines how keyboard events are handled for our window
    glfwSetKeyCallback( window, keyboard );

    // initialize the stuff we need for our rendering
    init();

    // loop until told to exit
    while( !glfwWindowShouldClose(window) ) {
        display();
        glfwSwapBuffers( window );
        glfwWaitEvents();
    }

    // kill everything
    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
