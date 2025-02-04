#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
// #define STB_PERLIN_IMPLEMENTATION
// #include <stb_perlin.h>
#include <cstdint>
#include <vector>
#include <chunk.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <iostream>
#include <settings.hpp>
#include "benchmark.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// void generateChunks(std::vector<std::vector<Chunk*>>& chunks);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


// const int MAP_SIZE = 3;
// settings

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
std::vector<std::vector<Chunk*>>chunks;
std::vector<glm::vec2> cords_chunks;
uint frames = 0;
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
    glfwSwapInterval(0);

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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    if (DEBUG) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("/home/user/RederGL/RederGL/Shaders/instanced_tight.vs", "terrain_level.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_monotone_cube[] = {
        // Bottom face (y = 0)
        0.0f, 0.0f, 0.0f, // 0
        1.0f, 0.0f, 0.0f, // 1
        1.0f, 0.0f, 1.0f, // 2
        0.0f, 0.0f, 1.0f, // 3
    
        // Top face (y = 1)
        0.0f, 1.0f, 0.0f, // 4
        1.0f, 1.0f, 0.0f, // 5
        1.0f, 1.0f, 1.0f, // 6
        0.0f, 1.0f, 1.0f, // 7
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
        // Bottom face (y = 0)
        0, 1, 2, // Triangle 1
        0, 2, 3, // Triangle 2
    
        // Top face (y = 1)
        4, 6, 5, // Triangle 1
        4, 7, 6, // Triangle 2
    
        // Front face (z = 1)
        3, 2, 6, // Triangle 1
        3, 6, 7, // Triangle 2
    
        // Back face (z = 0)
        0, 5, 1, // Triangle 1
        0, 4, 5, // Triangle 2
    
        // Left face (x = 0)
        0, 3, 7, // Triangle 1
        0, 7, 4, // Triangle 2
    
        // Right face (x = 1)
        1, 6, 2, // Triangle 1
        1, 5, 6, // Triangle 2
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

    // glm::vec3 positions[256*256];
    // for (int i = 0;i!=16*16;i++)
    // {
    //     for (int j =0;j!=16*16;j++)
    //     {
    //         positions[i*16*16+j] = glm::vec3(1*i,sqrt(i*i +j*j), 1*j);
    //         // positions[i*16+j] = glm::vec3((float)i,-29.0f, (float)j);
    //     }
    // }
        
    const int CHUNK_ROWS = 64;
    const int CHUNK_COLS = 64;
    
    chunks = std::vector<std::vector<Chunk*>>(CHUNK_ROWS, std::vector<Chunk*>(CHUNK_COLS));

    for (int x = 0; x!=CHUNK_ROWS; x++)
    {
        for (int z = 0; z != CHUNK_COLS; z++)
        {
            chunks[x][z] = new Chunk(x,z);
            cords_chunks.push_back(glm::vec2((float)x, (float) z));
       }
    }
    // for (int i = 0; i!=4;i++)
    // {
        // glBufferData(GL_ARRAY_BUFFER, chunks[0][0]->visible_blocks. (float), nullptr, GL_STATIC_DRAW);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, size2 * sizeof(float), data2);
    // }
    GLuint VBO_monotone_cube, VAO_monotone_cube, EBO_monotone_cube,instanceVBO, instanceVBO_chunkCoord;

    glGenVertexArrays(1, &VAO_monotone_cube);
    glGenBuffers(1, &VBO_monotone_cube);
    glGenBuffers(1, &EBO_monotone_cube);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &instanceVBO_chunkCoord);

    glBindVertexArray(VAO_monotone_cube);


    

    glBindBuffer(GL_ARRAY_BUFFER, VBO_monotone_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_monotone_cube), vertices_monotone_cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_monotone_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_monotone_cube), indices_monotone_cube, GL_STATIC_DRAW);

    // Calculate total buffer size
    size_t totalSize = 0;
    for (int i = 0; i < CHUNK_ROWS; i++) {
        for (int j = 0; j < CHUNK_COLS; j++) {
            totalSize += chunks[i][j]->visible_blocks.size() * sizeof(uint32_t);
        }
    }
    
    // Allocate buffer once
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
    
    // Upload chunks at correct offsets
    size_t offset = 0;
    for (int i = 0; i < CHUNK_ROWS; i++) {
        for (int j = 0; j < CHUNK_COLS; j++) {
            std::vector<uint32_t>& visible_blocks = chunks[i][j]->visible_blocks;
            size_t chunkSize = visible_blocks.size() * sizeof(uint32_t);
            glBufferSubData(GL_ARRAY_BUFFER, offset, chunkSize, visible_blocks.data());
            offset += chunkSize; // Move to next chunk's location
        }
    }

    
    
    // Unbind buffer
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_chunkCoord);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float)* CHUNK_ROWS * CHUNK_COLS* 2, cords_chunks.data(), GL_DYNAMIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    

    // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_monotone_cube);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(uint32_t), (void*)0);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Enable vertex attribute 2 (aChunkCoord)
    // glEnableVertexAttribArray(2);
    // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_chunkCoord);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);
    
    // // glVertexAttribDivisor(2, 256*2); // Advance once per chunk
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // for (uint32_t packed : chunks[0][0]->visible_blocks)
    // {
    //     uint x = (packed >> 28) & 0xF;       // Extract bits 28-31 (4 bits)
    //     uint y = (packed >> 20) & 0xFF;      // Extract bits 20-27 (8 bits)
    //     uint z = (packed >> 16) & 0xF;       // Extract bits 16-19 (4 bits)
    //     uint blockID = packed & 0xFFFF;      // Extract bits 0-15 (16 bits)
    //     std::cout << packed << ": " << x << " " << y << " " << z << " " << blockID << "\n";
    // }

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

    
    // int terrain[MAP_SIZE][MAP_SIZE];
    srand(time(0));
    SEED_X = (rand())%100003, SEED_Z = (rand())%100151;

  


    // render loop
    // -----------
    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.2f, 1000.f);
    ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    
    ChunkBenchmark bcmrk;
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
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
        
        // size_t totalInstances = 0;
        // for (int i = 0; i < CHUNK_ROWS; i++) {
        //     for (int j = 0; j < CHUNK_COLS; j++) 
        //     {
        //         totalInstances += chunks[i][j]->visible_blocks.size();          
        //     }
        // }


        
        uint32_t baseInstance = 0;  // Keeps track of where the next chunk starts
        for (int chunk_x = 0; chunk_x!=CHUNK_COLS; chunk_x++)
        {
            for (int chunk_z = 0; chunk_z != CHUNK_ROWS; chunk_z++)
            {
           
            
            // for (const Chunk& chunk : chunks) {
            // std::cout << cords_chunks[chunk_x*MAP_SIZE+chunk_z][0];
                ourShader.setVec2("chunkPosition", (float) chunk_x, (float) chunk_z);
                glDrawElementsInstancedBaseInstance(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, chunks[chunk_x][chunk_z]->visible_blocks.size(), baseInstance);
                baseInstance += chunks[chunk_x][chunk_z]->visible_blocks.size();  // Move to the next chunk's instance range
                
                // GLenum err;
                // while ((err = glGetError()) != GL_NO_ERROR) {
                //     std::cerr << "OpenGL Error: " << err << std::endl;
                // }
            }
        }
        frames ++;
        bcmrk.frame(deltaTime, CHUNK_COLS*CHUNK_ROWS);
   
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

    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &instanceVBO_chunkCoord);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    std::cout << "FPS: " << (float)frames / glfwGetTime() << "\n";
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float currentFrame = glfwGetTime();
    
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);}

    if ( (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) && (DEBUG_PRESS_HINT == true)){
        if (DEBUG)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        DEBUG = !DEBUG;
        DEBUG_PRESS_HINT = false;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        DEBUG_PRESS_HINT = true;
    }
    if ( glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // generateChunks(chunks);/
    }
    
    float cameraSpeed = static_cast<float>(25.0 * deltaTime);
    if (FPS_CAMERA)
    {
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
    else 
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraFront*cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraFront*cameraSpeed; 
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

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

// void generateChunks(std::vector<std::vector<Chunk*>>& chunks)
// {
//     SEED_X = (rand())%100003, SEED_Z = (rand())%100151;
//     for (int x = 0; x!=MAP_SIZE; x++)
//     {
//         for (int z = 0; z != MAP_SIZE; z++)
//         {
//             if (chunks[x][z] != nullptr)
//             {
                
//                 delete chunks[x][z];
//             }
//             chunks[x][z] = new Chunk(x,z);
//         }
//     }
//     std::cout << "chunks re-gen: " << MAP_SIZE * MAP_SIZE << std::endl;
// }