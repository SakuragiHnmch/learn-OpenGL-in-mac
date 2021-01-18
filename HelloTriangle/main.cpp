#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool init();
void clear();

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VAOs[2], VBOs[2],  EBO, shaderProgramOrange, shaderProgramYellow;

const char *vertex_shader_sources = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\n\0";
const char *fragment_shader_sources = "#version 330 core\n"
                                      "out vec4 FragColor;\n"
                                      "void main()\n"
                                      "{\n"
                                      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                      "}\n\0";

const char *fragment_shader_sources2 = "#version 330 core\n"
                                      "out vec4 FragColor;\n"
                                      "void main()\n"
                                      "{\n"
                                      "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                      "}\n\0";

int main()
{
    //glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "failed to create glfw window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all OpenGL function pointer
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "failed to initialize GLAD" << std::endl;
        return -1;
    }

    if(!init()){
        glfwSetWindowShouldClose(window, true);
        std::cout << "failed to init" << std::endl;
        return -1;
    }

    //render loop
    //-----------
    while(!glfwWindowShouldClose(window)){
        //input
        processInput(window);

        //render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw triangles
        glUseProgram(shaderProgramOrange);
        //seeing as we only have a single VAO there`s no need to bind it every time, but we`ll do so to keep things a bit more organized
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    clear();

    //glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //make sure the viewport matches the new window dimensions;
    //note that width and height will be significantly larger than specified on retina display
    glViewport(0, 0, width, height);
}

bool init()
{
    int success;
    char logInfo[512];

    //build and compile shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgramOrange = glCreateProgram();
    shaderProgramYellow = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertex_shader_sources, nullptr);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragment_shader_sources, nullptr);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragment_shader_sources2, nullptr);
    glCompileShader(fragmentShaderYellow);

    //link the first program object
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    //then link the second program object using a different fragment shader but the same vertex shader,
    //this is perfectly allowed since the input and out put of both the vertex and fragment shader are equally matched
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    //set up vertex data(and buffers), and configure vertex attributes
    float firstTriangles[] = {
            -0.9f, -0.5f, 0.0f,  // left
            -0.0f, -0.5f, 0.0f,  // right
            -0.45f, 0.5f, 0.0f,  //top
    };

    float secondTriangles[] = {
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top
    };

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attributes
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangles), firstTriangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangles), secondTriangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    return true;
}

void clear()
{
    //de-allocate all resources once they`ve outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);
}


