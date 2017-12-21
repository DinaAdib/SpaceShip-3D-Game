#pragma once
#include "Spaceship.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define FinishLine 1
#define Tunnel 2
#define EndOfGame 3
#define BlackHole 4
#define Gift 5
#define Fuel 6

vector<ObjectModel*> Objects(200);
char*Textures[] = {"Meteroids.bmp","Asteroids.bmp","Saturn.bmp","Mars.bmp","Mercury.bmp","Venus.bmp","Earth.bmp","Blackhole.bmp","LightTunnel.bmp", "Gift.bmp","Fuel.bmp","FinishLine.bmp"};
char*ObjectNames[]={"Meteroids.obj","Asteroids.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Sphere.obj","Blackhole.obj","Blackhole.obj","Gift.obj","Fuel.obj","Blackhole.obj"};
Buffers ObjectBuffers[] = { Buffers("Meteroids.obj") , Buffers("Asteroids.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Sphere.obj"), Buffers("Sphere.obj") , Buffers("Sphere.obj") , Buffers("Blackhole.obj") , Buffers("Blackhole.obj"), Buffers("Gift.obj"), Buffers("Fuel.obj") , Buffers("Blackhole.obj") };
void handleSpaceShipCollision(Spaceship& SS , float dx , float dy , float dz);
int nObjects = 0;
bool endofGame = false;
int nSpeed = 0;
int SSvibration = 0.05;


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
    if (glewInit() != GLEW_OK)
    {
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

    for (int j = 0; j < 12; j++)
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
    initText2D( "LCDish.tga" );

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


        if(!endofGame){
            for (int i = 0; i < nObjects; i++)
            {
                if(nSpeed>0)
                            {
                                Objects[i]->translateObject(0, 0, 0.5);
                                incrementSpeed(1);
                                nSpeed--;
                                printf("\n Current Speed %d", nSpeed);
                            }
                else {decrementFuel();}
                            int objectZ = Objects[i]->getCenter().z;
                            if (objectZ > getSSPosition().z)
                            {
                                float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0);
                                float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/3.0);
                                Objects[i]->translateObject(r1-r2, r2-r1, getSSPosition().z-10);
                            }
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
                else if (Objects[i]->getType() == Gift || Objects[i]->getType() == Fuel || Objects[i]->getType() == FinishLine)
                {
                    //Translation = Translation * (translate(mat4(), vec3(1.0f, orientation_sin, 1.0f)));
                    XYZRotation = eulerAngleYXZ(orientation_sin, 0.0f, 0.0f);
                }
                else
                {
                    XYZRotation = glm::mat4(1);
                }
                Objects[i]->rotateObject(XYZRotation);
                Objects[i]->Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
            }
            if (nSpeed > 0) {
                        SpaceGhost.translateObject(0, -SSvibration, 0);
                    }
            handleSpaceShipCollision(SpaceGhost, getSSPosition().x, getSSPosition().y, getSSPosition().z);
            glm::mat4 SpaceshipScaling = scale(mat4(), vec3(0.25f, 0.25f, 0.5f));
            glm::mat4 SpaceshipTranslation = translate(mat4(), getSSPosition());
            glm::mat4 SSModel = SpaceshipTranslation*SpaceshipScaling;
            SpaceGhost.setModelMatrix(SSModel);
            SpaceGhost.Draw(programID, MatrixID, vertexPosition_modelspaceID, vertexUVID, TextureID);
            char text[256] = "Fuel";
            printText2D(text,10,500, 25);

            char time[256] = "Time";
            printText2D(time,10,450, 25);

            //Displaying score
            if (getFuelLeft() > 0)
            {

                char currentFuel[256];
                snprintf(currentFuel, sizeof(currentFuel), "%d", getFuelLeft());
                printText2D(currentFuel,130,500, 25);
              //  cout << "current score " << currentScore <<endl;
               // cout << "Fuel Left = " << getFuelLeft() << endl;
            }
            else
            {
                cout << "Game Over" << endl;
                break;
            }
            decrementTime(1);
            if (getLeftTime() > 0)
            {
                char currentTime[256];
                snprintf(currentTime, sizeof(currentTime), "%d", getLeftTime());
                printText2D(currentTime,130,450, 25);
            }
            else
            {
                endofGame = true;
                break;
            }

        }
        else {
            char GameOver[256] = "Game Over";
            printText2D(GameOver,150,300, 50);
        }

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
        collision = Objects[i]->detectCollision(SS.getRadius() , SS.getCenter());
        if (collision)
        {
            int type = Objects[i]->getType();
            cout << "Collision with " << type << " ";

            switch (type) //GAME LOGIC
            {
                case 1: //FinishLine
                    cout << "Congratulations" << endl;
                    //beep(950, 1000);
                    EndGame();
                    break;

                case 2: //LightTunnel => Increase Speed
                    cout << "Collision with a Light Tunnel" << endl;
                    nSpeed = 500;
                  //  beep(150, 1000);
                    incrementSpeed(60);
                    break;

                case 3: //Planet or Meteriod => Game Over
                    cout << "Collision with a Planet" << endl;
                    //Beep(400, 1000);
                    endofGame = true;
                    EndGame();

                case 4: //BlackHole => decrease Speed
                    cout << "Collision with a BlackHole" << endl;
                   // beep(200, 1000);
                //    beep(1000, 500);
                    decrementSpeed(1);
                    decrementTime(20);
                    break;

                case 5://Gift => increase Time Left
                    cout << "Collision with a Gift"<< endl;
                //    Beep(1000, 500);
                    incrementTime(20);
                    break;

                case 6://Fuel => increase Fuel
                    cout << "Collision with a Fuel" << endl;
                  //  beep(1000, 500);
                    incrementFuel(20);
                    break;

                default:
                    break;
            }
        }
    }
}
