#ifndef GETTING_STARTED_CAMERA_H
#define GETTING_STARTED_CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

//Defines several possible options for camera movement. Used as abstraction to stay away from window system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



#endif //GETTING_STARTED_CAMERA_H