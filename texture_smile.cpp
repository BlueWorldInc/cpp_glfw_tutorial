#include <app.h>
#define GL_SILENCE_DEPRECATION 1

const GLchar* vertex120 = R"END(
#version 120
attribute vec3 inPosition;
attribute vec2 inUvs;
varying vec2 outUvs;
uniform mat4 matrix;
void main() {
    outUvs = inUvs;
    gl_Position = matrix * vec4(inPosition, 1);
}
)END";

const GLchar* raster120 = R"END(
#version 120
uniform vec2 res;
uniform float time;
varying vec2 outUvs;
uniform sampler2D tex;
void main() {
    gl_FragColor = texture2D(tex, outUvs);
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

    GLfloat positions[] = {
        -1, -1, 0,
        -1,  1, 0,
         1, -1, 0,
         1, -1, 0,
        -1,  1, 0,
         1,  1, 0
    };

    GLuint positionsData;
    glGenBuffers(1, &positionsData);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    GLfloat uvs[] = {
        0, 0,
        0, 1,
        1, 0,
        1, 0,
        0, 1,
        1, 1
    };
    
    GLuint uvsData;
    glGenBuffers(1, &uvsData);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    // texture

    #pragma pack(push)
    #pragma pack(1)

    struct Pixel {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    };

    Pixel O = {0x00, 0xFF, 0x00};
    Pixel X = {0xFF, 0xFF, 0x00};
    Pixel o = {0x11, 0x11, 0x11};
    Pixel x = {0x33, 0x33, 0x11};

    Pixel texture[] = {
        o, o, o, o, o,
        o, O, o, O, o,
        o, o, o, o, o,
        X, x, o, x, X,
        x, X, X, X, x,
    };

    #pragma pack(pop)

    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 5, 5, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*) texture);

    GLuint attribTex = glGetAttribLocation(shaderProgram, "tex");
    glUniform1i(attribTex, 0);




     // attributes
     GLfloat matrix[] = {
        -1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    GLuint attribMatrix;
    attribMatrix = glGetUniformLocation(shaderProgram, "matrix");
    glUniformMatrix4fv(attribMatrix, 1, GL_FALSE, matrix);

    GLuint attribPosition;
    attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, positionsData);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint attribUvs;
    attribUvs = glGetAttribLocation(shaderProgram, "inUvs");
    glEnableVertexAttribArray(attribUvs);
    glBindBuffer(GL_ARRAY_BUFFER, uvsData);
    glVertexAttribPointer(attribUvs, 2, GL_FLOAT, GL_FALSE, 0, 0);
    

    GLuint uniformRes;
    uniformRes = glGetUniformLocation(shaderProgram, "res");
    glUniform2f(uniformRes, 600.f, 600.f);

    GLuint uniformTime;
    uniformTime = glGetUniformLocation(shaderProgram, "time");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {


        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        glUniform1f(uniformTime, time);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(20000);
    }

    glfwTerminate();
    return 0;
}
