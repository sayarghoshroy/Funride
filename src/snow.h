#include "main.h"

#ifndef SNOW_H
#define SNOW_H


class Snow {
public:
    Snow() {}
    Snow(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(double gravity);
    double y_speed;
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
