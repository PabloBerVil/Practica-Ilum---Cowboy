#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define STB_IMAGE_IMPLEMENTATION
/*#define TINYOBJLOADER_IMPLEMENTATION*/

#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include "detail/func_matrix.hpp"
#include <math.h> 

#include <iostream>
//#include <vector>
//#include <iosfwd>
//#include <sstream>

#include "Shader.h"
#include <iosfwd>
#include <string>
#include <xiosbase>
#include <fstream>
#include <vector>


#include "VertexVec.h"
#include "Buffer.h"
#include "State.h"
#include "Camera.h"
#include "World.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "light.h"

#define RAD 0.01745329f
#define SCREEN_WIDTH 800.f
#define SCREEN_HEIGHT 600.f
#define PI 3.14159265f

int init();


const float FIXEDTICK = 1.f / 60.f;

double aTime = 0;
double deltaTime = 0;
double previousTime = 0;
float camSpeed=10.f;


int main()
{
    if (!glfwInit())
    {
        std::cout << "not initialized glfw" << std::endl;
        return -1;
    }

    // create window
      //glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* view = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "", nullptr, nullptr);
    if (!view)
    {
        std::cout << "not created opengl window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(view);


    //Init
	glewInit();

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	State::defaultShader = new Shader(GL_VERTEX_SHADER, "data/vertex.glsl", GL_FRAGMENT_SHADER, "data/fragment.glsl");

    World* world = new World();


    Camera* cam1 = new Camera();
    cam1->setClearColor(glm::vec3(0, 0, 0));
    cam1->setViewport(glm::ivec4(0, 0, 800, 600));
    cam1->setPosition(glm::vec3(8, 1, 58));
    cam1->setLookPoint(glm::vec3(0,0,-1));
    world->addEntity(cam1);

    Mesh* box = Mesh::load("data/box_stack.obj");
    Model* boxMod = new Model(*box);
    boxMod->setPosition(glm::vec3(-7, 0, 2));
    boxMod->setScale(glm::vec3(1, 1, 1));
    boxMod->setRotation(glm::vec3(0, 0, 0));
    world->addEntity(boxMod);

    Mesh* cowboy = Mesh::load("data/gunslinger.obj");
    Model* cowboyMod = new Model(*cowboy);
    cowboyMod->setPosition(glm::vec3(7.f, 0.f, 2.f));
    cowboyMod->setScale(glm::vec3(1, 1, 1));
    cowboyMod->setRotation(glm::vec3(0, 0, 0));
    world->addEntity(cowboyMod);



    Light* light1 = new Light();
    light1->setColor(glm::vec3(1.f,1.f,1.f));
    light1->setType(lightType::Directional);
    light1->setPosition(glm::vec3(1,1,1));   
    light1->setLinearAtteniation(1.f);
    world->addEntity(light1);

    Light* light2 = new Light();
    light2->setColor(glm::vec3(2.f,0.f,0.f)); //esto no eticamente correcto, pero se nota mas la luz
    light2->setType(lightType::Point);
    light2->setPosition(glm::vec3(5.f,1.5f,0.5f));
    light2->setLinearAtteniation(0.2f);
    world->addEntity(light2);

    float angle = 0;

    world->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

    previousTime = glfwGetTime();


    while (!glfwWindowShouldClose(view) && !glfwGetKey(view, GLFW_KEY_ESCAPE))
    {

        int screenWidth, screenHeight;
        glfwGetWindowSize(view, &screenWidth, &screenHeight);

        deltaTime += glfwGetTime() - previousTime;
        previousTime = glfwGetTime();


        while (deltaTime >= FIXEDTICK)
        {
            deltaTime -= FIXEDTICK;

            glm::vec3 actuallightpos = light2->getPosition();
      
            actuallightpos.x = 7 + (30 * cos((angle * PI) / 180));
            actuallightpos.z = 2 + (30 * sin((angle * PI) / 180));

            light2->setPosition(actuallightpos);
            boxMod->setPosition(actuallightpos);

            angle += 50 * FIXEDTICK;

            if (angle > 360.f)
            {
                angle -= 360;
            }

            world->update(FIXEDTICK);

        }
        world->draw();


        glm::vec3 vec1(0, 0, 0);
        if (glfwGetKey(view, GLFW_KEY_RIGHT) || glfwGetKey(view, GLFW_KEY_D))
        {
            vec1.x += 1;
        }
        else if (glfwGetKey(view, GLFW_KEY_LEFT) || glfwGetKey(view, GLFW_KEY_A))
        {
            vec1.x -= 1;
        }

        if (glfwGetKey(view, GLFW_KEY_UP) || glfwGetKey(view, GLFW_KEY_W))
        {
            vec1.z -= 1;
        }
        else if (glfwGetKey(view, GLFW_KEY_DOWN) || glfwGetKey(view, GLFW_KEY_S))
        {
            vec1.z += 1;
        }

        if (glfwGetKey(view, GLFW_KEY_Q))
        {
            vec1.y -= 1;
        }
        else if (glfwGetKey(view, GLFW_KEY_E))
        {
            vec1.y += 1;
        }

        vec1 = vec1 * camSpeed * FIXEDTICK;
        cam1->move(vec1);

        glfwSwapBuffers(view);
        glfwPollEvents();
    }

    // shutdown
    glfwTerminate();
}