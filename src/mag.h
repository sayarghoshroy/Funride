#include "main.h"

#ifndef MAG_H
#define MAG_H


class Mag {
public:
    Mag() {}
    Mag(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    
private:
    VAO *object;
};

#endif // BALL_H
