#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool init();
void clear();

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int VBO, VAO, EBO, shaderProgram;

const char *vertex_shader_sources = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"//位置变量的属性位置值设置为0
                                    "layout (location = 1) in vec3 aColor;\n"//颜色变量属性的位置值设置为1
                                    "\n"
                                    "out vec3 ourColor;\n"//为片段着色器指定一个颜色输出
                                    "void main()\n"
                                    "{\n"
                                    "    gl_Position = vec4(aPos, 1.0);\n"
                                    "    ourColor = aColor;\n" //将ourColor设置为从顶点数据那里得到的输入颜色
                                    "}\n\0";
const char *fragment_shader_sources = "#version 330 core\n"
                                      "out vec4 FragColor;\n"
                                      "in vec3 ourColor;\n"//在OpenGL中设置这个变量
                                      "void main()\n"
                                      "{\n"
                                      "   FragColor = vec4(ourColor, 1.0f);\n"
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

        //activate the shaderProgram
        glUseProgram(shaderProgram);

        //update the color of uniform
//        float timeValue = glfwGetTime();
//        float greenValue = sin(timeValue) / 2.0f + 0.5f;
//        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //seeing as we only have a single VAO there`s no need to bind it every time, but we`ll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);//draw triangle
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw rectangle(combined by two triangle)

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
    //--------------------------------
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_sources, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, logInfo);
        std::cout << " failed to compile vertex shader " << logInfo << std::endl;
        return false;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_sources, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr, logInfo);
        std::cout << "failed to compile fragment shader" << logInfo << std::endl;
        return false;
    }

    //link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, logInfo);
        std::cout << "Program: link failed" << logInfo << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //set up vertex data(and buffers), and configure vertex attributes
    float vertices[] = {
            //位置            //颜色
            .5f, -.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left
            -.5f, -.5f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom right
            0.0f, .5f, 0.0f, 0.0f, 0.0f, 1.0f //top
    };

//    unsigned int indices[] = {
//            0, 1, 3, //the first triangle
//            1, 2, 3  //the second triangle
//    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);

    //bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);

    //tell OpenGL how to parse the vertex data, and apply them to the specific vertex properties
    //---------------------------------------------------------------------------------
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attributes`s bound buffer object,
    //so afterwards, we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //we can unbind the VAO afterwards so other VAO calls won`t accidentally modify this VAO, but this rarely happens
    //modify other VAOs requires a call to glBindVertexArray anyway so we generally don`t unbind VAOs nor VBOs when it`s not directly necessary
    glBindVertexArray(0);

    return true;
}

void clear()
{
    //de-allocate all resources once they`ve outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}


