#include <app.h>
#define GL_SILENCE_DEPRECATION 1

const GLfloat positions[] = {
    -1, -1, 0,
     1, -1, 0,
     1,  1, 0,
    -1, -1, 0,
     1,  1, 0,
    -1,  1, 0
};
    
const GLchar* vertex120 = R"END(
#version 120
attribute vec3 inPosition;
void main() {
    gl_Position = vec4(inPosition, 1);
}
)END";

const GLchar* raster120 = R"END(
#version 120
uniform vec2 res;
void main() {
    // if (gl_FragCoord.y > 300) {
        // gl_FragColor = vec4(1, 0, 0, 1);    
    // } else {
        float i = 1 - gl_FragCoord.y / res.y;
        gl_FragColor = vec4(i, i, i, 1);
    // }
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



    // send data to GPU

    GLuint positionsData;
    glGenBuffers(1, &positionsData);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

     // attributes
    GLuint attribPosition;

    attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint uniformRes;
    uniformRes = glGetUniformLocation(shaderProgram, "res");
    glUniform2f(uniformRes, 600.f, 600.f);

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
