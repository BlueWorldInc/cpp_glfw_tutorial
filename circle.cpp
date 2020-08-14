#include <GLFW/glfw3.h>
#include <math.h>

const unsigned int steps = 100;
const double stepAngle = 3.1415926f * 2.f / steps;

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Circle", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        double xCenter = 0;
        double yCenter = 0;
        double radius = 1;

        double rx = xCenter;
        double ry = yCenter - radius;

        for (int i = 0; i <= steps; i++) {
            double angle = stepAngle * i;
            double newRx = radius * sinf(angle);
            double newRy = -radius * cosf(angle);
            glBegin(GL_TRIANGLES);
            glColor3f(0, 0.75, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(rx, ry, 0);
            glVertex3f(newRx, newRy, 0);
            glEnd();
            rx = newRx;
            ry = newRy;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}