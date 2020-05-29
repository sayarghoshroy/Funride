#include "main.h"

#ifndef SEMI_H
#define SEMI_H


class Semi {
public:
    Semi() {}
    Semi(double x, double y, double size, color_t color);
    glm::vec3 position;
    double rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
