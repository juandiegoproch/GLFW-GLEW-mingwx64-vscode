#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>




static GLuint CompileShader(GLenum type, const std::string& src)
{
    GLuint id = glCreateShader(type);
    const char* cstr_src = src.c_str();

    glShaderSource(id,1,&cstr_src,nullptr);

    glCompileShader(id);

    int result; 
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    
    if (result == GL_FALSE)
    {
        int len;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&len);
        char *errmsg = (char *)alloca(len*sizeof(char)); // alloc on stack dynamically??!

        glGetShaderInfoLog(id,len,&len,errmsg);

        std::cout << "Failed to compile shader" <<std::endl;
        std::cout << errmsg << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint CreateShader(std::string& vertexshader, std::string& fragmentshader)
{
    GLuint program = glCreateProgram();
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER,fragmentshader);
    GLuint vs = CompileShader(GL_VERTEX_SHADER,vertexshader);

    glAttachShader(program,fs);
    glAttachShader(program,vs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(fs);
    glDeleteShader(vs);

    return program;
}

int main(void)
{
    // initialize glfw
    if (glfwInit() != GLFW_TRUE)
        return -1;
    
    // initialize a glfw window
    GLFWwindow* main_window;
    main_window = glfwCreateWindow(640,220,"Hello World", NULL,NULL);

    // check if window has been created
    if (!main_window)
        return -1;

    glfwMakeContextCurrent(main_window);

    if (glewInit() != GLEW_OK)
        return -1;

    //create data
    float positions[6] = {
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f,-0.5f
    };
    // set callbacks and stuff

    GLuint VAO,vertex_buffer_id;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1,&vertex_buffer_id);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),positions,GL_STATIC_DRAW);

    // notifies of an attribute (basically a form of struct)
    glVertexAttribPointer(0, // index in python list
                          2, // qty of things
                          GL_FLOAT, //type of things
                          GL_FALSE, // normalize?
                          2*sizeof(float), // size of structure
                          (const void*)0 //offset inside struct to get to this attr's memory
                          );
    glEnableVertexAttribArray(0); // enable the 0th attribute?

    std::string vshad =
     "#version 330 core\n "
     "layout(location = 0) in vec4 position; \n"
     "void main() \n"
     "{\n"
     "gl_Position = position;\n"
     "}\n"
     ;
    std::string fshad =      
    "#version 330 core\n "
     "out vec4 color; \n"
     "void main() \n"
     "{\n"
     "color = vec4(1.0,1.0,1.0,1.0);\n"
     "}\n"
     ;


    GLuint shader = CreateShader(vshad,fshad);
    glLinkProgram(shader);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(main_window))
    {
        // render stuff
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 3); // We set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
        glBindVertexArray(0);

        //swap buffers lul
        glfwSwapBuffers(main_window);
        // handle events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
