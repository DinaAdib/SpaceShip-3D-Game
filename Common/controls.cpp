#include <GL/glfw.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.hpp"
using namespace glm;
using namespace std;

float rightFactor;
int fuel = 1000000;
int GameTime = 1200000;

bool fuelRunOut = false;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec3 position = glm::vec3(0, 0, 5);
glm::vec3 SSPosition = glm::vec3(0, 0.5, 8);

glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

glm::vec3 getCameraPosition()
{
    return position;
}

glm::vec3 getSSPosition()
{
    //SSPosition.x = position.x;
    SSPosition.y = position.y - 0.5f;
    SSPosition.z = position.z - 3.0f;
    return SSPosition;
}

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float horizontalSpeed = 6, verticalSpeed = 12, forwardSpeed = 40.0f; // 3 units / second
float mouseSpeed = 0.005f;
bool wasPressed = false;
float factor = 0.0f, tunnelFactor = 1.0f;
bool firstTime = true;
int xpos, ypos;

void computeMatricesFromInputs()
{
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    if(firstTime)
    {
        firstTime = false;
        glfwGetMousePos(&xpos, &ypos);
    }

    // Reset mouse position for next frame
    // EDIT : Doesn't work on Mac OS, hence the code below is a bit different from the website's
    //glfwSetMousePos(1024/2, 768/2);

    // Compute new orientation
    horizontalAngle = 3.14f + mouseSpeed * float(1024 / 2 - xpos);
    verticalAngle = mouseSpeed * float(768 / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction
    (
        0,
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3
    (
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);
    //if was pressed last time, increment and multiply by speed as a range from 0 to 1, else start over.


    if (glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (wasPressed && factor < 1)
        {
            factor += 0.0002;
        }
        else if (!wasPressed)
        {
            factor = 0.0;
        }
        wasPressed = true;
        if (!fuelRunOut)
        {
            position += direction * deltaTime * forwardSpeed * factor*tunnelFactor;
        }
    }
    if (glfwGetKey(GLFW_KEY_SPACE) != GLFW_PRESS && factor > 0)
    {
        factor -= 0.0001;
        if (!fuelRunOut)
        {
            position += direction * deltaTime * forwardSpeed * factor*tunnelFactor;
        }
    }

    if (fuel <= 0)
    {
        fuelRunOut = true;
    }
    // Move Up
    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
    {
        position += up * deltaTime * verticalSpeed;
    }
    // Move Down
    if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        position -= up * deltaTime * verticalSpeed;
    }
    // Strafe right
    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (SSPosition.x <= 1)
        {
            SSPosition += right * deltaTime * horizontalSpeed;
        }
    }
    // Strafe left
    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (SSPosition.x >= -1)
        {
            SSPosition -= right * deltaTime * horizontalSpeed;
        }
    }

    float FoV = initialFoV - 5 * glfwGetMouseWheel();

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt
    (
        position,           // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

void incrementSpeed(int amount)
{
    forwardSpeed += amount;
}
void decrementSpeed(int amount)
{
    forwardSpeed -= amount;
}
void stop()
{
    forwardSpeed = 40.0f;
    factor = 0;
}

void incrementFuel(int amount)
{
    fuel += amount;
}
void decrementFuel()
{
    fuel -= factor;
}
int getFuelLeft()
{
    return fuel;
}
int getLeftTime()
{
    return GameTime;
}
void EndGame()
{
    fuel = 0;
    GameTime = 0;
}
