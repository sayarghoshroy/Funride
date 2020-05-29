#include "main.h"

#ifndef THRUST_H
#define THRUST_H


class Thrust {
public:
    Thrust() {}
    Thrust(double x, double y, color_t color);
    glm::vec3 position;
    double rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
