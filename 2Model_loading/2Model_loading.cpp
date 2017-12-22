#pragma once
#include "Spaceship.h"
#include <iostream>
#include <fstream>
#include <string>
//#include <IL/il.h>
#include <GL/glut.h>
//#include <Windows.h>

using namespace std;

#define FinishLine 1
#define Tunnel 2
#define EndOfGame 3
#define BlackHole 4
#define Gift 5
#define Fuel 6

/*---------Global Variables--------*/
vector<ObjectModel*> Objects(200);
char*Textures[] = {"Meteroids.bmp","Asteroids.bmp","Saturn.bmp","Mars.bmp","Mercury.bmp","Venus.bmp","Earth.bmp","Blackhole.bmp","LightTunnel.bmp", "Gift.bmp","Fuel.bmp","FinishLine.bmp"};
char*ObjectNames[]={"Meteroids.obj","Asteroids.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Blackhole.obj","Blackhole.obj","Gift.obj","Fuel.obj","Blackhole.obj"};
Buffers ObjectBuffers[] = { Buffers("Meteroids.obj") , Buffers("Asteroids.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj"), Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Blackhole.obj") , Buffers("Blackhole.obj"), Buffers("Gift.obj"), Buffers("Fuel.obj") , Buffers("Blackhole.obj") };
void handleSpaceShipCollision(Spaceship& SS , float dx , float dy , float dz);
int nObjects = 0,nSpeed = 0, SSvibration = 0.05,nbFrames = 0;
bool endofGame = false, inTunnel = false;
GLuint programID, MatrixID , vertexPosition_modelspaceID, TextureID;
// For speed computation
double lastTime = glfwGetTime();
double lastFrameTime = lastTime;
float orientation = 0.0f, orientation_sin = 0.0f, BHorientation = 3.14159f / 2.0f;
double currentTime = glfwGetTime();
float deltaTime = (float)(currentTime - lastFrameTime);
char fuelText[256] = "Fuel";
char timeText[256] = "Time";
char GameOver[256] = "Game Over";
state GameState = MENU;

/*---------Functions' Headers--------*/
void ObjectLoader(int id , float dx , float dy , float dz, float sx, float sy, float sz, float rx, float ry, float rz);
void SceneReader(char* filename);
bool initialize();
void doInitialComputations(ObjectModel &SkySphere, GLuint vertexUVID);
void play(Spaceship &SpaceGhost, GLuint &vertexUVID);
void showMenu();
void cleanUp();

int main(void)
{
    // Initialise GLFW
    if(!initialize()) return -1;

  GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
  Spaceship SpaceGhost;
  ObjectModel SkySphere("Skysphere.bmp", &ObjectBuffers[3], 0);

    do
    {
        doInitialComputations(SkySphere, vertexUVID);

        switch(GameState) {
        case MENU:
            showMenu();
            break;
        case GAMESTARTED:
            play(SpaceGhost, vertexUVID);
            break;
        case GAMEENDED:
            printText2D(GameOver,150,270, 50);
            break;
        default:
            break;
        }


        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glfwSwapBuffers();	//Swap buffers
    }

    while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));

    // Cleanup VBO and shader
   cleanUp();
    return 0;
}


/*---------Functions----------*/
bool initialize(){
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return false;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }

    glfwSetWindowTitle("Guardians of the Galaxy");

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
    programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");

    // Get a handle for our buffers
    vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");


    // Get a handle for our "myTextureSampler" uniform
    TextureID = glGetUniformLocation(programID, "myTextureSampler");
    for (int j = 0; j < 12; j++)
    {
        ObjectBuffers[j].Create();
    }


    SceneReader("Scene.txt");
    initText2D( "LCDish.tga" );

    return true;
}

void SceneReader(char* filename) //Scene.txt
{
    int ID;
    float dX, dY, dZ;
    float sX, sY, sZ;
    float rX, rY, rZ;
    string line;
    ifstream myfile(filename);
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
    else if (id == 9)
    {
        type = Gift;
    }
    else if (id == 10)
    {
        type = Fuel;
    }
    else if (id == 11)
    {
        type = FinishLine;
    }
    else
    {
        type = EndOfGame;
    }
    Objects[nObjects] = new ObjectModel(Textures[id],&ObjectBuffers[id],type);
    glm::vec3 Scaling( sx, sy, sz);
    glm::vec3 Rotation (rx, ry, rz);
    glm::vec3 Translation(dx, dy, dz);
    Objects[nObjects]->constructModelMatrix(Translation,Scaling, Rotation);
    nObjects++;
}

void showMenu(){
  // printText2D("Guardians of the Galaxy", 70,480, 30);
}

