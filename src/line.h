#include "main.h"

#ifndef LINE_H
#define LINE_H


class Line {
public:
    Line() {}
    Line(float x, float y, int angle, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
