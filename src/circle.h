#include "main.h"

#ifndef CIRCLE_H
#define CIRCLE_H


class Circle {
public:
    Circle() {}
    Circle(double x, double y, double size, color_t color);
    glm::vec3 position;
    double rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