void handleSpaceShipCollision(Spaceship& SS, float dx, float dy, float dz)
{
    bool collision;
    SS.translateBoundingBox(dx, dy, dz);
    for (int i = 0; i < nObjects; i++)
    {
        collision = Objects[i]->detectCollision(SS.getRadius() , SS.getCenter());
        if (collision)
        {
            int type = Objects[i]->getType();
            cout << "Collision with " << type << " ";

            switch (type) //GAME LOGIC
            {
                case 1: //FinishLine
                    cout << "Congratulations" << endl;
                  //  Beep(950, 1000);
                    EndGame();
                    break;

                case 2: //LightTunnel => Increase Speed
                    cout << "Collision with a Light Tunnel" << endl;
                    nSpeed = 1000;
                    //Beep(150, 1000);
                    inTunnel = true;
                    incrementSpeed(1000);
                    break;

                case 3: //Planet or Meteriod => Game Over
                    cout << "Collision with a Planet" << endl;
                    //Beep(400, 1000);
                    endofGame = true;
                    EndGame();
                break;
                case 4: //BlackHole => decrease Speed
                    cout << "Collision with a BlackHole" << endl;
                    //Beep(200, 1000);
                    decrementSpeed(1);
                    decrementTime(20);
                    break;

                case 5://Gift => increase Time Left
                    cout << "Collision with a Gift"<< endl;
                    //Beep(1000, 500);
                    incrementTime(20);
                    break;

                case 6://Fuel => increase Fuel
                    cout << "Collision with a Fuel" << endl;
                    //Beep(1000, 500);
                    incrementFuel(20);
                    break;

                default:
                    break;
            }
        }
    }
}

void doInitialComputations(ObjectModel &SkySphere,GLuint vertexUVID){

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Use our shader
      glUseProgram(programID);

      // Measure speed

      lastFrameTime = currentTime;
      nbFrames++;
      if (currentTime - lastTime >= 1.0)
      {
          nbFrames = 0;
          lastTime += 1.0;
      }
      orientation += 3.14159f / 6.0f * deltaTime;
      orientation_sin = sin(3.14159f / 2.0f * currentTime);

      //Draw SkySphere
      computeMatricesFromInputs();
      glm::mat4 SceneScaling = scale(mat4(), vec3(100.0f, 100.0f, 100.0f));
      glm::mat4 SceneTranslation = translate(mat4(), getCameraPosition());
      glm::mat4 SceneModel = SceneTranslation* SceneScaling;
      SkySphere.setModelMatrix(SceneModel);
      SkySphere.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
}

glm::mat4 determineRotation(ObjectModel* Object){
     glm::mat4 XYZRotation;
    if (Object->getType() == EndOfGame)
    {
        XYZRotation = eulerAngleY(orientation) * eulerAngleZ(orientation);
    }
    else if (Object->getType() == Tunnel)
    {
        XYZRotation = eulerAngleX(BHorientation);
    }
    else if (Object->getType() == BlackHole || Object->getType() == FinishLine)
    {
        XYZRotation = eulerAngleX(BHorientation) * eulerAngleY(orientation*37);
    }
    else if (Object->getType() == Gift || Object->getType() == Fuel)
    {
        //Translation = Translation * (translate(mat4(), vec3(1.0f, orientation_sin, 1.0f)));
        XYZRotation = eulerAngleYXZ(orientation_sin, 0.0f, 0.0f);
    }
    else
    {
        XYZRotation = glm::mat4(1);
    }
    return XYZRotation;
}

void drawSpaceship(Spaceship &SpaceGhost, GLuint &vertexUVID){
    glm::mat4 SpaceshipScaling = scale(mat4(), vec3(0.25f, 0.25f, 0.5f));
    glm::mat4 SpaceshipTranslation = translate(mat4(), getSSPosition());
    glm::mat4 SSModel = SpaceshipTranslation*SpaceshipScaling;
    SpaceGhost.setModelMatrix(SSModel);
    SpaceGhost.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
    return;
}

void drawObject(ObjectModel* Object, Spaceship &SpaceGhost, GLuint &vertexUVID){

    if(nSpeed>0)
    {
        Object->translateObject(0, 0, 0.5);
      //  decrementSpeed(0.0001);
        if(nSpeed == 1) inTunnel = false;
        nSpeed--;

        SpaceGhost.translateObject(0, -SSvibration, 0);
        printf("\n Current Speed %d", nSpeed);
    }
    if(!inTunnel){
        decrementFuel();
    }

    int objectZ = Object->getCenter().z;
    if (objectZ > getSSPosition().z)
    {
        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0);
        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/3.0);
        Object->translateObject(r1-r2, r2-r1, getSSPosition().z-10);
    }

    glm::mat4 XYZRotation = determineRotation(Object);

    Object->rotateObject(XYZRotation);
    Object->Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
}

void play(Spaceship &SpaceGhost, GLuint &vertexUVID){
    for (int i = 0; i < nObjects; i++)
    {
       drawObject(Objects[i], SpaceGhost, vertexUVID);
    }

    handleSpaceShipCollision(SpaceGhost, getSSPosition().x, getSSPosition().y, getSSPosition().z);

    drawSpaceship(SpaceGhost,vertexUVID);

    printText2D(fuelText,10,500, 25);
    printText2D(timeText,10,450, 25);

    char currentFuel[256], currentTime[256];
    //Displaying score
    if (getFuelLeft() > 0 && getLeftTime() > 0)
    {
        snprintf(currentFuel, sizeof(currentFuel), "%d", getFuelLeft());
        printText2D(currentFuel,130,500, 25);

        snprintf(currentTime, sizeof(currentTime), "%d", getLeftTime());
        printText2D(currentTime,130,450, 25);
    }
    else endofGame = true;

    decrementTime(1);
    return;
}

void cleanUp(){
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    glfwTerminate();
}
