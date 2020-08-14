#include <app.h>

int main(void) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Circle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        double xCenter = 0;
        double yCenter = 0;
        double radius = 1;

        double rx = xCenter;
        double ry = yCenter - radius;

        drawCircle(1, 0, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void drawCircle(double red, double green, double blue) {
    double radius = 1;
    const double angle = 3.1415926 * 2 / steps;
    double oldX = 0;
    double oldY = 1;
    for (int i = 0; i <= steps; i++) {
        double newX = radius * sin(angle * i);
        double newY = -radius * cos(angle * i);
        glColor3f(red, green, blue);
        glBegin(GL_TRIANGLES);
        glVertex3f(0, 0, 0);
        glVertex3f(oldX, oldY, 0);
        glVertex3f(newX, newY, 0);
        glEnd();
        oldX = newX;
        oldY = newY;
    }
}