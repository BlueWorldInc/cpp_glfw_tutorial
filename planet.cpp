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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.1,0.1,1);

    double angleEarth = 0;
    double angleMoon = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        angleEarth += 1;
        angleMoon += 3;

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(1, 1, 0);

        {
            glPushMatrix();
            glRotatef(angleEarth, 0, 0, 1);
            glTranslatef(0, 5, 0);
            glScalef(0.6, 0.6, 1);
            drawCircle(0, 0.3, 1);
            
            glPushMatrix();
            glRotatef(angleMoon, 0, 0, 1);
            glTranslatef(0, 3, 0);
            glScalef(0.5, 0.5, 1);
            drawCircle(0.5, 0.5, 0.5);
            
            glPopMatrix();
            glPopMatrix();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
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