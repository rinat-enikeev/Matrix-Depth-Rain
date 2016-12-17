//
//  main.cpp
//  Matrix Depth Rain
//
//  Created by Rinat Enikeev on 17/12/2016.
//  Copyright © 2016 Rinat Enikeev. All rights reserved.
//

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// common
#include "shader.hpp"
#include "texture.hpp"
#include "text2D.hpp"

// other
#include <errno.h>
#include <limits.h>

int intFromArg(const char *arg) {
    char *p;
    int num;
    
    errno = 0;
    long conv = strtol(arg, &p, 10);
    
    // Check for errors: e.g., the string does not represent an integer
    // or the integer is larger than int
    if (errno != 0 || *p != '\0' || conv > INT_MAX) {
        fprintf(stderr, "usage: %s width height\n", arg);
        exit(1);
    } else {
        // No error
        num = conv;
    }
    return num;
}

int main(int argc, const char * argv[]) {
    
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "usage: %s width height\n", argv[0]);
        exit(1);
    }
    
    int width = intFromArg(argv[1]);
    int height = intFromArg(argv[2]);
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( width, height, "Matrix", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    
    // Dark background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
 
    // Initialize our little text library with the matrix font
    initText2D( "Holstein.DDS" );
    
    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        printText2D("abcdefghijk", 0, 0, 80);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Delete the text's VBO, the shader and the texture
    cleanupText2D();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}
