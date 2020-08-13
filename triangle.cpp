#include <iostream>
#include <GLFW/glfw3.h>

int main() {
    GLFWwindow * window;
    
    if (!glfwInit()) {
        std::cout << "Init error";
        return -1;
    }
    
    window = glfwCreateWindow(800,600,"Hello",0,0);
    if (!window) {
        std::cout << "Window creation error";
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // render loop
    while (!glfwWindowShouldClose(window)) {

        glClearColor(1.0, 1.0, 1.0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);

        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
}
