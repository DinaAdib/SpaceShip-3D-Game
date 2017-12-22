#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>//rotation
#include <glm/gtx/norm.hpp>//rotation
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/text2D.hpp>

enum state{
    MENU,
    GAMESTARTED,
    GAMEENDED
};
