#include "main.h"

#ifndef ENEMY_BAR_H
#define ENEMY_BAR_H


class Enemy_bar {
public:
    Enemy_bar() {}
    Enemy_bar(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int orientation;
    double speed;
    double y_speed;
private:
    VAO *object;
};

#endif // BALL_H
