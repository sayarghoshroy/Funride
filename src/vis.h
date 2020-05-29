#include "main.h"

#ifndef VIS_H
#define VIS_H


class Vis {
public:
    Vis() {}
    Vis(color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int direction);
    double speed;
    double x_speed;
    double y_speed;
private:
    VAO *object;
};

#endif // BALL_H
