#include <app.h>

const GLchar* vertex120 = R"END(
#version 120
uniform mat4 matrix;
attribute vec4 inColor;
attribute vec4 inPosition;
uniform mat4 matrix;
varying vec4 outColor;

void main() {
    outColor = inColor;
    gl_Position= matrix * inPosition;
}

)END";

const char* vertex150 = R"END(
#version 150
in vec4 inColor;
in vec4 inPosition;
out vec4 outColor;

void main() {
    outColor = inColor;
    gl_Position = inPosition;
}

)END";

const char* raster120 = R"END(
#version 120
varying vec4 outColor;

void main() {
    gl_FragColor = outColor;
}

)END";

const char* raster150 = R"END(
#version 150
in vec4 outColor;
out vec4 color;

void main() {
    color = outColor;
}
)END";

int main(void) {

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 1080, "Shaders", NULL, NULL);



    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // vertex shader

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex120, 0);
    glCompileShader(vertexShader);
    GLint compilationStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        char message[256];
        glGetShaderInfoLog(vertexShader, sizeof(message), 0, &message[0]);
        printf("%s", message);
        exit(1);
    }

    // fragment shader

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &raster120, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        char message[256];
        glGetShaderInfoLog(fragmentShader, sizeof(message), 0, &message[0]);
        printf("%s", message);
        exit(1);
    }

    // shader program

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        GLchar message[256];
        glGetProgramInfoLog(shaderProgram, sizeof(message), 0, &message[0]);
        printf("%d", message);
        exit(1);
    }

    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {


        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(1, 1, 0);

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