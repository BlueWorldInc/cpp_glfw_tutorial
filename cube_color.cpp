#include <app.h>
#define GL_SILENCE_DEPRECATION 1

const GLchar* vertex120 = R"END(
#version 120
attribute vec3 position;
attribute vec3 color;
varying vec3 outColor;
uniform float time;
uniform mat4 matrix;
uniform mat4 projection;
void main() {
    float theta = time;
    
    float co = cos(theta);
    float si = sin(theta);
    
    mat4 rotationY = mat4(co, 0, si,  0,
                          0,  1,  0,  0,
                          -si,  0, co, 0,
                          0,  0,  0,  1);

    co = cos(theta/2.);
    si = sin(theta/2.);

    mat4 rotationX = mat4(1, 0, 0, 0,
                          0, co, -si, 0,
                          0, si, co, 0,
                          0, 0, 0, 1);
    outColor = color;
    gl_Position = matrix * rotationY * rotationX * vec4(position, 1);
}
)END";

const GLchar* raster120 = R"END(
#version 120
varying vec3 outColor;
uniform float time;
void main() {
    gl_FragColor = vec4(outColor, 1);
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

    GLfloat vertices[] = {
        -1, -1, +1,
        -1, +1, +1,
        +1, +1, +1,
        +1, -1, +1,
        -1, -1, -1,
        -1, +1, -1,
        +1, +1, -1,
        +1, -1, -1,
    };

    GLfloat colors[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 0,
        0, 1, 1,
        0, 1, 0,
        1, 0, 0,
    };

    GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 5,
        0, 5, 1,
        1, 5, 6,
        1, 6, 2,
        3, 2, 6,
        3, 6, 7,
        4, 0, 7,
        7, 0, 3,
        7, 6, 5,
        7, 5, 4,
    };

    GLuint verticesBuf;
    glGenBuffers(1, &verticesBuf);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colorsBuf;
    glGenBuffers(1, &colorsBuf);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint indicesBuf;
    glGenBuffers(1, &indicesBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


     // attributes
    GLuint attribPosition;
    attribPosition = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBuf);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint attribColor;
    attribColor = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(attribColor);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuf);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    GLfloat matrix[] = {
        0.5, 0,   0,   0,
        0,   0.5, 0,   0,
        0,   0,   0.5, 0,
        0,   0,   0,   1
    };
    
    GLuint attribMatrix;
    attribMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(attribMatrix, 1, GL_FALSE, matrix);

    GLuint uniformTime;
    uniformTime = glGetUniformLocation(shaderProgram, "time");

    glEnable(GL_CULL_FACE); 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {


        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        glUniform1f(uniformTime, time);

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
    }

    glfwTerminate();
    return 0;
}
