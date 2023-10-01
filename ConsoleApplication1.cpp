#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h>

using namespace std;

int windowSizeY = 1080;
int windowSizeX = 1920;

const char* loadFile(const char* fileName) {
    FILE* shader;
    char* chars = new char[8000];
    memset(chars, 0, 8000);
    fopen_s(&shader, fileName, "rb");
    fread(chars, sizeof(char), 8000, shader);
    fclose(shader);
    return chars;
}

const char* vertex_shader = loadFile("vertexShader.glsl");
const char* fragment_shader = loadFile("fragmentShader.glsl");;

void _init_() {
    gladLoadGL();
}

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    windowSizeX = width;
    windowSizeY = height;
    glViewport(0, 0, windowSizeX, windowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int node) {
    if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

GLfloat points[12] = {-1.0,  1.0, 0.0, 
                      -1.0, -1.0, 0.0,
                       1.0, -1.0, 0.0,
                       1.0,  1.0, 0.0};


GLuint createShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
    glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);

    int success;
    GLchar inf[500];

    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        cout << "Vertex shader problem!\n";
        glGetShaderInfoLog(vertexShader, 500, NULL, inf);
        for (int i = 0; i < 500; i++)
            cout << inf[i];
        cout << '\n';
    }

    glCompileShader(fragmentShader);
    success = 1;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        cout << "Fragment shader problem!\n";
        glGetShaderInfoLog(fragmentShader, 500, NULL, inf);
        for (int i = 0; i < 500; i++)
            cout << inf[i];
        cout << '\n';
    }

    GLuint sp = glCreateProgram();
    glAttachShader(sp, vertexShader);
    glAttachShader(sp, fragmentShader);
    glLinkProgram(sp);

    success = 1;
    glGetProgramiv(sp, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(sp, 500, NULL, &inf[0]);
        cout << "Bad linkin'!\n";
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return sp;
}

int main(void)
{
    setlocale(0, "");
    /* Initialize the library */
    if (!glfwInit())
    {
        cout << "GLFW initialisation failed!\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(1080, 1080, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    _init_();

    cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glClearColor(0, 0, 0, 1);
    
    GLuint sp = createShaderProgram();

    GLuint pointsVBO = 0;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    //GLint vertexResolution = glGetUniformLocation(sp, "resolution");
    //glUniform2f(vertexResolution, 640.0, 480.0);

    GLint success;
    GLchar inf[500];


    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    
    GLfloat timerStart;
    GLfloat timerEnd;

    GLint timeLocation = glGetUniformLocation(sp, "time");

    bool firstIt = 1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (firstIt) {
            timerStart = clock();
            firstIt = 0;
        }
        timerEnd = clock();

        glUniform1f(timeLocation, (timerEnd - timerStart) / 1000.0);
        glUseProgram(sp);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    
    }
    
    glfwTerminate();
    return 0;
}