#include <app.h>

// const GLchar* vertex120 = R"END(
// #version 120
// attribute vec4 inColor;
// attribute vec4 inPosition;
// uniform mat4 matrix;
// varying vec4 outColor;

// void main() {
//     outColor = inColor;
//     gl_Position= matrix * inPosition;
// }

// )END";

const GLchar* vertex120 = R"END(
#version 120
attribute vec3 inPosition;
attribute vec3 inColor;
varying vec3 outColor;
void main()
{
    outColor = inColor;
    gl_Position = vec4(inPosition,1);
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

// const char* raster120 = R"END(
// #version 120
// varying vec4 outColor;

// void main() {
//     gl_FragColor = outColor;
// }

// )END";

const GLchar* raster120 = R"END(
#version 120
varying vec3 outColor;
void main()
{
    gl_FragColor = vec4(outColor,1);
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
    window = glfwCreateWindow(600, 600, "Shaders", NULL, NULL);



    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    const char* source;

    // vertex shader

    source = vertex120;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    printf("hey\n");
    glShaderSource(vertexShader, 1, &source, 0);
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

    source = raster120;

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);

    if (compilationStatus == GL_FALSE) {
        char message[256];
        glGetShaderInfoLog(fragmentShader, sizeof(message), 0, &message[0]);
        printf("%s", message);
        exit(1);
    }

    // shader program

    GLint linkStatus;
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

    // VBO setup

    GLfloat positions[] = {
        -1, -1, 0,
        -1,  1, 0,
         1, -1, 0,
         1, -1, 0,
        -1,  1, 0,
         1,  1, 0
    };
    
    GLfloat colors[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 1, 0,
        1, 0, 1,
        0, 1, 1
    };

    // send data to GPU

    GLuint positionsData;
    glGenBuffers(1, &positionsData);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    GLuint colorsData;
    glGenBuffers(1, &colorsData);
    glBindBuffer(GL_ARRAY_BUFFER, colorsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

     // attributes
    GLuint attribPosition;
    GLuint attribColor;

    attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    attribColor = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(attribColor);
    glBindBuffer(GL_ARRAY_BUFFER, colorsData);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {


        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
    }

    glfwTerminate();
    return 0;
}
