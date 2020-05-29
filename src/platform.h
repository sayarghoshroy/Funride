#include "main.h"

#ifndef PLATFORM_H
#define PLATFORM_H


class Platform {
public:
    Platform() {}
    Platform(color_t color);
    glm::vec3 position;
    float rotation;
    double speed;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // PLATFORM_H
