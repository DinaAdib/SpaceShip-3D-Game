#pragma once
#include "Spaceship.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define Reward 1
#define Tunnel 2
#define EndOfGame 3
#define BlackHole 4
#define FinishLine 5

vector<ObjectModel*> Objects(100);
char*Textures[] = {"Meteroids.bmp","Asteroids.bmp","Saturn.bmp","Mars.bmp","Mercury.bmp","Venus.bmp","Earth.bmp","Blackhole.bmp","LightTunnel.bmp", "Gift.bmp","Fuel.bmp","Samboxsa.bmp"};
char*ObjectNames[]={"Meteroids.obj","Asteroids.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Blackhole.obj","Blackhole.obj","Gift.obj","Fuel.obj","Blackhole.obj"};
Buffers ObjectBuffers[] = { Buffers("Meteroids.obj") , Buffers("Asteroids.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj"), Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Blackhole.obj") , Buffers("Blackhole.obj"), Buffers("Gift.obj"), Buffers("Fuel.obj") , Buffers("Blackhole.obj") };

void handleSpaceShipCollision(Spaceship& SS , float dx , float dy , float dz);
int nObjects = 0;

void ObjectLoader(int id , float dx , float dy , float dz, float sx, float sy, float sz, float rx, float ry, float rz)
{
	int type;
	if (id >= 0 && id <= 6)
	{
		type = EndOfGame;
	}
	else if (id == 8)
	{
		type = Tunnel;
	}
	else if (id == 7)
	{
		type = BlackHole;
	}
	else if (id == 9 || id == 10)
	{
		type = Reward;
	}
	else
	{
		type = FinishLine;
	}
	Objects[nObjects] = new ObjectModel(Textures[id],&ObjectBuffers[id],type); 
	glm::vec3 Scaling( sx, sy, sz);
	glm::vec3 Rotation (rx, ry, rz);
	glm::vec3 Translation(dx, dy, dz);
	Objects[nObjects]->constructModelMatrix(Translation,Scaling, Rotation); // translate object and bounding box 
	nObjects++;
}

void SceneReader(string filename) //Scene.txt
{
	int ID; 
	float dX, dY, dZ;
	float sX, sY, sZ;
	float rX, rY, rZ;
	string line;
    ifstream myfile(filename.c_str());
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			myfile >> ID >> dX >> dY >> dZ;
			myfile >> sX >> sY >> sZ;
			myfile >> rX >> rY >> rZ;
			ObjectLoader(ID, dX, dY, dZ,sX,sY,sZ,rZ,rY,rZ);
		}
	}
	else cout << "Unable to open file";
}



int main(void)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetWindowTitle("Model Loading");

    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);
    glfwSetMousePos(1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    /*Vertices , UV and Normals Loading*/
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.

	for (int j = 0; j < 11; j++)
	{
		ObjectBuffers[j].Create();
	}

    Spaceship SpaceGhost;
	ObjectModel SkySphere("Skysphere.bmp", &ObjectBuffers[3], 0);

    // For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;
	float orientation = 0.0f;
	float orientation_sin = 0.0f;
	float BHorientation = 3.14159f / 2.0f;

	SceneReader("Scene.txt");
	
    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

		// Measure speed
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
		nbFrames = 0;
		lastTime += 1.0;
		}
		orientation += 3.14159f / 6.0f * deltaTime;
		orientation_sin = sin(3.14159f / 2.0f * currentTime);

		computeMatricesFromInputs();
		glm::mat4 SceneScaling = scale(mat4(), vec3(100.0f, 100.0f, 100.0f));
		glm::mat4 SceneTranslation = translate(mat4(), getCameraPosition());
		glm::mat4 SceneModel = SceneTranslation* SceneScaling;
		SkySphere.setModelMatrix(SceneModel);
		SkySphere.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

		for (int i = 0; i < nObjects; i++)
		{
			glm::mat4 Scaling = Objects[i]->GetModelScaling();
			glm::mat4 Rotation = Objects[i]->GetModelRotation();
			glm::mat4 Translation = Objects[i]->GetModelTranslation();
			glm::mat4 XYZRotation;
			if (Objects[i]->getType() == EndOfGame)
			{
				XYZRotation = eulerAngleY(orientation) * eulerAngleZ(orientation);
			}
			else if (Objects[i]->getType() == Tunnel)
			{
				XYZRotation = eulerAngleX(BHorientation);
			}
			else if (Objects[i]->getType() == BlackHole)
			{
				XYZRotation = eulerAngleX(BHorientation) * eulerAngleY(orientation*37);
			}
			else if (Objects[i]->getType() == Reward)
			{
				//Translation = Translation * (translate(mat4(), vec3(1.0f, orientation_sin, 1.0f)));
				XYZRotation = eulerAngleYXZ(orientation_sin, 0.0f, 0.0f);
			}
			else
			{
				XYZRotation = glm::mat4(1);
			}

			glm::mat4 NewModelMatrix = Translation * XYZRotation * Scaling;
			Objects[i]->setModelMatrix(NewModelMatrix);
			Objects[i]->Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
		}

        handleSpaceShipCollision(SpaceGhost, getSSPosition().x, (getSSPosition().y), (getSSPosition().z));
	    glm::mat4 SpaceshipScaling = scale(mat4(), vec3(0.25f, 0.25f, 0.5f));
        glm::mat4 SpaceshipTranslation = translate(mat4(), getSSPosition());
		//SpaceShip.translateObject(getCameraPosition().x, (getCameraPosition().y - 0.5f), (getCameraPosition().z) - 3.0f);
        glm::mat4 SSModel = SpaceshipTranslation*SpaceshipScaling;
		SpaceGhost.setModelMatrix(SSModel);
		SpaceGhost.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glfwSwapBuffers();	//Swap buffers

    }

    while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    glfwTerminate();
    return 0;
}


void handleSpaceShipCollision(Spaceship& SS, float dx, float dy, float dz)
{
	bool collision;
	SS.translateBoundingBox(dx, dy, dz);
	for (int i = 0; i < nObjects; i++)
	{
		collision = Objects[i]->detectCollision(SS.getBoundingBox());
		if (collision)
		{
			int type = Objects[i]->getType();
			cout << "\n Collision with " << type;
			switch (type) //GAME LOGIC
			{
				case 1: //Rewarding

				break;

				case 2: //LightTunnel

				break;

				case 3: //Collision--> End Game !

				break;

				default: //FinishLine

				break;
			}
		}
	}
}
