#ifndef PLAYER_H
#define PLAYER_H

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Player
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    Player();
    ~Player();

    void Init(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0,1,0));
    void Reset();
    void Update(double dt);

private:
    float rotationSpeed = 1.0f;
    float movementSpeed = 1.0f;
    float mouseSensitivity = 0.1f;


    // camera rotaiotn
    float yaw = -90.0f;
    float pitch = 0.0f;

    float targetYaw = -90.0f;
    float targetPitch = 0.0f;

    float rotationSmoothness = 40.0f; // higher is more snappier

    // camera tilt
    float roll = 0.0f;
    float targetRoll = 0.0f;

    float maxTilt = 2.f;
    float tiltSmoothness = 10.0f;

    bool isDirty;   // indicate if there is a need to recalculate the camera attributes

    void Refresh();
};

#endif

