#include <app.h>

int main(void) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 1080, "Solar System", NULL, NULL);



    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.01, 0.01, 1);

    double angleMercury = 0;
    double angleVenus = 0;
    double angleEarth = 0;
    double angleMoon = 0;
    double angleMars = 0;
    double angleJupiter = 0;
    double angleSaturn = 0;
    double angleUranus = 0;
    double angleNeptune = 0;
    double anglePluto = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        angleEarth += 1;
        angleMoon += 3;
        angleMercury += 1 / 0.24;
        angleVenus += 1 / 0.61;
        angleMars += 1 / 1.88;
        angleJupiter += 1 / 11.86;
        angleSaturn += 1 / 29.46;
        angleUranus += 1 / 84.01;
        angleNeptune += 1 / 164.79;
        anglePluto += 1 / 357;

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(1, 1, 0);

        {
            // earth and moon

            glPushMatrix();
            glRotatef(angleEarth, 0, 0, 1);
            glTranslatef(0, 10, 0);
            glScalef(0.1, 0.1, 1);
            drawCircle(0, 0.3, 1);

            glPushMatrix();
            glRotatef(angleMoon, 0, 0, 1);
            glTranslatef(0, 3, 0);
            glScalef(0.5, 0.5, 1);
            drawCircle(0.5, 0.5, 0.5);
            
            glPopMatrix();
            glPopMatrix();
        }

        {
            
            drawPlanet(angleMercury, 3.87, 0.0382, 0.3, 0.3, 0.9);
            drawPlanet(angleVenus, 7.23, 0.0949, 0.9, 0.3, 0.3);
            drawPlanet(angleMars, 15.24, 0.0532, 1, 0.3, 0);
            drawPlanet(angleJupiter, 52.03, 1.119, 0.5, 0.5, 0.5);
            drawPlanet(angleSaturn, 95.29, 0.926, 0.4, 0.6, 0.4);
            drawPlanet(angleUranus, 191.9, 0.401, 0.3, 0.3, 0.9);
            drawPlanet(angleNeptune, 300.6, 0.388, 0.1, 0.4, 0.9);
            drawPlanet(anglePluto, 497.3, 0.018, 0.7, 0.7, 0.7);

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

void drawPlanet(double angle, double distance, double diameter, double red, double green, double blue) {
    glPushMatrix();
    glRotatef(angle, 0, 0, 1);
    glTranslatef(distance, 0, 0);
    glScalef(diameter, diameter, 1);
    drawCircle(red, green, blue);
    glPopMatrix();
}