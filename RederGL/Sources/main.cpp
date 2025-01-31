

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glCullFace(GL_FRONT);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("transform.vs", "color.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_monotone_cube[] = {
        0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 
        1.0f, 1.0f, 0.0f, 
        1.0f, 0.0f, 0.0f, 

        0.0f, 0.0f, 1.0f, 
        0.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 1.0f, 
    };
    // float vertices_monotone_cube[] = 
    // {
    //     -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    //     0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f
    // };
    // unsigned int indices[] = 
    // {
    //     0, 1, 2, 
    //     0, 2, 3
    // };
    unsigned int indices_monotone_cube[] = {
        // down face
        0, 1, 2, 
        0, 2, 3,

        // up face
        4, 5, 6, 
        4, 6, 7,

        // left face 
        0, 4, 5,
        0, 1, 5,

        // right face
        3, 7, 6,
        3, 6, 2,

        // back face
        1, 5, 2,
        5, 6, 2,

        // front face
        0, 4, 7, 
        0, 7, 3

    };
    // world space positions of our cubes
    // glm::vec3 cubePositions[] = {
    //     glm::vec3( 0.0f, -2.0f,  0.0f),
    //     glm::vec3( 0.0f, -2.0f, 1.0f),
    //     glm::vec3( 0.0f, -2.0f,  2.0f),
    //     glm::vec3( 1.0f, -2.0f,  0.0f),
    //     glm::vec3( 1.0f, -2.0f,  1.0f),
    //     glm::vec3( 1.0f, -2.0f,  2.0f),
    //     glm::vec3( 2.0f, -2.0f,  0.0f),
    //     glm::vec3( 2.0f, -2.0f,  1.0f),
    //     glm::vec3( 2.0f, -2.0f, 2.0f),
    // };

    GLuint VBO_monotone_cube, VAO_monotone_cube, EBO_monotone_cube;

    glGenVertexArrays(1, &VAO_monotone_cube);
    glGenBuffers(1, &VBO_monotone_cube);
    glGenBuffers(1, &EBO_monotone_cube);

    glBindVertexArray(VAO_monotone_cube);

    

    glBindBuffer(GL_ARRAY_BUFFER, VBO_monotone_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_monotone_cube), vertices_monotone_cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_monotone_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_monotone_cube), indices_monotone_cube, GL_STATIC_DRAW);
    

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    // unsigned int texture1;
    // // texture 1
    // // ---------
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // unsigned char *data = stbi_load("/home/user/RederGL/RederGL/Textures/container.jpg", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    // texture 2
    // ---------
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // data = stbi_load("/home/user/RederGL/RederGL/Textures/face.png", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    // ourShader.use();
    // ourShader.setInt("texture1", 0);
    // ourShader.setInt("texture2", 1);

    int terrain[160][160];
    for (int x = 0; x!=160; x++)
    {
        for (int z = 0; z != 160; z++)
        {
            
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        
        // float angle = 20.0f * i;
        double sin_v = 0.f, cos_v = 0.f;
        sincos(glfwGetTime(), &sin_v, &cos_v);
        float final = (glfwGetTime());
        float lacunarity = 2.0f, gain = 0.3f;
        int octaves = 5;
    
        // Scale the coordinates and add an offset to avoid integer grid points
        float scale = 0.02f;
        terrain[x][z] = stb_perlin_fbm_noise3((float)x * scale, 0.0f, (float)z * scale, lacunarity, gain, octaves)*10;
        }
    }
    // render loop
    // -----------
    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 160.0f);
    ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glBindVertexArray(VAO_monotone_cube);
    
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

         // bind textures on corresponding texture units
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader

        // create transformations
        // glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // glm::mat4 projection    = glm::mat4(1.0f);
       
        // glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // float radius = 10.0f;

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);
        // ourShader.setMat4("model", model);

        // // pass transformation matrices to the shader
        // ourShader.setMat4("view", view);
        // ourShader.setVec4("color", glm::vec4(1.f, 1.f,1.f,0.f));

        // render boxes
        

        for (int x = 0; x!=160; x++)
        {
            for (int z = 0; z != 160; z++)
            {
                
            // calculate the model matrix for each object and pass it to shader before drawing
            
            // for (int y = terrain[x][z]; y!= terrain[x][z];y++)
            // {
                glm::mat4 model = glm::mat4(1.0f);
                int y = terrain[x][z]-25;
                // std::cout << x << " " << y << " " << z <<" ";
                model = glm::translate(model, glm::vec3(x, y, z));   
            
        
                // model = glm::scale(model, glm::vec3(final, 1.f, final));
                // model = glm::rotate(model, final, glm::vec3(1.0f, 1.f, 1.f));
                // // model = glm::rotate(model, 5.f, glm::vec3( i%4, i%3, i%2));
                ourShader.setMat4("model", model);
    
    
                // glDrawArrays(GL_TRIANGLES, 0, 180);
                // glDrawElements(GL_TRIANGLES, sizeof(indices_monotone_cube) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            // }
            // std::cout << "\n\n";

            }
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO_monotone_cube);
    glDeleteBuffers(1, &VBO_monotone_cube);
    glDeleteBuffers(1, &EBO_monotone_cube);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
int frame = 0;
float fps = 0.0f;
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    frame ++;
    float currentFrame = glfwGetTime();
    
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    fps += deltaTime;
    if (frame == 60 ){
         std::cout << 60.0/fps << "\n";
         fps = 0.0f;
         frame = 0;
        }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);}


    float cameraSpeed = static_cast<float>(25.0 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += (glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z))*cameraSpeed) ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= (glm::normalize(glm::vec3(cameraFront.x, 0, cameraFront.z))*cameraSpeed) ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    cameraPos.y = 0.0f;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}